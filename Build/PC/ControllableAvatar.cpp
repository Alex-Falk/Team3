//					,.ood888888888888boo.,
//              .od888P^""            ""^Y888bo.
//          .od8P''   ..oood88888888booo.    ``Y8bo.
//       .odP'"  .ood8888888888888888888888boo.  "`Ybo.
//     .d8'   od8'd888888888f`8888't888888888b`8bo   `Yb.
//    d8'  od8^   8888888888[  `'  ]8888888888   ^8bo  `8b
//  .8P  d88'     8888888888P      Y8888888888     `88b  Y8.
// d8' .d8'       `Y88888888'      `88888888P'       `8b. `8b
//.8P .88P            """"            """"            Y88. Y8.
//88  888                 Nick Bedford                 888  88
//88  888           ControllableAvatar Class           888  88
//88  888.        ..       13/02/2018       ..        .888  88
//`8b `88b,     d8888b.od8bo.      .od8bo.d8888b     ,d88' d8'
// Y8. `Y88.    8888888888888b    d8888888888888    .88P' .8P
//  `8b  Y88b.  `88888888888888  88888888888888'  .d88P  d8'
//    Y8.  ^Y88bod8888888888888..8888888888888bod88P^  .8P
//     `Y8.   ^Y888888888888888LS888888888888888P^   .8P'
//       `^Yb.,  `^^Y8888888888888888888888P^^'  ,.dP^'
//          `^Y8b..   ``^^^Y88888888P^^^'    ..d8P^'
//              `^Y888bo.,            ,.od888P^'
//                   "`^^Y888888888888P^^'"      

#include "ControllableAvatar.h"
#include <ncltech\SphereCollisionShape.h>
#include <string.h>
#include "GameInput.h"
#include "Game.h"
#include <nclgl\PlayerRenderNode.h>
#include "Pickup.h"
#include "WeaponPickup.h"

//I blame Microsoft...
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

ControllableAvatar::ControllableAvatar() : Avatar()
{
	lifeDrainFactor = 1000;
	curMove = NO_MOVE;
	previousMove = NO_MOVE;
}

ControllableAvatar::~ControllableAvatar()
{
}

ControllableAvatar::ControllableAvatar(Vector3 pos, Colour c, uint id, float s) : Avatar(pos, c, id, s)
{
	lifeDrainFactor = 1000;
	curMove = NO_MOVE;
	previousMove = NO_MOVE;
}


//Takes Player Input and move the player using force
void ControllableAvatar::ProcessAvatarInput(float dt)
{
	Movement move = NO_MOVE;
	float yaw = GraphicsPipeline::Instance()->GetCamera()->GetYaw();

	// Movement 
	if (Input::Instance()->GetInput(FORWARD)) { 		//Front
		move = MOVE_FORWARD;
	}
	if (Input::Instance()->GetInput(BACKWARD)) {		//Back
		move = MOVE_BACKWARD;
	}
	if (Input::Instance()->GetInput(LEFT)) {		//Left
		move = MOVE_LEFT;
	}
	if (Input::Instance()->GetInput(RIGHT)) {		//Right
		move = MOVE_RIGHT;
	}
	if (Input::Instance()->GetInput(JUMP)) {		//Jump
		move = MOVE_JUMP;
	}

	
	MovementState(move, yaw, dt);




	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_1)) {
		NCLDebug::Log("Pistol Activated");
		weapon = PAINT_PISTOL;
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_2)) {
		NCLDebug::Log("Rocket Activated");
		weapon = PAINT_ROCKET;
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_3)) {
		NCLDebug::Log("Spray Activated");
		weapon = PAINT_SPRAY;
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_4)) {
		NCLDebug::Log("Auto Activated");
		weapon = AUTO_PAINT_LAUNCHER;
	}

	if (Input::Instance()->GetInput(SHOOT))
	{
		shooting = true;
	}
}


// Updates everything on player
void ControllableAvatar::Update(float dt) {

	shooting = false;

	ProcessAvatarInput(dt);
	
	UpdatePickUp(dt);

	if (weapon != NUM_OF_WEAPONS) 
	{
		if (canShoot) 
		{
			ManageWeapons();
		}
	}


	if (targetLife > minLife)
	{
		targetLife -= dt * (float)min((Physics()->GetLinearVelocity().LengthSQ()) / lifeDrainFactor, 2.0f);

		if (targetLife < minLife)
		{
			targetLife = minLife;
		}
	}
	
	LerpLife(dt);

	curSize = size * (life / 100);

	ChangeSize(curSize);

	if (collisionTimerActive)
	{
		collisionTimer -= dt;
		if (collisionTimer <= 0)
		{
			collisionTimerActive = false;
			inAir = true;
			((PlayerRenderNode*)Render()->GetChild())->SetIsInAir(true);
		}
	}

}

bool ControllableAvatar::PlayerCallbackFunction(PhysicsNode* self, PhysicsNode* collidingObject) {
	if (collidingObject->GetType() == DEFAULT_PHYSICS || collidingObject->GetType() == BIG_NODE || (collidingObject->GetType() == PAINTABLE_OBJECT))
	{
		canJump = true;
		collisionTimerActive = true;
		collisionTimer = timeUntilInAir;
		inAir = false;
		if (collidingObject->GetType() == BIG_NODE) {
			((PlayerRenderNode*)Render()->GetChild())->SetIsInAir(false);
		}
	}
	else if (collidingObject->GetType() == PICKUP)
	{
		Pickup * p = (Pickup*)(collidingObject->GetParent());
		if (p->GetActive())
		{
			activePickUp = p->GetPickupType();
			if (activePickUp == WEAPON)
			{
				weapon = ((WeaponPickup*)p)->GetWeaponType();
			}

			if (Game::Instance()->getUserID() == 0)
			{
				PickUpBuffActivated(activePickUp);
				//phil 06/03/2018 so the paint pools don't dissapear on minimap
				//if(p->GetPickupType() != PickupType::PAINTPOOL)
					p->SetActive(false);
			}

			Game::Instance()->ClaimPickup(p);

		}

		return false;
	}
	return true;
}

void ControllableAvatar::MovementState(Movement inputDir, float yaw, float dt)
{
	Vector3 force;
	moveTimer += dt;
	switch (inputDir)
	{
	case NO_MOVE: {
		force = Vector3(0, 0, 0);
		break;
	}
	case MOVE_FORWARD:
		force = Matrix3::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * speed;
		dirRotation = Matrix3::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(-1, 0, 0) * (standarSpinSpeed);
		curMove = MOVE_FORWARD;
		break;

	case MOVE_BACKWARD:
		force = Matrix3::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, 1) * speed;
		dirRotation = Matrix3::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(1, 0, 0) * (standarSpinSpeed);
		curMove = MOVE_BACKWARD;
		break;

	case MOVE_LEFT:
		force = Matrix3::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(-1, 0, 0) * speed;
		dirRotation = Matrix3::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, 1) * (standarSpinSpeed);
		curMove = MOVE_LEFT;
		break;

	case MOVE_RIGHT:
		force = Matrix3::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(1, 0, 0) * speed;
		dirRotation = Matrix3::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * (standarSpinSpeed);
		curMove = MOVE_RIGHT;
		break;
	case MOVE_JUMP: {
		curMove = MOVE_JUMP;
		if (canJump) {
			Vector3 vel = Physics()->GetLinearVelocity();
			Physics()->SetLinearVelocity(Vector3(vel.x*.6f, jumpImpulse, vel.z*.6f));
			inAir = true;
			((PlayerRenderNode*)Render()->GetChild())->SetIsInAir(true);
			canJump = false;
		}
		break;
	}
	}
	force.y = 0;

	// Setting Angular Velocity
	int basicSpinSpeed = 55; //Change this number to change the spin speed
	if (moveTimer > 2.f) { rollSpeed -= 1; }
	if (rollSpeed < 0) { rollSpeed = 0; }

	if (curMove != previousMove)
	{
		Physics()->SetAngularVelocity(((dirRotation * 3.0f) / (2.0f * life * PI)) * (float)basicSpinSpeed);
		previousMove = curMove;
		moveTimer = 0;
	}
	else if (curMove == inputDir && inputDir != MOVE_JUMP) {
		rollSpeed += 1;
		if (inAir) {
			if (rollSpeed > maxRotationSpeed) { rollSpeed = maxRotationSpeed; }
		}
		else {
			if (rollSpeed > 20) { rollSpeed = 20; }
		}
		Physics()->SetAngularVelocity(((dirRotation * 3) / (2 * life * PI)) * (basicSpinSpeed + rollSpeed));
	}

	// Setting MoveMent
	if (inAir)
	{
		force = Vector3(0, 0, 0);
	}

	Physics()->SetForce(force);

	if (Physics()->GetLinearVelocity().Length() > maxVel)
	{
		Physics()->SetLinearVelocity(Vector3(Physics()->GetLinearVelocity()).Normalise() * maxVel);
	}
}