#include "Game.h"

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
		delete avatars[i];
		avatars[i] = nullptr;
	}
	delete user;
	user = nullptr;
	enet_deinitialize();
}