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
     .,;.            '0MMMMMMMMMMMMMMMWc             ';.			Alexander Falk & Phillip Beck
     .lo.            '0MMMMMMMMMMMMMMMWc            .cd,			Game.h
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
// Controls the game update. Only connection point between the Network and the rest of the game

#pragma once
#include "Avatar.h"
#include "GamePlay.h"
#include <networking\NetworkCommon.h>
#include "Map.h"
#include <networking\User.h>

class Pickup;
class CaptureArea;
class MinionBase;

class Game: public TSingleton<Game>
{
	friend class TSingleton <Game>;
public:

	//--------------------------------------------------------------------------------------------//
	// Setters
	//--------------------------------------------------------------------------------------------//

	// Helpers used by Network
	inline void SetScore(uint id, int score)			{ teamScores[id] = (float)score; }
	inline void SetPlayerNumber(uint i)					{ playerNumber = i; }
	inline void SetName(string name)					{ userNames[GetUserID()] = name; user->UpdateName(); }
	inline void SetPlayerName(uint id, string name)		{ userNames[id] = name; }
	
	inline void SetSize(uint id, float size)			{ avatars[id]->SetLife(size); }
	inline void SetAcceleration(uint id, Vector3 a)		{ avatars[id]->GetGameObject()->Physics()->SetAcceleration(a); }
	inline void SetLinearVelocity(uint id, Vector3 v)	{ avatars[id]->GetGameObject()->Physics()->SetLinearVelocity(v); }
	inline void SetAngularVelocity(uint id, Vector3 v)	{ avatars[id]->GetGameObject()->Physics()->SetAngularVelocity(v); }
	inline void SetPosition(uint id, Vector3 p)			{ avatars[id]->GetGameObject()->Physics()->SetPosition(p); }
	inline void SetGameLength(float f)					{ gameLength = f; }

	// Setters to set up current user
	void SetServer();
	void SetClient(IP ip);

	inline void SetAvatar(uint id, Avatar * p)			{ avatars[id] = p; }

	//--------------------------------------------------------------------------------------------//
	// Getters
	//--------------------------------------------------------------------------------------------//

	inline uint GetPlayerNumber()			{ return playerNumber; }
	inline uint GetMapIndex()				{ return mapIdx; }
	inline uint GetUserID()					{ return user->GetUserID(); }
	inline float GetGameLength()			{ return gameLength; }
	inline float GetTimeLeft()				{ return gameLength - time; }
	inline float GetTime()					{ return time; }

	inline Avatar * GetPlayer(uint id)		{ return avatars[id]; }
	inline Avatar * GetCurrentAvatar()		{ return avatars[user->GetUserID()]; }

	inline User * GetUser()					{ return user; }

	Scene * GetMap();

	inline string GetName(uint id)			{ return userNames[id]; }

	inline float GetScore(uint id)			{ return teamScores[id]; }


	inline void StartGame(uint mapID = 0)	{ gameRunning = true; user->StartGame(mapID); mapIdx = mapID;  }
	inline void StopGame()					{ gameRunning = false; }

	inline bool IsRunning()					{ return gameRunning; }
	inline bool IsHost()					{ return isHost; }

	//--------------------------------------------------------------------------------------------//
	// Utility
	//--------------------------------------------------------------------------------------------//

	void Update(float dt);
	void ResetGame();
	void DetermineWinner();

	void ClaimPickup(uint i);
	void Capture(uint i, Colour c,int scoreValue);

	void SpawnMinion(MinionBase * minion);
	void KillMinion(MinionBase * minion);

	//--------------------------------------------------------------------------------------------//	Fragkas Nikolaos
	// Performance Timers																				Date: 02/03/2018
	//--------------------------------------------------------------------------------------------//0
	void PrintPerformanceTimers(const Vector4& color)
	{
		perfNetwork.PrintOutputToStatusEntry(color, "            Network Update  :");
	}


private:
	//private constructor
	Game() 
	{
		for (uint i = 0; i < 4; i++) {
			avatars[i] = nullptr;
		}
	};

	~Game() 
	{
		for (int i = 0; i < 4; i++) {
			SAFE_DELETE(avatars[i]);
		}
		SAFE_DELETE(user);
	};

	//--------------------------------------------------------------------------------------------//
	// Variables
	//--------------------------------------------------------------------------------------------//

	uint playerNumber = 0;
	uint mapIdx;

	float teamScores[4];
	float captureScores[4];
	Avatar* avatars[4];
	string userNames[4] = { "Player 1","Player 2","Player 3","Player 4" };

	User* user = nullptr;
	bool isHost = false;


	bool gameRunning = false;
	float updateTimestep = 1.0f / 60.f;

	//--------------------------------------------------------------------------------------------//	Fragkas Nikolaos
	// Performance Timers																				Date: 02/03/2018
	//--------------------------------------------------------------------------------------------//	
	PerfTimer perfNetwork;
	float time = 0.0f;
	float gameLength = 180.0f;
};