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
#pragma once
#include "GamePlay.h"
#include <ncltech\GameObject.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\CommonMeshes.h>
#include <nclgl\PlayerRenderNode.h>


class Avatar : public GameObject
{
protected:

	//GameObject* playerGameObject;		//Pointer to the Player's Gameobject

	Colour col;				    // Colour - Team
	Vector4 colour;				// The actual colour of the player;

	bool canJump;
	bool canShoot;
	bool inAir;

	float life;
	float maxLife;
	float minLife;

	float size;					//Player size
	float curSize;
	float jumpImpulse;    	    // Jump Power
	float standardJumpImpulse;
	float boostedJumpImpulse;
	float speed;			    // movement speed
	float boostedSpeed;
	float standardSpeed;
	float maxForce;			// Sets Maximum applied Force 
	float shootCooldown;
	float collisionTimer;
	


	// Boosts
	PickupType activePickUp;
	float boostactiveTime;

	bool speedBoost = false;
	float speedTimer;		// Boost timer

	bool jumpBoost = false;
	float jumpBoostTimer;	// Boost timer

							//Weapon
	WeaponType weapon;
	bool weaponActive = false;
	float weaponTimer;				// Weapon timer
	bool shooting;

	Vector3 velocity;
	Vector3 lastPos; //used to determine distance travelled each frame for life

	uint playerId;

	void ChangeSize(float newSize);

public:
	Avatar();
	Avatar(Vector3 pos, Colour c, uint id = 0, float s = 1.0f); //Build Player using starting possition Colour and size

	virtual void OnAvatarUpdate(float dt);

	Vector3 GetPosition() { return Physics()->GetPosition(); }

	float GetMaxLife() { return maxLife; }
	void SetMaxLife(float x) { maxLife = x; }

	Colour GetColour() { return col; }
	void SetColour(Colour c) { col = c; }

	WeaponType GetWeapon() { return weapon; }
	void SetWeapon(WeaponType newWeapon) { weapon = newWeapon; }

	bool IsPlayerInAir() { return inAir; }
	
	Vector4 GetColourRGBA() { return colour; }

	void Spray();
	void ShootRocket();
	void ShootProjectile();
	
	float GetSize() { return size; }
	void SetSize(float s) { size = s; }

	void ChangeLife(float x) { life += x; if (life < minLife) { life = minLife; } else if (life > maxLife) { life = maxLife; }	}

	void RestoreLife() { life = maxLife/2; }
	float GetLife() { return life; }
	void SetLife(float l) { life = l; }

	virtual void PickUpBuffActivated(PickupType pickType);			//Checks if any pick up is picked up			Nikos 13.20
	void UpdatePickUp(float dt);			// Update Pick Ups based on time

	GameObject* GetGameObject() { return this; }	//Pointer to the Player's Gameobject

	
	virtual void ManageWeapons();
	virtual bool GetShooting() { return shooting; }
	//Example of member callback ('this' parameter is bound at bind time)
	bool PlayerCallbackFunction(PhysicsNode* self, PhysicsNode* collidingObject);

	virtual ~Avatar();

};

