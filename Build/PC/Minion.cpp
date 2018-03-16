//Michael Davis 16/02/2018
//Adapted and split up by Alex Falk (necessary for Netwokring functionality)
#include "Minion.h"
#include "MinionStates.h"
#include "Game.h" 
#include "Map.h"
#include "Behaviours.h"
#include "../ncltech/SphereCollisionShape.h"
#include "../ncltech/SceneManager.h"

Minion::Minion() : MinionBase() {

}

Minion::Minion(Colour c, Vector4 RGBA, Vector3 position, const string name) : MinionBase(c, RGBA, position, name) {

	detectionRadiusSQ = 15.0f * 15.0f;
	pursueRadiusSQ = 17.5f * 17.5f;
	allyHealPursueLimit = 50.0f; 

	behaviourWeight = 12;
	behaviourXZMagnitude = 20;

	ChangeState(MinionStateWander::GetInstance());

	ComputeClosestEnemyPlayer();
	ComputeClosestFriendlyPlayer();
	ComputeNewWanderPosition();
	ComputeClosestCaptureArea();

	Physics()->SetOnCollisionCallback(
		std::bind(&Minion::MinionCallbackFunction,
			this,							//Any non-placeholder param will be passed into the function each time it is called
			std::placeholders::_1,			//The placeholders correlate to the expected parameters being passed to the callback
			std::placeholders::_2
		)
	);
}

bool Minion::MinionCallbackFunction(PhysicsNode * self, PhysicsNode * collidingObject)
{
	switch (collidingObject->GetType())
	{
	case PLAYER:
		if (!dead) {
			Avatar* avatar = static_cast<Avatar*>(collidingObject->GetParent());
			if (avatar->GetColour() != this->colour) {
				avatar->ChangeLife(-(life / 5));
			}
			else avatar->ChangeLife(life / 5);
			dead = true;
		}
		return false;
	case MINION:
		if (!dead)
		{
			MinionBase* minion = static_cast<MinionBase*>(collidingObject->GetParent());
			if (minion->GetColour() != this->colour && minion->IsAlive()) {
				float tempLife = life;
				ChangeLife(-minion->GetLife());
				minion->ChangeLife(-(tempLife));
				return false;
			}
		}
		break;
	case BIG_NODE:
	case DEFAULT_PHYSICS: isGrounded = true; ChangeLife(-1); break;
	case PICKUP: return false;
	case PAINTABLE_OBJECT:
		CaptureArea * captureArea = static_cast<CaptureArea*>(collidingObject->GetParent());
		if (captureArea->GetColour() != this->colour)
		{
			dead = true;
		}
		isGrounded = true;
		break;
	}
	return true;
}

Minion::~Minion() {
	closestCaptureArea = NULL;
	closestEnemyPlayer = NULL;
	closestFriendlyPlayer = NULL;
}

void Minion::ChangeState(State<Minion>* newState)
{
	// If new state exists
	if (newState)
	{
		if (currentState) {
			previousState = currentState;
			currentState->Exit(this);
		}
		currentState = newState;
		currentState->Enter(this);
	}

}

void Minion::RevertState()
{
	//If a previous state exists
	if (previousState)
	{
		State<Minion>* temp;
		temp = previousState;
		previousState = currentState;
		currentState->Exit(this);
		currentState = temp;
		currentState->Enter(this);
	}
}

void Minion::Update(float dt)
{
	MinionBase::Update(dt);

	if (life < minLife) {
		dead = true;
	}

	if (dead)
	{
		this->SetToDestroy();
		Game::Instance()->KillMinion(this);
		((Map*)Game::Instance()->GetMap())->RemoveMinion(this);
		return;
	}

	lastPos = Physics()->GetPosition();


	size = 0.3f * (life / 50);

	ChangeSize(size);
	
	if (currentState){
		currentState->Execute(this);
	}
	if (GetIsGrounded()) 
	{
		if (minionBlackboard.GetGoToNearestCaptureZone()) 
		{
			if (GetClosestCaptureArea()) 
			{
				physicsNode->SetAngularVelocity(Vector3{ 0,0,0 });
				physicsNode->SetLinearVelocity(Vector3{ 0,0,0 });
				physicsNode->SetAcceleration(Behaviours::Seek(closestCaptureArea->Physics(), physicsNode, isGrounded, behaviourWeight, behaviourXZMagnitude));
				isGrounded = false;
			}
			
		}
		else if (minionBlackboard.GetWander()) 
		{
			physicsNode->SetAngularVelocity(Vector3{ 0,0,0 });
			physicsNode->SetLinearVelocity(Vector3{ 0,0,0 });
			physicsNode->SetAcceleration(Behaviours::Seek(wanderPosition, physicsNode->GetPosition(), physicsNode->GetLinearVelocity(), isGrounded, behaviourWeight, behaviourXZMagnitude));
			isGrounded = false;
		}
		else if (minionBlackboard.GetFleeTarget()) {

		}
		else if (minionBlackboard.GetGoToClosestAlly()) 
		{
			if (closestFriendlyPlayer)
			{
				physicsNode->SetAngularVelocity(Vector3{ 0,0,0 });
				physicsNode->SetLinearVelocity(Vector3{ 0,0,0 });
				physicsNode->SetAcceleration(Behaviours::Pursue(closestFriendlyPlayer->Physics(), physicsNode, isGrounded, behaviourWeight, behaviourXZMagnitude));
				isGrounded = false;
			}
		}
		else if (minionBlackboard.GetGoToNearestEnemy()) 
		{
			if (closestEnemyPlayer)
			{
				physicsNode->SetAngularVelocity(Vector3{ 0,0,0 });
				physicsNode->SetLinearVelocity(Vector3{ 0,0,0 });
				physicsNode->SetAcceleration(Behaviours::Pursue(closestEnemyPlayer->Physics(), physicsNode, isGrounded, behaviourWeight, behaviourXZMagnitude));
				isGrounded = false;
			}

		}
	}
	else 
	{
		physicsNode->SetAcceleration({ 0, 0, 0 });
	}


	wanderTimer += dt;
	closestPlayerTimer += dt;
	closestCaptureAreaTimer += dt;
	if (wanderTimer > 4.0f || WanderPositionInRange()) {
		ComputeNewWanderPosition();
	}
	
	if (closestPlayerTimer > 0.25f) {	
		ComputeClosestFriendlyPlayer();
		ComputeClosestEnemyPlayer();
	}
	if (closestCaptureAreaTimer > 0.75f) {
		ComputeClosestCaptureArea();
	}
}


void Minion::ComputeNewWanderPosition() {
	wanderTimer = 0.0f;
	int xDim = (int)((Map*)SceneManager::Instance()->GetCurrentScene())->GetXDimension();
	int zDim = (int)((Map*)SceneManager::Instance()->GetCurrentScene())->GetYDimension();
	float randX = (float)((rand() % (xDim*2)) - xDim);
	float randZ = (float)((rand() % (zDim*2)) - zDim);
	wanderPosition = Vector3{ randX, 2.5f, randZ };
}

void Minion::ComputeClosestEnemyPlayer() {
	closestEnemyPlayer = nullptr;
	closestPlayerTimer = 0.0f;
	float minDist = 10000.0f;
	for (int i = 0; i < 4; i++) {
		if (Game::Instance()->GetPlayer(i)) {
			float dist = (this->physicsNode->GetPosition() - Game::Instance()->GetPlayer(i)->Physics()->GetPosition()).LengthSQ();
			if (dist < minDist && this->colour != Game::Instance()->GetPlayer(i)->GetColour()) {
				closestEnemyPlayer = Game::Instance()->GetPlayer(i);
				minDist = dist;
			}
		}
	}
}

void Minion::ComputeClosestFriendlyPlayer() {
	closestFriendlyPlayer = nullptr;
	closestPlayerTimer = 0.0f;
	float minDist = 10000.0f;
	for (int i = 0; i < 4; i++) {
		if (Game::Instance()->GetPlayer(i)) {
			float dist = (this->physicsNode->GetPosition() - Game::Instance()->GetPlayer(i)->Physics()->GetPosition()).LengthSQ();
			if (dist < minDist && this->colour == Game::Instance()->GetPlayer(i)->GetColour()) {
				closestFriendlyPlayer = Game::Instance()->GetPlayer(i);
				minDist = dist;
			}
		}
	}
}

void Minion::ComputeClosestCaptureArea() {
	closestCaptureAreaTimer = 0.0f;
	float minDist = 10000.0f;

	Scene * m = SceneManager::Instance()->GetCurrentScene();
	for (GameObject * go : m->GetConstantGameObjects())
	{
		if (go->Physics())
		{
			if (go->Physics()->GetType() == PAINTABLE_OBJECT)
			{
				float dist = (this->physicsNode->GetPosition() - go->Physics()->GetPosition()).LengthSQ();
				if (dist < minDist) {
					closestCaptureArea = static_cast<CaptureArea*>(go);
				}
			}
		}
	}

	if (closestCaptureArea) {
		if (closestCaptureArea->GetColour() == this->GetColour()) {
			closestCaptureArea = NULL;
		}
	}
}

bool Minion::WanderPositionInRange() {
	if (abs(wanderPosition.x - physicsNode->GetPosition().x) < 5.f) {
		if (abs(wanderPosition.z - physicsNode->GetPosition().z) < 5.f) {
			return true;
		}
	}
	return false;
}