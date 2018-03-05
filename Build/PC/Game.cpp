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
}

void Game::ClaimPickup(Pickup * pickup)
{
	user->RequestPickup(getUserID(), pickup->GetName());
}

void Game::ClaimArea(CaptureArea * object)
{
	user->RequestCaptureArea(getUserID(), object->GetName());
}