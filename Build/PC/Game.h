#pragma once
#include "ControllableAvatar.h"
#include "GamePlay.h"
#include "ControllableAvatar.h"

class Game: public TSingleton<Game>
{
	friend class TSingleton <Game>;
public:
	//STUBS
	inline void SetScore(uint id, int score) { teamScores[id] = score; }
	inline void SetAmmo(uint id, float ammo) {}
	inline void SetAcceleration(uint id, Vector3 a) {}
	//FINISHED FUNCTIONS
	inline void SetSize(uint id, float size) { players[id]->SetSize(size); }
	inline void SetLinearVelocity(uint id, Vector3 v) { players[id]->GetGameObject()->Physics()->SetLinearVelocity(v); }
	inline void SetAngularVelocity(uint id, Vector3 v) { players[id]->GetGameObject()->Physics()->SetAngularVelocity(v); }
	inline void SetPosition(uint id, Vector3 p) { players[id]->GetGameObject()->Physics()->SetPosition(p); }


	// Getters
	//STUBS
	inline Vector3 GetSpawnLocation() {}
	//FINISHED FUNCTIONS
	inline ControllableAvatar * GetPlayer(uint id) { return players[id]; }
	inline int GetMapIndex() { return 0; }
private:
	//private constructor
	Game() {
		for (int i = 0; i < 4; i++) {
			players[i] = new ControllableAvatar();
		}
	};
	~Game() {
		for (int i = 0; i < 4; i++) {
			delete players[i];
		}
	};
	//variables
	ControllableAvatar* players[4];
	int teamScores[4];

	// Everything about score
	static const int groundScoreAccuracy = 100;
	static const int xOnGrid = (int)2 * (DIMENSION_X + 10)*groundScoreAccuracy; //Array cordinates for the x position of the player on the grid
	static const int yOnGrid = (int)2 * (DIMENSION_Y + 10)*groundScoreAccuracy; //Array cordinates for the y position of the player on the grid
	Colour ground[xOnGrid][yOnGrid];
	int groundTeamScore[5];

	void BuildGroundScore(); //Builds the array for the ground score
	void UpdateGroundScore(ControllableAvatar* player); //Updates the ground cells 
	void ChangeGridScore(Colour teamToDecrease, Colour teamToIncrease); // updates the score
	void PrintScore(int x); // debug
};