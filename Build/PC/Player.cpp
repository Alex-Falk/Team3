//Nick Bedford
//Date: 07/02/2018
//Removed unnecessary matrix multiplications
//Also added a size change based on player life in Update()
//Changes the bounding radius of the render node and physics node for frustum and broadphase culling.
//Changes the size of the raius of the collision sphere to change size physically.
//Changes the render node's child transform to change size graphically.


#include "Player.h"
#include <ncltech\SphereCollisionShape.h>
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

	playerGameObject = CommonUtils::BuildSphereObject("Player",
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
			force = Matrix3::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -10);
			force.y = 0;
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_S)) {		//Back
			if (force.z < 0)force.z /= 2;
			force = Matrix3::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, 10);
			force.y = 0;
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {		//Left
			if (force.x > 0)force.x /= 2;
			force = Matrix3::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(-10, 0, 0);
			force.y = 0;
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_D)) {		//Right
			if (force.x < 0)force.x /= 2;
			force = Matrix3::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(10, 0, 0);
			force.y = 0;
		}
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

	Input(dt);


	if (life > minLife) 
	{
		life -= dt*5;

		if (life < minLife)
		{
			life = minLife;
		}
	}
	
	curSize = size * (life / 100);

	ChangeSize(curSize);
	
}