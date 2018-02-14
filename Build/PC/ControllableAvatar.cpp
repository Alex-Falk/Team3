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
#include <ncltech\CommonMeshes.h>
#include <nclgl\PlayerRenderNode.h>
#include <nclgl\common.h>

//I blame Microsoft...
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

ControllableAvatar::ControllableAvatar() : Avatar()
{
	lifeDrainFactor = 2500;
}

ControllableAvatar::~ControllableAvatar()
{
	lifeDrainFactor = 2500;
}

ControllableAvatar::ControllableAvatar(Vector3 pos, Colour c, uint id, float s) : Avatar(pos, c, id, s)
{
	lifeDrainFactor = 2500;
}


//Takes Player Input and move the player using force
void ControllableAvatar::ProcessAvatarInput(float dt) 
{
	Vector3 force(0,0,0);

	if (!inAir) 
	{
		float yaw = GraphicsPipeline::Instance()->GetCamera()->GetYaw();

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
			Physics()->SetAngularVelocity(velocity);
		}
	}

	if (Input::GetInput()->GetInput(JUMP) && canJump) 
	{		//Jump
		Physics()->SetLinearVelocity(Vector3(force.x /3.0f, jumpImpulse, force.z / 3.0f));
		inAir = true;
		((PlayerRenderNode*)Render()->GetChild())->SetIsInAir(true);
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

	if (Input::GetInput()->GetInput(SHOOT))
	{
		shooting = true;
	}

	if (force.x > maxForce)force.x = maxForce;
	if (force.x < -maxForce)force.x = -maxForce;
	if (force.z > maxForce)force.z = maxForce;
	if (force.z < -maxForce)force.z = -maxForce;
	Physics()->SetForce(force);

}


// Updates everything on player
void ControllableAvatar::OnAvatarUpdate(float dt) {

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


	if (life > minLife) 
	{
		life -= dt * (float)min((Physics()->GetLinearVelocity().LengthSQ()) / lifeDrainFactor, 2.0f);

		if (life < minLife)
		{
			life = minLife;
		}
	}
	
	curSize = size * (life / 100);

	ChangeSize(curSize);
	
}

