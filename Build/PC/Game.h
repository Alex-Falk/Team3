/*
           \\\///
		  / ~  ~ \
		(| (.)(.) |)
.-----.OOOo--C---oOOO.---.
|                        |
|   Made By Philip Beck  |
|      12/02/2018        |
|                        |
'-----.oooO--------------'
      (   )   Oooo.
       \ (    (   )
	    \_)    ) /
	          (_/
*/

//Extra functionality added by Alex Falk - 12/02/2018
//Extra functionality about score added by Nikos Fragkas 15/02/2018

#pragma once
#include "ControllableAvatar.h"
#include "GamePlay.h"
#include <networking\Client.h>
#include <networking\Server.h>
#include "Map.h"
#include "Pickup.h"
#include "CaptureArea.h"

class Game: public TSingleton<Game>
{
	friend class TSingleton <Game>;
public:

	//--------------------------------------------------------------------------------------------//
	// Setters
	//--------------------------------------------------------------------------------------------//

	inline void SetScore(uint id, int score)			{ teamScores[id] = (float)score; }

	inline void SetPlayerNumber(uint i)					{ playerNumber = i; }

	inline void SetPlayerName(uint id, string name)		{ userNames[id] = name; }

	inline void SetSize(uint id, float size)			{ avatars[id]->SetLife(size); }

	inline void SetAcceleration(uint id, Vector3 a)		{ avatars[id]->GetGameObject()->Physics()->SetAcceleration(a); }
	inline void SetLinearVelocity(uint id, Vector3 v)	{ avatars[id]->GetGameObject()->Physics()->SetLinearVelocity(v); }
	inline void SetAngularVelocity(uint id, Vector3 v)	{ avatars[id]->GetGameObject()->Physics()->SetAngularVelocity(v); }
	inline void SetPosition(uint id, Vector3 p)			{ avatars[id]->GetGameObject()->Physics()->SetPosition(p); }
	inline void SetGameLength(float f)					{ gameLength = f; }

	inline void SetServer() {
		if (user) { SAFE_DELETE(user) }; 
		user = new Server();
	}
	inline void setClient(IP ip) { 
		if (user) { SAFE_DELETE(user) }; 
		user = new Client(ip); 
	}

	inline void SetAvatar(uint id, Avatar * p)			{ avatars[id] = p; }

	//--------------------------------------------------------------------------------------------//
	// Getters
	//--------------------------------------------------------------------------------------------//

	inline uint GetPlayerNumber()			{ return playerNumber; }
	inline uint GetMapIndex()				{ return mapIdx; }
	inline uint getUserID()					{ return user->GetUserID(); }
	inline float GetGameLength()			{ return gameLength; }
	inline float GetTimeLeft()				{ return gameLength - time; }

	inline Avatar * GetPlayer(uint id)		{ return avatars[id]; }
	inline Avatar * GetCurrentAvatar()		{ return avatars[user->GetUserID()]; }

	inline User * GetUser()					{ return user; }

	Scene * GetMap();

	inline string GetName(uint id)			{ return userNames[id]; }

	inline float GetScore(uint id)			{ return teamScores[id]; }


	inline void StartGame(uint mapID = 0)	{ gameRunning = true; mapIdx = mapID; user->StartGame(mapID); }
	inline void StopGame()					{ gameRunning = false; }

	inline bool IsRunning()					{ return gameRunning; }


	//--------------------------------------------------------------------------------------------//
	// Utility
	//--------------------------------------------------------------------------------------------//

	void Update(float dt);
	void ResetGame();
	void DetermineWinner() {};

	void ClaimPickup(Pickup * pickup);
	void ClaimArea(CaptureArea *object);

	void KillMinion(MinionBase * minion);

	//--------------------------------------------------------------------------------------------//	Fragkas Nikolaos
	// Performance Timers																				Date: 02/03/2018
	//--------------------------------------------------------------------------------------------//	
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
			delete avatars[i];
		}
	};

	//--------------------------------------------------------------------------------------------//
	// Variables
	//--------------------------------------------------------------------------------------------//

	uint playerNumber = 0;
	uint mapIdx;

	float teamScores[4];
	Avatar* avatars[4];
	string userNames[4] = { "Player 1","Player 2","Player 3","Player 4" };

	User* user = nullptr;


	bool gameRunning = false;
	float updateTimestep = 1.0f / 60.f;

	//--------------------------------------------------------------------------------------------//	Fragkas Nikolaos
	// Performance Timers																				Date: 02/03/2018
	//--------------------------------------------------------------------------------------------//	
	PerfTimer perfNetwork;
	float time = 0.0f;
	float gameLength = 30.0f;
};