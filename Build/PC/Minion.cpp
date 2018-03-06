//Michael Davis 16/02/2018
#include "Minion.h"
#include "MinionStates.h"
#include "Game.h" 
#include "Map.h"
#include "Behaviours.h"

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
	//float lifeLoss = (Physics()->GetPosition() - lastPos).LengthSQ();
	//life -= lifeLoss / (dt * 10);


	lastPos = Physics()->GetPosition();


	size = 0.3f * (life / 50);

	ChangeSize(size);
	
	if (currentState){
		currentState->Execute(this);
	}
	if (life < minLife || dead) {
		dead = true;
		//destroy = true;
	}
	else if (GetIsGrounded()) 
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
			physicsNode->SetAngularVelocity(Vector3{ 0,0,0 });
			physicsNode->SetLinearVelocity(Vector3{ 0,0,0 });
			physicsNode->SetAcceleration(Behaviours::Pursue(closestFriendlyPlayer->Physics(), physicsNode, isGrounded, behaviourWeight, behaviourXZMagnitude));
			isGrounded = false;
		}
		else if (minionBlackboard.GetGoToNearestEnemy()) 
		{
			physicsNode->SetAngularVelocity(Vector3{ 0,0,0 });
			physicsNode->SetLinearVelocity(Vector3{ 0,0,0 });
			physicsNode->SetAcceleration(Behaviours::Pursue(closestEnemyPlayer->Physics(), physicsNode, isGrounded, behaviourWeight, behaviourXZMagnitude));
			isGrounded = false;
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
	float randX = rand() % 80 + -40;
	float randZ = rand() % 80 + -40;
	wanderPosition = Vector3{ randX, 2.5f, randZ };
}

void Minion::ComputeClosestEnemyPlayer() {
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
	for (int i = 0; i < ((Map*)SceneManager::Instance()->GetCurrentScene())->GetCaptureAreaVector().size(); i++) {
		if (((Map*)SceneManager::Instance()->GetCurrentScene())->GetCaptureArea(i)->GetColour() != this->colour) {
			float dist = (this->physicsNode->GetPosition() - ((Map*)SceneManager::Instance()->GetCurrentScene())->GetCaptureArea(i)->Physics()->GetPosition()).LengthSQ();
			if (dist < minDist) {
				closestCaptureArea = ((Map*)SceneManager::Instance()->GetCurrentScene())->GetCaptureArea(i);
				minDist = dist;
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