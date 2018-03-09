#include "Game.h"
#include <ncltech\SceneManager.h>
#include <networking\Client.h>
#include <networking\Server.h>
#include "Minion.h"
//--------------------------------------------------------------------------------------------//
// Setters
//--------------------------------------------------------------------------------------------//

void Game::SetServer()
{
	if (user) { SAFE_DELETE(user) };
	user = new Server();
	isHost = true;
}

void Game::SetClient(IP ip)
{
	if (user) { SAFE_DELETE(user) };
	user = new Client(ip);
}

Scene * Game::GetMap()
{
	return (SceneManager::Instance()->GetCurrentScene());
}

//--------------------------------------------------------------------------------------------//
// Utility
//--------------------------------------------------------------------------------------------//

void Game::Update(float dt)
{ 
	perfNetwork.UpdateRealElapsedTime(updateTimestep);


	if (user)
	{
		perfNetwork.BeginTimingSection();
		user->UpdateUser(dt);
		perfNetwork.EndTimingSection();
	}

	if (gameRunning)
	{
		time += dt;

		if (GetUserID() == 0)
		{
			for (uint i = 0; i < playerNumber; ++i)
			{
				teamScores[i] = GraphicsPipeline::Instance()->GetScore(i) + teamAreaScores[i];
			}
		}
		//NCLDebug::Log(to_string(gameTime));


		if (time > gameLength) {
			DetermineWinner();
			//StopGame();
			//PhysicsEngine::Instance()->SetPaused(true);
			time = 0.0f;
		}
	}
}

void Game::ResetGame()
{
	for (uint i = 0; i < Game::Instance()->GetPlayerNumber(); ++i)
	{
		if (avatars[i])
		{
			SceneManager::Instance()->GetCurrentScene()->RemoveGameObject(avatars[i]);
			avatars[i] = nullptr;
		}

	}
	if (user)
	{
		user->Disconnect();
		delete user;
		user = nullptr;
	}

	enet_deinitialize();
	gameRunning = false;
	time = 0.0f;
	GUIsystem::Instance()->SetResult(RESULT::NONE);
	PostProcess::Instance()->SetPostProcessType(PostProcessType::HDR_BLOOM);
	GUIsystem::Instance()->SetDrawResult(false);
	//PhysicsEngine::Instance()->SetPaused(false);
}

void Game::ClaimPickup(uint i)
{
	user->RequestPickup(GetUserID(), i);
}

void Game::Capture(uint i, Colour c)
{
	if (IsHost())
	{
		((Server*)user)->SendAreaCapture(i,c);
	}
}

void Game::SpawnMinion(MinionBase * minion)
{
	Map * m = static_cast<Map*>(GetMap());

	m->AddMinion(minion);

	if (GetUserID() == 0)
		((Server*)user)->SendMinionSpawn(m->GetMinionID(minion), minion->GetColour(), minion->Physics()->GetPosition());

}

void Game::KillMinion(MinionBase * minion)
{
	Map * m = static_cast<Map*>(GetMap());

	uint minionID = m->GetMinionID(minion);

	if (GetUserID() == 0)
	{
		((Server*)user)->SendMinionDeath(minionID);
	}
}



void Game::DetermineWinner() {
	int currentWinner = 0;
	float currentWinnerScore = 0;

	for (int i = 0; i < 4; i++) {
		if (teamScores[i] > currentWinnerScore) {
			currentWinner = i;
			currentWinnerScore = teamScores[i];
		}
	}

	//Determine whether the winner is this user
	GUIsystem::Instance()->SetDrawResult(true);
	if (currentWinner == GetUserID()) {
		PostProcess::Instance()->SetPostProcessType(PostProcessType::PERFORMANCE_BLUR);
		GUIsystem::Instance()->SetResult(RESULT::WIN);
	}
	else {
		PostProcess::Instance()->SetPostProcessType(PostProcessType::GRAYSCALE);
		GUIsystem::Instance()->SetResult(RESULT::LOST);
	}
	GUIsystem::Instance()->drawPlayerName = false;
}
