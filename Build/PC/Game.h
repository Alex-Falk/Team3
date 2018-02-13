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

#pragma once
#include "ControllableAvatar.h"
#include "GamePlay.h"
#include <networking\Client.h>
#include <networking\Server.h>

class Game: public TSingleton<Game>
{
	friend class TSingleton <Game>;
public:
	//STUBS
	inline void SetScore(uint id, int score) { scores[id] = score; }
	inline void SetAmmo(uint id, float ammo) {}
	//FINISHED FUNCTIONS
	inline void SetSize(uint id, float size) { avatars[id]->SetLife(size); }
	inline void SetAcceleration(uint id, Vector3 a) { avatars[id]->GetGameObject()->Physics()->SetAcceleration(a); }
	inline void SetLinearVelocity(uint id, Vector3 v) { avatars[id]->GetGameObject()->Physics()->SetLinearVelocity(v); }
	inline void SetAngularVelocity(uint id, Vector3 v) { avatars[id]->GetGameObject()->Physics()->SetAngularVelocity(v); }
	inline void SetPosition(uint id, Vector3 p) { avatars[id]->GetGameObject()->Physics()->SetPosition(p); }
	inline void SetServer() { user = new Server(); }
	inline void setClient(IP ip) { user = new Client(ip); }
	inline void SetAvatar(uint id, Avatar * p) { avatars[id] = p; }

	// Getters
	//STUBS
	inline Vector3 GetSpawnLocation() {}
	//FINISHED FUNCTIONS
	inline Avatar * GetPlayer(uint id) { return avatars[id]; }
	inline int GetMapIndex() { return 0; }
	inline int GetScore(uint id) { return scores[id]; }
	inline uint getUserID() { return user->GetUserID(); }
	inline User * GetUser() { return user; }



	void Update(float dt);
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
	Avatar* avatars[4];
	int scores[4];
	User* user = nullptr;
};