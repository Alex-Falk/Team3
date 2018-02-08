#include "Player.h"
#include <string.h>

Player::Player() : GameObject("Player")
{
	life = maxLife;
	timer = 0;

	playerGameObject = CommonUtils::BuildSphereObject("Player1",
		Vector3(0.0f, 1.0f, 0.0f),
		1.0f,									//Radius
		true,									//Has Physics Object
		1.0f,
		true,									//Has Collision Shape
		false,									//Dragable by the user
		Vector4(0.5, 0.5, 0.5, 1.0));	//Color
	canJump = true;
	playerGameObject->Physics()->SetElasticity(0);
	playerGameObject->Physics()->SetFriction(50);
}

Player::~Player()
{
}

Player::Player(Vector3 pos, Colour c, float s) : GameObject("Player")
{
	colour = c;
	size = s;
	life = maxLife;
	timer = 0;

	Vector4 Colour;
	canJump = true;

	switch (c)
	{
	case DEFAULT:
	{
		Colour = Vector4(1.0, 1.0, 1.0, 1.0);
	}
	case GREEN:
	{
		Colour = Vector4(0.0, 1.0, 0.0, 1.0);
	}
	break;
	case BLUE:
	{
		Colour = Vector4(0.0, 0.0, 1.0, 1.0);
	}
	break;
	case RED:
	{
		Colour = Vector4(1.0, 0.0, 0.0, 1.0);
	}
	break;
	case PINK
		:
	{
		Colour = Vector4(1.0, 0.41, 0.7, 1.0);
	}
		break;
	default:
	{
		Colour = Vector4(0.5, 0.5, 0.5, 1.0);
	}
		break;
	}

	playerGameObject = CommonUtils::BuildSphereObject("Player" + to_string(c),
		Vector3(0.0f, 1.0f*s, 0.0f) + pos,
		1.0f * size,							//Radius
		true,									//Has Physics Object
		1.0f,
		true,									//Has Collision Shape
		false,									//Dragable by the user
		Colour);								//Colour

	playerGameObject->Physics()->SetElasticity(0);
	playerGameObject->Physics()->SetFriction(50);

}

bool Player::canJump; // Resets Players ability to jump
bool Player::inAir;
bool Player::SetCanJump(PhysicsNode* self, PhysicsNode* collidingObject) {
	canJump = true;
	inAir = false;
	return true;
}

//Takes Player Input and move the player using force
void Player::Input(float dt) {

	playerGameObject->Physics()->SetOnCollisionCallback(SetCanJump);

	float yaw = GraphicsPipeline::Instance()->GetCamera()->GetYaw();
	float pitch = GraphicsPipeline::Instance()->GetCamera()->GetPitch();

	if (!inAir) {
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_W)) { 		//Front
			if (force.z > 0)force.z /= 2;
			force =  Matrix3::Rotation(yaw, Vector3(0, 10, 0)) * Vector3(0, 0, -10) * speed;
			velocity += Matrix3::Rotation(yaw, Vector3(0, 2, 0)) * Vector3(-2, 0, 0) *dt* speed;
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_S)) {		//Back
			if (force.z < 0)force.z /= 2;
			force = Matrix3::Rotation(yaw, Vector3(0, 10, 0)) * Vector3(0, 0, 10) * speed;
			velocity += Matrix3::Rotation(yaw, Vector3(0, 2, 0)) * Vector3(2, 0, 0)*dt* speed;
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {		//Left
			if (force.x > 0)force.x /= 2;
			force = Matrix3::Rotation(yaw, Vector3(0, 10, 0)) * Vector3(-10, 0, 0) * speed;
			velocity += Matrix3::Rotation(yaw, Vector3(0, 2, 0)) * Vector3(0, 0, 2)*dt* speed;
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_D)) {		//Right
			if (force.x < 0)force.x /= 2;
			force = Matrix3::Rotation(yaw, Vector3(0, 10, 0)) * Vector3(10, 0, 0) * speed;
			velocity += Matrix3::Rotation(yaw, Vector3(0, 2, 0)) * Vector3(0, 0, -2)*dt* speed;
		}
		force.y = 0;
		if (force != Vector3(0, 0, 0)) {
			playerGameObject->Physics()->SetAngularVelocity(velocity);
		}
	}

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_1)) {
		PickedPickUp(SPEED_BOOST);
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_2)) {
		PickedPickUp(JUMP_BOOST);
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE) && canJump) {		//Jump
 		playerGameObject->Physics()->SetLinearVelocity(Vector3(force.x /1.5, jumpImpulse, force.z / 1.5));
		inAir = true;
	}
	canJump = false;
	
	if (force.x > maxForce)force.x = maxForce;
	if (force.x < -maxForce)force.x = -maxForce;
	if (force.z > maxForce)force.z = maxForce;
	if (force.z < -maxForce)force.z = -maxForce;
	playerGameObject->Physics()->SetForce(force);
	
	force = Vector3(0, 0, 0);
}

// Updates everything on player
void Player::OnPlayerUpdate(float dt) {

	timer += dt;

	Input(dt);
	
	UpdatePickUp(dt);

	if (life > minLife) {
		life -= 0.01;
	}
	
	curSize = size * (life / 100);

	cout << curSize;

	playerGameObject->Render()->GetChild()->SetBoundingRadius(curSize);
	playerGameObject->RegisterPhysicsToRenderTransformCallback();
}


void Player::PickedPickUp(PickupType pickType) {

	switch (pickType)
	{
	case SPEED_BOOST:
		speed = speed * 20;
		speedBoost = true;
		speedTimer = boostactiveTime;
		break;
	case JUMP_BOOST:
		jumpImpulse = jumpImpulse * 2;
		jumpBoost = true;
		jumpBoostTimer = boostactiveTime;
		break;
	case WEAPON:
		weapon = true;
		break;
	default:
		break;
	}


}

void Player::UpdatePickUp(float dt) {
	// Timers Countdown
	speedTimer -= dt;
	jumpBoostTimer -= dt;

	if (speedTimer < 0 && speedBoost) {
		speed /= 5;
		speedBoost = false;
	}
	if (jumpBoostTimer < 0 && jumpBoost) {
		jumpImpulse /= 2;
		jumpBoost = false;
	}


}