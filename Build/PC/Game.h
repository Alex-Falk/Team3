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
#include "ControllableAvatar.h"


class Game: public TSingleton<Game>
{
	friend class TSingleton <Game>;
public:
	//STUBS
	inline void SetAmmo(uint id, float ammo) {}
	//FINISHED FUNCTIONS
	inline void LoadLevel(int index) { SceneManager::Instance()->JumpToScene(index); }
	inline void SetPlayerNumber(uint i) { playerNumber = i; }
	inline void SetSize(uint id, float size) { avatars[id]->SetLife(size); }
	inline void SetAcceleration(uint id, Vector3 a) { avatars[id]->GetGameObject()->Physics()->SetAcceleration(a); }
	inline void SetLinearVelocity(uint id, Vector3 v) { avatars[id]->GetGameObject()->Physics()->SetLinearVelocity(v); }
	inline void SetAngularVelocity(uint id, Vector3 v) { avatars[id]->GetGameObject()->Physics()->SetAngularVelocity(v); }
	inline void SetPosition(uint id, Vector3 p) { avatars[id]->GetGameObject()->Physics()->SetPosition(p); }
	inline void SetServer() { user = new Server(); }
	inline void setClient(IP ip) { user = new Client(ip); }
	inline void SetAvatar(uint id, Avatar * p)
	{
		if (avatars[id])
			delete avatars[id];
		avatars[id] = p; 
	}

	// Getters
	//STUBS
	inline Vector3 GetSpawnLocation() {}
	//FINISHED FUNCTIONS
	inline  uint GetPlayerNumber() { return playerNumber; }
	inline Avatar * GetPlayer(uint id) { return avatars[id]; }
	inline Avatar * GetCurrentAvatar() { return avatars[user->GetUserID()]; }
	inline int GetMapIndex() { return 0; }
	inline uint getUserID() { return user->GetUserID(); }
	inline User * GetUser() { return user; }

	inline void StartGame() { gameRunning = true; user->StartGame(); }
	inline void StopGame() { gameRunning = false; }
	inline bool IsRunning() { return gameRunning; }

	void Update(float dt);
	void ResetGame();
private:
	//private constructor
	Game() {
		for (uint i = 0; i < 4; i++) {
			avatars[i] = nullptr;
		}
	};
	~Game() {
		for (int i = 0; i < 4; i++) {
			delete avatars[i];
		}
	};
	//variables
	uint playerNumber = 0;

	Avatar* avatars[4];
	User* user = nullptr;
	bool gameRunning = false;
};