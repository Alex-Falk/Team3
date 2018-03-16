 /*               
                          .,okkkd:.                          
                       .:x0KOdooxKKkl,.                      
                   .,oOKKxc'. .. .;oOX0d:.                   
                ...oKOo;. .;dO00xc.  'cxKO, ..               
            .,lk0l...  .:oxXMMMMMWOoc'  .. ,O0d:.            
         .:d0XOo;.     ;c..kMMMMMK;.;:.     'ckKKkc'.        
      'lkKKxc'  .,.        oWMMMMO.        ''  .:d0KOo;.     
     '0Wk;. .,loo:.        :NMMMMx.        ,loo:. .,oXNc     
     ,0X: .lKWMKl.         ,KMMMWo         .;kWWXx' .kNc     
     '0X; .OMMMMWXx;.      ,0MMMNl       'o0NMMMMN: .kWc     
     '0X; .k0d0NWMMW0o,..cxKWMMMMXkl,..ckNMMMWKxkK: .kWc     
     '0X; .kl  ':okKWMNKXWMMMMMMMMMMNKXWWXOdc,. ,O: .kWc     
     '0X;  ,.      .,oXMMMMMMMMMMMMMMMWk;.      .;. .kNc     
     .,;.            '0MMMMMMMMMMMMMMMWc             ';.			Alexander Falk
     .lo.            '0MMMMMMMMMMMMMMMWc            .cd,			Client.h
     '0X: .:,     .,lkNMMMMMMMMMMMMMMMWKo:'.    .c' .OWl     
     '0X; .ko.':okXWMW0xkXWMMMMMMMMN0xkNMWN0xl;.:O: .OWc     
     '0X; .OX0NMMMWKx;.  .:xNMMMW0l,.  'lONMMMWKKX: .kWc     
     '0X: .OMMMMNkc.       '0MMMNc       .;dKWMMMN: .kWc     
     '0N: .;xKWKc.         ;XMMMWo          'kNXkl. .OWc     
     .xNKd:. .;loc.        cNMMMMk.       .;ol;. .,lONK;     
      .'lkKKkl,. .         dWMMWM0'        .  .:d0XOo;.      
          .:d0X0d,     ,l:;OMMMMMXl;lc.    .ckKKkc'          
             .,lxc.,c'. .:d0WMMMMXkl,. .;:.'dd:.             
                  .l0XOo;. .;oooc' .'cxKKx'                  
                    .,lkKKxc'.  .;oOK0d:.                    
                        .:d0K000KKkl,.                       
                           .,cll:.                            
*/

// Additions by Phillip Beck, Nickos Fragkas, Jianfei Zhou

#include "Game.h"
#include <ncltech\SceneManager.h>
#include <networking\Client.h>
#include <networking\Server.h>
#include "Minion.h"
#include "AudioSystem.h"
//--------------------------------------------------------------------------------------------//
// Setters
//--------------------------------------------------------------------------------------------//

// Make this user a server
void Game::SetServer()
{
	if (user) { SAFE_DELETE(user) };
	user = new Server();
	isHost = true;
}

// Make this user a client
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

	// If there is a network user, update it
	if (user)
	{
		perfNetwork.BeginTimingSection();
		user->UpdateUser(dt);
		perfNetwork.EndTimingSection();
	}

	// If the game is currently running, update timer and scores
	if (gameRunning)
	{
		time += dt;

		if (GetUserID() == 0)
		{
			for (uint i = 0; i < playerNumber; ++i)
			{
				teamScores[i] = GraphicsPipeline::Instance()->GetScore(i) + captureScores[i];
			}
		}

		// Stop the game - only the host does it, and once the host closes the clients get kicked to the menu as well
		if (IsHost() && PhysicsEngine::Instance()->IsPaused() && time > gameLength)
		{
			ResetGame();
			SceneManager::Instance()->JumpToScene(0);
		}
		// find a winner to the game, reset timer to 10 seconds
		else if (time > gameLength) {
			
			DetermineWinner();
			
			PhysicsEngine::Instance()->SetPaused(true);
			time = gameLength - 10.0f;
		}
	}
}

void Game::ResetGame()
{
	PhysicsEngine::Instance()->SetPaused(false);

	// delete all avatars
	for (uint i = 0; i < Game::Instance()->GetPlayerNumber(); ++i)
	{
		if (avatars[i])
		{
			SceneManager::Instance()->GetCurrentScene()->RemoveGameObject(avatars[i]);
			avatars[i] = nullptr;
		}

	}
	// disconnect and delete the user
	if (user)
	{
		user->Disconnect();
		delete user;
		user = nullptr;
	}

	// reset scores
	for (uint i = 0; i < 4; ++i)
	{
		teamScores[i] = 0;
		captureScores[i] = 0;
		userNames[i] = "Player " + to_string(i+1);
	}

	// deinitialize enet and reset variables
	enet_deinitialize();
	gameRunning = false;
	time = 0.0f;
	isHost = false;
	//reset GUI
	GUIsystem::Instance()->SetResult(RESULT::NONE);
	GUIsystem::Instance()->SetHasWeapon(false);
	GUIsystem::Instance()->SetDrawResult(false);
}

// connects pickup request to network, not done directly to avoid the rest of the game needing to know of any network functionality
void Game::ClaimPickup(uint i)
{
	user->RequestPickup(GetUserID(), i);
}

// connects area capture to network (only done by the host)
void Game::Capture(uint i, Colour c,int scorevalue)
{
	if (IsHost())
	{
		captureScores[c] += scorevalue;
		((Server*)user)->SendAreaCapture(i,c);
	}
}

// Add a minion to the map and send it across the network
void Game::SpawnMinion(MinionBase * minion)
{
	Map * m = static_cast<Map*>(GetMap());

	m->AddMinion(minion);

	if (IsHost())
		((Server*)user)->SendMinionSpawn(m->GetMinionID(minion), minion->GetColour(), minion->Physics()->GetPosition());

}

// Remove the minion from the map and send it over the network
void Game::KillMinion(MinionBase * minion)
{
	Map * m = static_cast<Map*>(GetMap());

	uint minionID = m->GetMinionID(minion);

	if (IsHost())
	{
		((Server*)user)->SendMinionDeath(minionID);
	}
}

// Find user with highest score
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
	GUIsystem::Instance()->RandomizeMessage();
	if (currentWinner == GetUserID()) {
		PostProcess::Instance()->SetPostProcessType(PostProcessType::PERFORMANCE_BLUR);
		GUIsystem::Instance()->SetResult(RESULT::WIN);
		AudioSystem::Instance()->PlayASound(VICTORY_SOUND, false);
	}
	else {
		PostProcess::Instance()->SetPostProcessType(PostProcessType::GRAYSCALE);
		GUIsystem::Instance()->SetResult(RESULT::LOST);
		AudioSystem::Instance()->PlayASound(LOSS_SOUND, false);
	}
	GUIsystem::Instance()->drawPlayerName = false;
}
