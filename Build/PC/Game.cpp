#include "Game.h"
#include <ncltech\SceneManager.h>
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

		if (getUserID() == 0)
		{
			for (uint i = 0; i < playerNumber; ++i)
			{
				teamScores[i] = GraphicsPipeline::Instance()->GetScore(i);
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
		SceneManager::Instance()->GetCurrentScene()->RemoveGameObject(avatars[i]);
		avatars[i] = nullptr;
	}
	user->Disconnect();
	delete user;
	user = nullptr;
	enet_deinitialize();
	gameRunning = false;
	time = 0.0f;
	GUIsystem::Instance()->SetResult(RESULT::NONE);
	PostProcess::Instance()->SetPostProcessType(PostProcessType::HDR_BLOOM);
	GUIsystem::Instance()->SetDrawResult(false);
	//PhysicsEngine::Instance()->SetPaused(false);
}

void Game::ClaimPickup(Avatar * player, Pickup * pickup)
{
	((Client*)user)->RequestPickup(getUserID(), pickup->GetName());
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

	/*
	if (currentWinner == RED) {
		cout << "Red Wins!" << endl;
	}
	else if (currentWinner == BLUE) {
		cout << "Blue Wins!" << endl;
	}
	else if (currentWinner == GREEN) {
		cout << "Green Wins!" << endl;
	}
	else if (currentWinner == PINK) {
		cout << "Pink Wins!" << endl;
	}
	*/

	//Determine whether the winner is this user
	GUIsystem::Instance()->SetDrawResult(true);
	if (currentWinner == getUserID()) {
		PostProcess::Instance()->SetPostProcessType(PostProcessType::PERFORMANCE_BLUR);
		GUIsystem::Instance()->SetResult(RESULT::WIN);
	}
	else {
		PostProcess::Instance()->SetPostProcessType(PostProcessType::GRAYSCALE);
		GUIsystem::Instance()->SetResult(RESULT::LOST);
	}
	GUIsystem::Instance()->drawPlayerName = false;
}