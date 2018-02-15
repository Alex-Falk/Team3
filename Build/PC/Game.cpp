#include "Game.h"
#include <ncltech\SceneManager.h>
void Game::Update(float dt)
{ 
	if (user) 
	{
		user->UpdateUser(dt); 
	} 

}

void Game::ResetGame()
{
	for (uint i = 0; i < 4; ++i)
	{
		SceneManager::Instance()->GetCurrentScene()->RemoveGameObject(avatars[i]);
		delete avatars[i];
		avatars[i] = nullptr;
	}
	delete user;
	user = nullptr;
	enet_deinitialize();
}