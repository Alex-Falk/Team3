#include "Player.h"
#include <string.h>

Player::Player() : GameObject("Player")
{
	life = maxLife;

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
}

Player::~Player()
{
}

Player::Player(Vector3 pos, Colour c, float s) : GameObject("Player")
{
	colour = c;
	size = s;
	life = maxLife;

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
}

bool Player::canJump; // Resets Players ability to jump
bool Player::SetCanJump(PhysicsNode* self, PhysicsNode* collidingObject) {
	canJump = true;
	return true;
}

//Takes Player Input and move the player using force
void Player::Input(float dt) {

	playerGameObject->Physics()->SetOnCollisionCallback(SetCanJump);


	float yaw = GraphicsPipeline::Instance()->GetCamera()->GetYaw();
	float pitch = GraphicsPipeline::Instance()->GetCamera()->GetPitch();


	if (Window::GetKeyboard()->KeyDown(KEYBOARD_W)) { 		//Front
		if (force.z > 0)force.z /= 2;
	//	force.y = 0;
		force = Matrix3::Rotation(0, Vector3(10, 0, 0)) * Matrix3::Rotation(yaw, Vector3(0, 0, 0)) * Vector3(0, 0, -3) * speed;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_S)) {		//Back
		if (force.z < 0)force.z /= 2;
	//	force.y = 0;
		force = Matrix3::Rotation(0, Vector3(-10, 0, 0)) * Matrix3::Rotation(yaw, Vector3(0, 0, 0)) * Vector3(0, 0, 3) * speed;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {		//Left
		if (force.x > 0)force.x /= 2;
	//	force.y = 0;
		force = Matrix3::Rotation(0, Vector3(0, 0, -10)) * Matrix3::Rotation(yaw, Vector3(0, 0, 3)) * Vector3(-3, 0, 0) * speed;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_D)) {		//Right
		if (force.x < 0)force.x /= 2;
	//	force.y = 0;
		force = Matrix3::Rotation(0, Vector3(0, 0, 10)) * Matrix3::Rotation(yaw, Vector3(0, 0, 3)) * Vector3(3, 0, 0) * speed;
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_SPACE) && canJump) {		//Jump
		playerGameObject->Physics()->SetLinearVelocity(Vector3(force.x / 2, jumpImpulse, force.z / 2));
		canJump = false;
	}
	


	if (force.x > maxForce)force.x = maxForce;
	if (force.x < -maxForce)force.x = -maxForce;
	if (force.z > maxForce)force.z = maxForce;
	if (force.z < -maxForce)force.z = -maxForce;
	playerGameObject->Physics()->SetForce(force);
	force = Vector3(0, 0, 0);
}

// Updates everything on player
void Player::OnPlayerUpdate(float dt) {

	Input(dt);


	if (life > minLife) {
		life -= 0.01;
	}
	
	curSize = size * (life / 100);

	cout << curSize;

	playerGameObject->Render()->GetChild()->SetBoundingRadius(curSize);
	playerGameObject->RegisterPhysicsToRenderTransformCallback();
}