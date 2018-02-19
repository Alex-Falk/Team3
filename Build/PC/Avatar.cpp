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

#include "Avatar.h"
#include <ncltech\SphereCollisionShape.h>
#include <string.h>
#include "GameInput.h"
#include <ncltech\CommonMeshes.h>
#include <nclgl\PlayerRenderNode.h>
#include "Projectile.h"
#include "Pickup.h"
#include "WeaponPickup.h"
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

	standardSpeed = 5.0f;
	speed = standardSpeed;
	boostedSpeed = standardSpeed * 20;

	maxForce = 30;

	minLife = 10;
	maxLife = 100;
	life = maxLife/2;

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

	weapon = NUM_OF_WEAPONS;

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
	case PINK:
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

	RenderNode* rnode = new RenderNode();
	float radius = s;
	RenderNode* dummy = new PlayerRenderNode(CommonMeshes::Sphere(), colour);
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
	

}

bool Avatar::PlayerCallbackFunction(PhysicsNode* self, PhysicsNode* collidingObject) {

	if (collidingObject->GetType() == DEFAULT_PHYSICS || collidingObject->GetType() == BIG_NODE || (collidingObject->GetType() == PAINTABLE_OBJECT))
	{
		if ((Pickup*)(collidingObject->GetParent())) 
		{
			activePickUp = ((Pickup*)(collidingObject->GetParent()))->GetPickupType();
			if (activePickUp == WEAPON)
			{
				weapon = ((WeaponPickup*)(collidingObject->GetParent()))->GetWeaponType();
			}
			PickUpBuffActivated(activePickUp);
		}

		canJump = true;
		inAir = false;
		((PlayerRenderNode*)Render()->GetChild())->SetIsInAir(false);
	}

	return true;
}


void Avatar::ChangeSize(float newSize)
{
	Render()->GetChild()->SetBoundingRadius(newSize);
	Render()->SetBoundingRadius(newSize);
	Physics()->SetBoundingRadius(newSize);
	Physics()->SetInverseMass(0.5f/newSize);
	((SphereCollisionShape*)Physics()->GetCollisionShape())->SetRadius(newSize);
	
	Render()->GetChild()->SetTransform(Matrix4::Scale(Vector3(newSize, newSize, newSize)));
}

// Updates everything on player
void Avatar::OnAvatarUpdate(float dt) {

	shooting = false;
	
	inAir = true;
	((PlayerRenderNode*)Render()->GetChild())->SetIsInAir(true);

	ManageWeapons();
	
	UpdatePickUp(dt);

	if (life > minLife)
	{
		
		if (life < minLife)
		{
			life = minLife;
		}
	}

	curSize = size * (life / 100);

	ChangeSize(curSize);

}


void Avatar::PickUpBuffActivated(PickupType pickType) {

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
	case WEAPON:
		weaponActive = true;
		weaponTimer = boostactiveTime;
		break;
	default:
		break;
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
			jumpImpulse = standardSpeed;
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
	
	
	for (int i = 0; i < 10; ++i)
	{
		randPitch = rand() % 90 + 0;
		randYaw = rand() % 360;
		float a = (float)(rand() % 10);
		float b = (float)(rand() % 10);
		float c = (float)(rand() % 10);
		direction = Matrix3::Rotation((float)randPitch, Vector3(1.0f, 0.0f, 0.0f)) * Matrix3::Rotation((float)randYaw, Vector3(0.0f, 1.0f, 0.0f)) * Vector3(0.0f, 0.0f, -1.0f) * 15;

		Projectile * spray = new Projectile(col, colour, Physics()->GetPosition(), direction, 0.2f, 5.0f, SPRAY, 2, "Spray");

		SceneManager::Instance()->GetCurrentScene()->AddGameObject(spray);
	}
}

void Avatar::ShootRocket()
{
	/*

	Vector3 direction = playerVelocityDirection;

	Rocket* projectile =  new Rocket(position, direction, size, enum Colour, Vector4 RGBA);;

	SceneManager::Instance()->GetCurrentScene()->AddGameObject(projectile);
	*/
}

void Avatar::ShootProjectile()
{
	/*
	
	Vector3 direction = playerVelocityDirection;

	Projectile* projectile =  new Projectile(position, direction, size, enum Colour, Vector4 RGBA);;
	
	SceneManager::Instance()->GetCurrentScene()->AddGameObject(projectile);
	*/
}

void Avatar::ManageWeapons() 
{
	if (weaponActive && shooting)
	{
		switch (weapon)
		{
			case PAINT_SPRAY:
				
				Spray();
				shootCooldown = 2.5f;
				canShoot = false;
				break;

			case PAINT_PISTOL:
				
				ShootProjectile();
				shootCooldown = 0.5f;
				break;

			case AUTO_PAINT_LAUNCHER:
				
				ShootProjectile();
				shootCooldown = 0.15f;
				canShoot = false;
				break;

			case PAINT_ROCKET:
				
				ShootRocket();
				shootCooldown = 0.5f;
				canShoot = false;
				
				break;

			default:
				break;
		}
	}
}