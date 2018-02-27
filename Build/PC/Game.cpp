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
	delete user;
	user = nullptr;
	enet_deinitialize();
	gameRunning = false;
	gameTime = 0;
}