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
//88  888                 Avatar Class                 888  88
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
// Nikos Fragkas 05/02/2018
// Extended by Alex Falk



#include <ncltech\CommonMeshes.h>
#include <nclgl\PlayerRenderNode.h>
#include <ncltech\SphereCollisionShape.h>
#include <string.h>
#include <ncltech\SceneManager.h>
#include "Game.h"
#include "Projectile.h"
#include "Avatar.h"
#include "AudioSystem.h"


Avatar::Avatar()
{
	Vector3 pos = Vector3(0.0f, 1.0f, 0.0f);
	Colour c = START_COLOUR;
	uint id = 0;
	float s = 1.0f;
	Avatar(pos, c, id, s);
}

Avatar::~Avatar()
{
}

Avatar::Avatar(Vector3 pos, Colour c, uint id, float s)
{
	col = c;
	size = s;

	standardSpeed = 15.0f;
	speed = standardSpeed;
	boostedSpeed = standardSpeed * 2;

	maxRotationSpeed = 20;

	maxVel = 40.0f;

	minLife = 20;
	maxLife = 100;
	life = maxLife/2;
	targetLife = maxLife / 2;
	moveTimer = 0.0f;
	rollSpeed = 0.2f;
	

	jumpImpulse = 8.0f;
	boostactiveTime = 15.0f;
	shootCooldown = 0.0f;

	standardJumpImpulse = 8.0f;
	jumpImpulse = standardJumpImpulse;
	boostedJumpImpulse = standardJumpImpulse * 2;

	canJump = false;
	inAir = true;
	canShoot = true;
	shooting = false;

	collisionTimerActive = false;
	collisionTimer = 0.0f;
	timeUntilInAir = 0.1f;

	weapon = NUM_OF_WEAPONS;
	string name;
	switch (c)
	{
		case START_COLOUR:	colour = DEFAULT_COLOUR;	name = "DEFAULT_PLAYER";	break;
		case GREEN:			colour = GREEN_COLOUR;		name = "GREEN_PLAYER";		break;
		case BLUE:			colour = BLUE_COLOUR;		name = "BLUE_PLAYER";		break;
		case RED:			colour = RED_COLOUR;		name = "RED_PLAYER";		break;
		case PINK:			colour = PINK_COLOUR;		name = "PINK_PLAYER";		break;
		default:			colour = DEFAULT_COLOUR;	name = "DEFAULT_PLAYER";	break;
	}

	RenderNode* rnode = new RenderNode();
	float radius = s;
	RenderNode* dummy = new PlayerRenderNode(CommonMeshes::Sphere(), name, colour);
	dummy->SetTransform(Matrix4::Scale(Vector3(radius, radius, radius)));

	dummy->SetMaterial(GraphicsPipeline::Instance()->GetAllMaterials()[MATERIALTYPE::Forward_Lighting]);
	dummy->SetSmoothness(0.7f);

	rnode->AddChild(dummy);

	rnode->SetTransform(Matrix4::Translation(pos));
	rnode->SetBoundingRadius(radius);

	PhysicsNode* pnode = NULL;
	pnode = new PhysicsNode();
	pnode->SetPosition(pos);
	pnode->SetInverseMass(1.0f);
	pnode->SetBoundingRadius(s);
	pnode->SetType(PLAYER);
	CollisionShape* pColshape = new SphereCollisionShape(s);
	pnode->SetCollisionShape(pColshape);
	pnode->SetInverseInertia(pColshape->BuildInverseInertia(1.0f));

	this->friendlyName = "Player";
	this->renderNode = rnode;
	this->physicsNode = pnode;
	RegisterPhysicsToRenderTransformCallback();
	SetPhysics(pnode);
	pnode->SetName("Player");

	Physics()->SetElasticity(0);

	Physics()->SetOnCollisionCallback(
		std::bind(&Avatar::PlayerCallbackFunction,
			this,							//Any non-placeholder param will be passed into the function each time it is called
			std::placeholders::_1,			//The placeholders correlate to the expected parameters being passed to the callback
			std::placeholders::_2
		)
	);

	playerId = id;
	((PlayerRenderNode*)dummy)->SetIsInAir(true);
	inAir = true;
}

bool Avatar::PlayerCallbackFunction(PhysicsNode* self, PhysicsNode* collidingObject) {
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
	return true;
}


void Avatar::ChangeSize(float newSize)
{
	Render()->GetChild()->SetBoundingRadius(newSize);
	Render()->SetBoundingRadius(newSize);
	Physics()->SetBoundingRadius(newSize);
	Physics()->SetInverseMass(0.5f/newSize);
	standardSpeed = 25.0f*newSize;
	boostedSpeed = 50.0f*newSize;
	standarSpinSpeed = 40 * newSize;

	((SphereCollisionShape*)Physics()->GetCollisionShape())->SetRadius(newSize);
	
	Render()->GetChild()->SetTransform(Matrix4::Scale(Vector3(newSize, newSize, newSize)));
}

// Updates everything on player
void Avatar::Update(float dt) {

	shooting = false;
	
	ManageWeapons();
	
	UpdatePickUp(dt);

	// Hard Reset if player falls off the map
	if (this->Physics()->GetPosition().y <= -10.0f)
	{
		this->Physics()->SetPosition(((Map*)Game::Instance()->GetMap())->GetSpawnPos(this->col));
		this->Physics()->SetForce(Vector3(0, 0, 0));
		this->Physics()->SetLinearVelocity(Vector3(0, 0, 0));
		this->Physics()->SetAngularVelocity(Vector3(0, 0, 0));
	}

	if (targetLife < minLife)
	{
		targetLife = minLife;
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

void Avatar::LerpLife(float dt)
{
	float lifeDif = ((targetLife - life) * dt);
	
	if (lifeDif > 0.5f)
	{
		life = life + lifeDif * 2;
	}
	else if (lifeDif > 0.001f)
	{
		life = life + lifeDif;
	}
	else
	{
		life = life + lifeDif;
	}

	if (life > maxLife)
	{
		life = maxLife;
	}
	else if (life < minLife)
	{
		life = minLife;
	}
}

void Avatar::PickUpBuffActivated()
{
	PickUpBuffActivated(activePickUp);
}

void Avatar::PickUpBuffActivated(PickupType pickType) {

	switch (pickType)
	{
		case SPEED_BOOST:	speed = boostedSpeed;					speedBoost = true;				speedTimer = boostactiveTime;		break;
		case JUMP_BOOST:	jumpImpulse = boostedJumpImpulse;		jumpBoost = true;				jumpBoostTimer = boostactiveTime;	break;
		case WEAPON:		weaponActive = true;					weaponTimer = boostactiveTime;										break;
	}
}

void Avatar::UpdatePickUp(float dt)
{
	if (speedBoost)
	{
		speedTimer -= dt;
		if (speedTimer <= 0)
		{
			speed = standardSpeed;
			speedBoost = false;
		}
	}

	if (jumpBoost)
	{
		jumpBoostTimer -= dt;
		if (jumpBoostTimer <= 0)
		{
			jumpImpulse = standardJumpImpulse;
			jumpBoost = false;
		}
	}
	
	if (weaponActive)
	{
		weaponTimer -= dt;
		if (weaponTimer <= 0)
		{
			weaponActive = false;
		}
	}

	if (weaponActive && !canShoot) 
	{
		shootCooldown -= dt;
		if (shootCooldown <= 0) 
		{
			canShoot = true;
		}
	}
}

void Avatar::Spray()
{
	int randPitch;
	int randYaw;
	Vector3 direction;

	if (targetLife > minLife + 5.0f)
	{
		int randPitch;
		int randYaw;
		int randSpeed;
		Vector3 direction;

		targetLife -= 5.0f;

		for (int i = 0; i < 40; ++i)
		{
			randPitch = (rand() % 20) - 10;
			randYaw = (rand() % 20) - 10;
			randSpeed = (rand() % 10) - 5;

			float yaw = GraphicsPipeline::Instance()->GetCamera()->GetYaw();
			float pitch = GraphicsPipeline::Instance()->GetCamera()->GetPitch();

			if (canJump && pitch < 0) {
				pitch = 0.0f;
			}

			direction = 
				Matrix3::Rotation(pitch + (float)randPitch, Vector3(1.0f, 0.0f, 0.0f)) * 
				Matrix3::Rotation(yaw + (float)randYaw, Vector3(0.0f, 1.0f, 0.0f)) * 
				Vector3(0.0f, 0.0f, -1.0f) * 
				(50 + randSpeed);

			Projectile * spray = new Projectile(col, colour, Physics()->GetPosition(), direction, 0.15f, 5.0f, SPRAY, 1, "Spray");
			SceneManager::Instance()->GetCurrentScene()->AddGameObject(spray,1);
			AudioSystem::Instance()->PlayASound(PROJECTILE_LAUNCH_SOUND, false, physicsNode->GetPosition());
			// Alex Falk - Required for networking
			Game::Instance()->GetUser()->SendWeaponFire(Game::Instance()->GetUserID(), PAINT_SPRAY, Physics()->GetPosition(), direction);
		}
	}
}

void Avatar::ShootRocket()
{
	if (targetLife > minLife + 15.0f)
	{
		targetLife -= 15.0f;
		float yaw = GraphicsPipeline::Instance()->GetCamera()->GetYaw();
		float pitch = GraphicsPipeline::Instance()->GetCamera()->GetPitch();

		if (canJump && pitch < 0) {
			pitch = 0.0f;
		}

		Vector3 direction = Matrix3::Rotation(pitch, Vector3(1.0f, 0.0f, 0.0f)) * Matrix3::Rotation(yaw, Vector3(0.0f, 1.0f, 0.0f)) * Vector3(0.0f, 0.0f, -1.0f) * 30;

		ShootRocket(Physics()->GetPosition(), direction);

		// Alex Falk - Required for networking
		Game::Instance()->GetUser()->SendWeaponFire(Game::Instance()->GetUserID(), PAINT_ROCKET, Physics()->GetPosition(), direction);
	}
}

void Avatar::ShootProjectile()
{
	if (targetLife > minLife + 5.0f)
	{
		targetLife -= 5.0f;
		float yaw = GraphicsPipeline::Instance()->GetCamera()->GetYaw();

		float pitch = GraphicsPipeline::Instance()->GetCamera()->GetPitch();
		if (canJump && pitch < 0) {
			pitch = 0;
		}

		Vector3 direction = Matrix3::Rotation(pitch, Vector3(1.0f, 0.0f, 0.0f)) * Matrix3::Rotation(yaw, Vector3(0.0f, 1.0f, 0.0f)) * Vector3(0.0f, 0.0f, -1.0f) * 50;
		ShootProjectile(Physics()->GetPosition(), direction);

	}
}

//-Alex Falk----------------------------------------------------------//
void Avatar::Spray(Vector3 pos, Vector3 dir)
{
	Projectile * spray = new Projectile(col, colour, pos, dir, 0.15f, 5.0f, SPRAY, 1, "Spray");
	SceneManager::Instance()->GetCurrentScene()->AddGameObject(spray,1);
}

void Avatar::ShootRocket(Vector3 pos, Vector3 dir)
{
	Projectile* projectile = new Projectile(col, colour, pos, dir, { 0.2f,0.2f,0.5f }, 5.0f, PROJECTILE, 5, "Rocket");
	
	float angle = atan2(dir.x, dir.z);
	
	projectile->Physics()->SetOrientation(Quaternion(0, sin(angle / 2), 0, cos(angle / 2)));
	
	SceneManager::Instance()->GetCurrentScene()->AddGameObject(projectile,1);
	AudioSystem::Instance()->PlayASound(ROCKET_FLYING_SOUND, true, projectile->Physics()->GetPosition(), { 0,0,0 }, projectile);
}

void Avatar::ShootProjectile(Vector3 pos, Vector3 dir)
{
	Projectile* projectile = new Projectile(col, colour, pos, dir, 0.18f, 5.0f, PROJECTILE, 2, "Projectile");
	SceneManager::Instance()->GetCurrentScene()->AddGameObject(projectile,1);
	AudioSystem::Instance()->PlayASound(PROJECTILE_LAUNCH_SOUND, false, physicsNode->GetPosition());
}
//--------------------------------------------------------------------//

void Avatar::ManageWeapons() 
{
	if (weaponActive && shooting)
	{
		switch (weapon)
		{
			case PAINT_SPRAY:			Spray();			shootCooldown = 2.5f;	canShoot = false;	break;
			case PAINT_PISTOL:			ShootProjectile();	shootCooldown = 0.5f;						break;
			case AUTO_PAINT_LAUNCHER:	ShootProjectile();	shootCooldown = 0.15f;	canShoot = false;	break;
			case PAINT_ROCKET:			ShootRocket();		shootCooldown = 0.5f;	canShoot = false;	break;
		}
	}
}

