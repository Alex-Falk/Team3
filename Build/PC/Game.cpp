#include "Game.h"
#include <ncltech\SceneManager.h>
void Game::Update(float dt)
{ 
	if (user)
	{
		user->UpdateUser(dt);
	}

	if (gameRunning)
	{
		gameTime += dt;

		if (getUserID() == 0)
		{
			for (uint i = 0; i < playerNumber; ++i)
			{
				teamScores[i] = GraphicsPipeline::Instance()->GetScore(i) + teamAreaScores[i];
			}
		}
		//NCLDebug::Log(to_string(gameTime));
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
	gameTime = 0;
}

void Game::ClaimPickup(Avatar * player, Pickup * pickup)
{
	((Client*)user)->RequestPickup(getUserID(), pickup->GetName());
}