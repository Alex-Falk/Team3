//Michael Davis 16/02/2018
#pragma once

#include "GamePlay.h"
#include "../ncltech/GameObject.h"
#include "../ncltech/SphereCollisionShape.h"
#include "CaptureArea.h" 
#include "Avatar.h"
#include "../ncltech/SceneManager.h"
#include "MinionBlackboard.h"
#include "State.h"


class Minion : public GameObject {
protected:
	Colour colour;
	Vector4 RGBA;
	
	float life;
	float minLife;
	float maxLife;
	float size;

	MinionBlackboard minionBlackboard;
	
	Vector3 lastPos;

	bool isGrounded;
	bool dead; 

	State<Minion> *currentState;
	State<Minion> *previousState;

	Avatar * closestPlayer;
	CaptureArea * closestCaptureArea;

	Vector3 wanderPosition;
	float wanderTimer;
	float closestPlayerTimer;

public:
	

	Minion();
	Minion(Colour c, Vector4 RGBA, Vector3 position, const string name = "");
	
	void SetColour(Colour c) { colour = c;	}
	Colour GetColour() { return colour; }

	void SetRGBA(Vector4 r) { RGBA = r; }
	Vector4 GetRGBA() { return RGBA; }

	void SetDead(bool b) { dead = b; }
	bool GetDead() { return dead; }

	void SetIsGrounded(bool b) { isGrounded = b; }
	bool GetIsGrounded() { return isGrounded; }

	MinionBlackboard* GetMinionBlackBoard() { return &minionBlackboard; }

	void ChangeLife(float l);
	float GetLife() { return life; }

	void ChangeState(State<Minion>* newState);
	void RevertState();

	string GetCurrentState() { return currentState->GetState(); }

	void ChangeSize(float newSize);

	void Update(float dt);
	bool MinionCallbackFunction(PhysicsNode* self, PhysicsNode* collidingObject);
	
	Avatar * GetClosestPlayer() { return closestPlayer; }
	CaptureArea * GetClosestCaptureArea() { return closestCaptureArea; }

	Vector3 GetWanderPosition() { return wanderPosition; }
	void ComputeNewWanderPosition();
	void ComputeClosestPlayer();

	~Minion();
};