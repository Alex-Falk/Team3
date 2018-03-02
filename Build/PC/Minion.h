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
	float detectionRadiusSQ;
	float pursueRadiusSQ;
	float allyHealPursueLimit;  //Life level of ally where the minion will not pursue them

	State<Minion> *currentState;
	State<Minion> *previousState;

	Avatar * closestEnemyPlayer;
	Avatar * closestFriendlyPlayer;
	CaptureArea * closestCaptureArea;

	Vector3 wanderPosition;
	float wanderTimer;
	float closestPlayerTimer;
	float closestCaptureAreaTimer;

	float behaviourWeight;
	float behaviourXZMagnitude;

public:
	Minion();
	Minion(Colour c, Vector4 RGBA, Vector3 position, const string name = "");
	
	void SetColour(Colour c) { colour = c;	}
	Colour GetColour() { return colour; }

	void SetRGBA(Vector4 r) { RGBA = r; }
	Vector4 GetRGBA() { return RGBA; }

	void SetDead(bool b) { dead = b; }

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
	bool IsAlive() { return !dead; }
	
	Avatar * GetClosestEnemyPlayer() { return closestEnemyPlayer; }
	Avatar * GetClosestFriendlyPlayer() { return closestFriendlyPlayer; }
	CaptureArea * GetClosestCaptureArea() { return closestCaptureArea; }

	//TODO: change physics engine to hold sperate lists of objects
	bool IncomingProjectile() {/*Needs implimentation*/ return false; };
	float GetDetectionRadiusSQ() { return detectionRadiusSQ; }
	float GetPursueRadiusSQ() { return pursueRadiusSQ; }

	float DistanceToClosestFriendlySQ() {
		if (closestFriendlyPlayer) return (physicsNode->GetPosition() - closestFriendlyPlayer->Physics()->GetPosition()).LengthSQ();
		else return detectionRadiusSQ + 1;
	}
	float HealthOfClosestFriendly() {  return closestFriendlyPlayer->GetLife(); }
	float GetAllyHealPursueLimit() { return allyHealPursueLimit; }

	float DistanceToClosestEnemySQ() { 
		if (closestEnemyPlayer) return (physicsNode->GetPosition() - closestEnemyPlayer->Physics()->GetPosition()).LengthSQ();
		else return detectionRadiusSQ + 1;
	}
	float DistanceToEnemyZoneSQ() { 
		if (closestCaptureArea) return (physicsNode->GetPosition() - closestCaptureArea->Physics()->GetPosition()).LengthSQ();
		else return -1.0f;
	}

	Vector3 GetWanderPosition() { return wanderPosition; }
	void ComputeNewWanderPosition();
	void ComputeClosestEnemyPlayer();
	void ComputeClosestFriendlyPlayer();
	void ComputeClosestCaptureArea();

	bool WanderPositionInRange();

	~Minion();
};