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

ControllableAvatar::ControllableAvatar()
{
	life = maxLife;

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

ControllableAvatar::~ControllableAvatar()
{
}

ControllableAvatar::ControllableAvatar(Vector3 pos, Colour c, uint id, float s)
{
	col = c;
	size = s;
	
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
	case START_COLOUR:
	{
		colour = DEFAULT_COLOUR;
	}
	case GREEN:
	{
		colour = GREEN_COLOUR;
	}
	break;
	case BLUE:
	{
		colour = BLUE_COLOUR;
	}
	break;
	case RED:
	{
		colour = RED_COLOUR;
	}
	break;
	case PINK
		:
	{
		colour = PINK_COLOUR;
	}
		break;
	default:
	{
		colour = DEFAULT_COLOUR;
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
		std::bind(&Avatar::PlayerCallbackFunction,
			this,							//Any non-placeholder param will be passed into the function each time it is called
			std::placeholders::_1,			//The placeholders correlate to the expected parameters being passed to the callback
			std::placeholders::_2
		)
	);

	weapon = NUM_OF_WEAPONS;
	playerId = id;

}


//Takes Player Input and move the player using force
void ControllableAvatar::Input(float dt) {

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

	if (Input::GetInput()->GetInput(SHOOT))
	{

	}

	if (force.x > maxForce)force.x = maxForce;
	if (force.x < -maxForce)force.x = -maxForce;
	if (force.z > maxForce)force.z = maxForce;
	if (force.z < -maxForce)force.z = -maxForce;
	playerGameObject->Physics()->SetForce(force);

}


// Updates everything on player
void ControllableAvatar::OnAvatarUpdate(float dt) {

	shooting = false;

	Input(dt);
	
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
		life -= dt * (float)min((playerGameObject->Physics()->GetLinearVelocity().LengthSQ()) / 2500.0f, 2.0f);

		if (life < minLife)
		{
			life = minLife;
		}
	}
	
	curSize = size * (life / 100);

	ChangeSize(curSize);
	
}

