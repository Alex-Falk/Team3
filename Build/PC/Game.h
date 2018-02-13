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
#include "Player.h"
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
	inline void SetSize(uint id, float size) { players[id]->SetLife(size); }
	inline void SetAcceleration(uint id, Vector3 a) { players[id]->GetGameObject()->Physics()->SetAcceleration(a); }
	inline void SetLinearVelocity(uint id, Vector3 v) { players[id]->GetGameObject()->Physics()->SetLinearVelocity(v); }
	inline void SetAngularVelocity(uint id, Vector3 v) { players[id]->GetGameObject()->Physics()->SetAngularVelocity(v); }
	inline void SetPosition(uint id, Vector3 p) { players[id]->GetGameObject()->Physics()->SetPosition(p); }
	inline void SetServer() { user = new Server(); }
	inline void setClient(IP ip) { user = new Client(ip); }
	inline void SetPlayer(uint id, Player* player) { players[id] = player; }

	inline void Setplayer(uint id, Player * p) { players[id] = p; }

	// Getters
	//STUBS
	inline Vector3 GetSpawnLocation() {}
	//FINISHED FUNCTIONS
	inline Player * GetPlayer(uint id) { return players[id]; }
	inline int GetMapIndex() { return 0; }
	inline int GetScore(uint id) { return scores[id]; }



	void Update(float dt);
private:
	//private constructor
	Game() {
		for (uint i = 0; i < 4; i++) {
			//probably temporary for the location
			players[i] = nullptr;
		}
	};
	~Game() {
		for (int i = 0; i < 4; i++) {
			delete players[i];
		}
	};
	//variables
	Player* players[4];
	int scores[4];
	User* user = nullptr;
};