#include "Game.h"

void Game::Update(float dt)
{ 
	if (user) 
	{
		user->UpdateUser(dt); 
	} 

}