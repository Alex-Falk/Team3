//Michael Davis 16/02/2018
#pragma once

#include "MinionBase.h"
#include "MinionBlackboard.h"
#include "State.h"


class Minion : public MinionBase {
protected:


	MinionBlackboard minionBlackboard;
	
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
	

	MinionBlackboard* GetMinionBlackBoard() { return &minionBlackboard; }

	void ChangeState(State<Minion>* newState);
	void RevertState();

	string GetCurrentState() { return currentState->GetState(); }

	virtual void Update(float dt);

	Avatar * GetClosestEnemyPlayer() { return closestEnemyPlayer; }
	Avatar * GetClosestFriendlyPlayer() { return closestFriendlyPlayer; }
	CaptureArea * GetClosestCaptureArea() { return closestCaptureArea; }

	//TODO: change physics engine to hold sperate lists of objects
	bool IncomingProjectile() {/*Needs implimentation*/ return false; };
	float GetDetectionRadiusSQ() { return detectionRadiusSQ; }
	float GetPursueRadiusSQ() { return pursueRadiusSQ; }

	float DistanceToClosestFriendlySQ() {
		ComputeClosestFriendlyPlayer();
		if (closestFriendlyPlayer) return (physicsNode->GetPosition() - closestFriendlyPlayer->Physics()->GetPosition()).LengthSQ();
		else return detectionRadiusSQ + 1;
	}
	float HealthOfClosestFriendly() {  return closestFriendlyPlayer->GetLife(); }
	float GetAllyHealPursueLimit() { return allyHealPursueLimit; }

	float DistanceToClosestEnemySQ() { 
		ComputeClosestEnemyPlayer();
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