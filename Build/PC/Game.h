#pragma once
#include "Player.h"
#include "GamePlay.h"

class Game: public TSingleton<Game>
{
	friend class TSingleton <Game>;
public:
	//STUBS
	inline void SetScore(uint id, int score) {}
	inline void SetAmmo(uint id, float ammo) {}
	inline void SetAcceleration(uint id, Vector3 a) {}
	//FINISHED FUNCTIONS
	inline void SetSize(uint id, float size) { players[id]->SetSize(size); }
	inline void SetLinearVelocity(uint id, Vector3 v) { players[id]->Physics()->SetLinearVelocity(v); }
	inline void SetAngularVelocity(uint id, Vector3 v) { players[id]->Physics()->SetAngularVelocity(v); }
	inline void SetPosition(uint id, Vector3 p) { players[id]->Physics()->SetPosition(p); }


	// Getters
	//STUBS
	inline Vector3 GetSpawnLocation() {}
	//FINISHED FUNCTIONS
	inline Player * GetPlayer(uint id) { return players[id]; }
	inline int GetMapIndex() { return 0; }
private:
	//private constructor
	Game() {
		for (int i = 0; i < 4; i++) {
			players[i] = new Player();
		}
	};
	~Game() {
		for (int i = 0; i < 4; i++) {
			delete players[i];
		}
	};
	//variables
	Player* players[4];
};