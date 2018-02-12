// Nikos Fragkas
// Date 30/01/2018
// Made Player Class and functionality



//Nick Bedford
//Date: 08/02/2018
//Removed setting values to player constructor and
//Created a player callback function

#pragma once

#include "GamePlay.h"
#include <ncltech\GameObject.h>
#include <ncltech\CommonUtils.h>
#include "Weapons.h"


class Player : public GameObject
{
private:

	GameObject* playerGameObject;		//Pointer to the Player's Gameobject

	Colour col;				// Colour - Team
	Vector4 colour;				// The actual colour of the player;

	bool canJump;
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

	// Boosts
	float boostactiveTime;

	bool speedBoost = false;
	float speedTimer;		// Boost timer

	bool jumpBoost = false;
	float jumpBoostTimer;	// Boost timer

	//Weapon
	WeaponType weapon;
	float weaponAmmo;				// Weapon Ammo



	float timer;
			
	Vector3 velocity;

	uint playerId;
public:
	Player();
	Player(Vector3 pos, Colour c, uint id = 0, float s = 1.0f); //Build Player using starting possition Colour and size
	
	virtual void OnPlayerUpdate(float dt);

	virtual Vector3 GetPosition() { return playerGameObject->Physics()->GetPosition(); }

	virtual float GetMaxLife() { return maxLife; }
	virtual void SetMaxLife(float x) { maxLife = x;}

	virtual Colour GetColour() { return col; }
	virtual void SetColour(Colour c) { col = c; }

	virtual Vector4 GetActualColour() { return colour; }

	void ChangeSize(float newSize);

	virtual	float GetSize() { return size; }
	virtual void SetSize(float s) { size = s; }

	virtual void IncrLife(float x) { life += x; }
	virtual void DecrLife(float x) { life -= x; }
	virtual void RestoreLife() { life = maxLife; }
	virtual float GetLife() { return life; }

	virtual void PickedPickUp(PickupType pickType);			//Checks if any pick up is Picked Up			Nikos 13.20
	virtual void UpdatePickUp(float dt);			// Update Pick Ups based on time

	virtual GameObject* GetGameObject() { return playerGameObject; }	//Pointer to the Player's Gameobject

	virtual void Input(float time);		// Takes the keyboard input to control ball


	// Weapon Functions
	//virtual WeaponType GetBulletDirection(Vector3 &pos, Vector3 &dir);
	

	virtual void ManageWeapons(WeaponType wt);


										//Example of member callback ('this' parameter is bound at bind time)
	bool PlayerCallbackFunction(PhysicsNode* self, PhysicsNode* collidingObject);


	~Player();

};

