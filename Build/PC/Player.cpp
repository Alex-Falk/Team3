#include "Player.h"
#include <string.h>

Player::Player()
{
	Pl = CommonUtils::BuildSphereObject("Player",
		Vector3(0.0f, 1.0f, 0.0f),
		1.0f,									//Radius
		true,									//Has Physics Object
		1.0f,
		true,									//Has Collision Shape
		true,									//Dragable by the user
		Vector4(0.5, 0.5, 0.5, 1.0));	//Color
}

Player::~Player()
{
}

Player::Player(Vector3 pos, Colour c, float s)
{
	Vector4 colour;

	switch (c)
	{
	case DEFAULT: 
	{
		colour = Vector4(0.5, 0.5, 0.5, 1.0);
	}
	case GREEN:
	{
		colour = Vector4(0.0, 1.0, 0.0, 1.0);
	}
	break;
	case BLUE:
	{
		colour = Vector4(0.0, 0.0, 1.0, 1.0);
	}
	break;
	case RED:
	{
		colour = Vector4(1.0, 0.0, 0.0, 1.0);
	}
	break;
	case PINK:
	{
		colour = Vector4(1.0, 2.0, 1.0, 1.0);
	}
	break;
	default:
	{
		colour = Vector4(0.5, 0.5, 0.5, 1.0);
	}
	break;
	}

	Pl = CommonUtils::BuildSphereObject("Player",
		Vector3(0.0f, 1.0f*s, 0.0f) + pos,
		1.0f * s,								//Radius
		true,									//Has Physics Object
		1.0f / s,
		true,									//Has Collision Shape
		true,									//Dragable by the user
		colour);								//Colour

}

void Player::Input(float dt) {

	Jumptime += dt;
	if (Jumptime > 5)
	{
		CanJump = true;
		Jumptime = 0;
	}

	float yaw = GraphicsPipeline::Instance()->GetCamera()->GetYaw();
	float pitch = GraphicsPipeline::Instance()->GetCamera()->GetPitch();

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_SPACE) && CanJump) {
		Pl->Physics()->SetLinearVelocity(Vector3(force.x / 2, 10, force.z / 2));
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_W)) {
		force += Matrix3::Rotation(0, Vector3(3, 0, 0)) * Matrix3::Rotation(yaw, Vector3(0, 3, 0)) * Vector3(0, 0, -3) * 5 * dt;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_S)) {
		force += Matrix3::Rotation(0, Vector3(-3, 0, 0)) * Matrix3::Rotation(yaw, Vector3(0, -3, 0)) * Vector3(0, 0, 3) * 5 * dt;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {
		force += Matrix3::Rotation(0, Vector3(0, 0, -5)) * Matrix3::Rotation(yaw, Vector3(0, 0, 3)) * Vector3(-3, 0, 0) * 5 * dt;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_D)) {
		force += Matrix3::Rotation(0, Vector3(0, 0, 5)) * Matrix3::Rotation(yaw, Vector3(0, 0, 3)) * Vector3(3, 0, 0) * 5 * dt;
	}
	if (force.x > 5)force.x = 5;
	if (force.x < -5)force.x = -5;
	if (force.z > 5)force.z = 5;
	if (force.z < -5)force.z = -5;
	Pl->Physics()->SetForce(force);
}