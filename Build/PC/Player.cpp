//Nick Bedford
//Date: 08/02/2018
//Moved setting values from header to constructor 
//Stopped resetting callback every up date
//
//

#include "Player.h"
#include <ncltech\SphereCollisionShape.h>
#include <string.h>
#include "GameInput.h"

Player::Player()
{
	life = maxLife;
	timer = 0;

	playerGameObject = CommonUtils::BuildSphereObject("Player",
		Vector3(0.0f, 1.0f, 0.0f),
		1.0f,									//Radius
		true,									//Has Physics Object
		1.0f,
		true,									//Has Collision Shape
		false,									//Dragable by the user
		PLAYER,
		Vector4(0.5, 0.5, 0.5, 1.0));	//Color
	canJump = true;
	canShoot = true;

	playerGameObject->Physics()->SetElasticity(0);

	playerId = 0;
	timer = 0;
	shootCooldown = 0.0f;

	standardSpeed = 5.0f;
	speed = standardSpeed;
	boostedSpeed = standardSpeed * 20;
	
	maxForce = 30;
	
	minLife = 10;
	maxLife = 100;
	life = maxLife;

	standardJumpImpulse = 10.0f;
	jumpImpulse = standardJumpImpulse;
	boostedJumpImpulse = standardJumpImpulse * 2;
	shooting = false;
	weapon = NUM_OF_WEAPONS;
	boostactiveTime = 15.0f;
}

Player::~Player()
{
}

Player::Player(Vector3 pos, Colour c, uint id, float s)
{
	col = c;
	size = s;
	
	timer = 0;
	speed = 5;
	maxForce = 30;
	
	minLife = 10;
	maxLife = 100;
	life = maxLife;

	jumpImpulse = 10.0f;
	boostactiveTime = 15.0f;
	shootCooldown = 0.0f;

	canJump = true;
	canShoot = true;
	shooting = false;

	switch (c)
	{
	case DEFAULT:
	{
		colour = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	case GREEN:
	{
		colour = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	}
	break;
	case BLUE:
	{
		colour = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
	}
	break;
	case RED:
	{
		colour = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	break;
	case PINK
		:
	{
		colour = Vector4(1.0f, 0.41f, 0.7f, 1.0f);
	}
		break;
	default:
	{
		colour = Vector4(0.5, 0.5, 0.5, 1.0);
	}
		break;
	}

	playerGameObject = CommonUtils::BuildSphereObject("Player",
		Vector3(0.0f, 1.0f*s, 0.0f) + pos,
		1.0f * size,							//Radius
		true,									//Has Physics Object
		1.0f,
		true,									//Has Collision Shape
		false,									//Dragable by the user
		PLAYER,
		colour);								//Colour

	playerGameObject->Physics()->SetElasticity(0);

	playerGameObject->Physics()->SetOnCollisionCallback(
		std::bind(&Player::PlayerCallbackFunction,
			this,							//Any non-placeholder param will be passed into the function each time it is called
			std::placeholders::_1,			//The placeholders correlate to the expected parameters being passed to the callback
			std::placeholders::_2
		)
	);

	weapon = NUM_OF_WEAPONS;
	playerId = id;

}

bool Player::PlayerCallbackFunction(PhysicsNode* self, PhysicsNode* collidingObject) {
	
	if (collidingObject->getName() != "Pickup")
	{
		canJump = true;
		inAir = false;
	}

	return true;
}

//Takes Player Input and move the player using force
void Player::Input(float dt) {

	float yaw = GraphicsPipeline::Instance()->GetCamera()->GetYaw();
	float pitch = GraphicsPipeline::Instance()->GetCamera()->GetPitch();
	Vector3 force(0,0,0);

	if (!inAir) {
		if (Input::GetInput()->GetInput(FORWARD)) { 		//Front
			force =  Matrix3::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -10) * speed;
			velocity += Matrix3::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(-2, 0, 0) *dt* speed;
		}
		if (Input::GetInput()->GetInput(BACKWARD)) {		//Back
			force = Matrix3::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, 10) * speed;
			velocity += Matrix3::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(2, 0, 0) * dt * speed;
		}
		if (Input::GetInput()->GetInput(LEFT)) {		//Left
			force = Matrix3::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(-10, 0, 0) * speed;
			velocity += Matrix3::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, 2)*dt* speed;
		}
		if (Input::GetInput()->GetInput(RIGHT)) {		//Right
			force = Matrix3::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(10, 0, 0) * speed;
			velocity += Matrix3::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -2)*dt* speed;
		}
		force.y = 0;
		if (force != Vector3(0, 0, 0)) {
			playerGameObject->Physics()->SetAngularVelocity(velocity);
		}
	}

	//if (Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE) && canJump) {		//Jump
	if (Input::GetInput()->GetInput(JUMP) && canJump) 
	{		//Jump
		playerGameObject->Physics()->SetLinearVelocity(Vector3(force.x /3.0f, jumpImpulse, force.z / 3.0f));
		inAir = true;
	}
	canJump = false;
	

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

	if (force.x > maxForce)force.x = maxForce;
	if (force.x < -maxForce)force.x = -maxForce;
	if (force.z > maxForce)force.z = maxForce;
	if (force.z < -maxForce)force.z = -maxForce;
	playerGameObject->Physics()->SetForce(force);

}

void Player::ChangeSize(float newSize)
{
	playerGameObject->Render()->GetChild()->SetBoundingRadius(newSize);
	playerGameObject->Render()->SetBoundingRadius(newSize);
	playerGameObject->Physics()->SetBoundingRadius(newSize);
	((SphereCollisionShape*)playerGameObject->Physics()->GetCollisionShape())->SetRadius(newSize);

	playerGameObject->Render()->GetChild()->SetTransform(Matrix4::Scale(Vector3(newSize, newSize, newSize)));
}

// Updates everything on player
void Player::OnPlayerUpdate(float dt) {

	shooting = false;

	timer += dt;

	Input(dt);
	
	UpdatePickUp(dt);

	if (weapon != NUM_OF_WEAPONS) {
		if (Input::GetInput()->GetInput(SHOOT) && canShoot) {
			ManageWeapons(weapon);
		}
	}

	
	curSize = size * (life / 100);

	ChangeSize(curSize);
	
}


void Player::PickedPickUp(PickupType pickType) {

	switch (pickType)
	{
	case SPEED_BOOST:
		speed = boostedSpeed;
		speedBoost = true;
		speedTimer = boostactiveTime;
		break;
	case JUMP_BOOST:
		jumpImpulse = boostedJumpImpulse;
		jumpBoost = true;
		jumpBoostTimer = boostactiveTime;
		break;
	case WEAPON: {

	}
		break;
	default:
		break;
	}


}

void Player::UpdatePickUp(float dt) 
{
	if (speedBoost)
	{
		speedTimer -= dt;
		
		if (speedTimer < 0)
		{
			speed = standardSpeed;
			speedBoost = false;
		}
	}

	if (jumpBoost)
	{
		jumpBoostTimer -= dt;

		if (jumpBoostTimer < 0)
		{
			jumpImpulse = standardSpeed;
			jumpBoost = false;
		}
	}

	if (!canShoot) {
		shootCooldown -= dt;
		if (shootCooldown <= 0) {
			canShoot = true;
		}
	}
}

void Player::ManageWeapons(WeaponType wt) {
	
	if (Input::GetInput()->GetInput(SHOOT)) {
		shooting = true;
		switch (wt)
		{
		case PAINT_SPRAY:
			NCLDebug::Log("Spray splash");
			ammo = Weapons::ShootPaintSpray(this->GetPosition(), curSize, colour);
			shootCooldown = 3.0f;
			canShoot = false;
			break;
		case PAINT_PISTOL:
			NCLDebug::Log("Pistol piou piou");
			ammo = Weapons::ShootPistol(this->GetPosition(), curSize, colour);
			break;
		case AUTO_PAINT_LAUNCHER:
			ammo = Weapons::ShootPistol(this->GetPosition(), curSize, colour);
			shootCooldown = 0.15f;
			canShoot = false;
			break;
		case PAINT_ROCKET:
			NCLDebug::Log("Rocket piou piou");
			ammo = Weapons::ShootRocket(this->GetPosition(), curSize, colour);
			break;
		default:
			break;
		}
	}
}