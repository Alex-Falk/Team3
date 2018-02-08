//Nick Bedford
//Date: 07/02/2018
//Added a default value to player size

#pragma once
#include <ncltech\GameObject.h>
#include <ncltech\CommonUtils.h>
#include "GamePlay.h"

class Player : public GameObject
{
private:

	GameObject* playerGameObject;		//Pointer to the Player's Gameobject

	Vector3 force = Vector3(0, 0, 0);	//Hold the force for smooth movement

	Colour colour;				// Colour - Team

	float life;
	float maxLife = 100;
	float minLife = 10;

	float size;					//Player size
	float curSize;
	float jumpImpulse = 10.f;	// Jump Power
	float speed = 10;			// movement speed
	float maxForce = 15;			// Sets Maximum applied Force 

	static bool canJump;
	static bool inAir;

public:
	Player();
	Player(Vector3 pos, Colour c, float s = 1.0f); //Build Player using starting possition Colour and size
	
	virtual void OnPlayerUpdate(float dt);

	virtual float GetMaxLife() { return maxLife; }
	virtual void SetMaxLife(float x) { maxLife = x;}

	virtual Colour GetColour() { return colour; }
	virtual void SetColour(Colour c) { colour = c; }

	void ChangeSize(float newSize);

	virtual	float GetSize() { return size; }
	virtual void SetSize(float s) { size = s; }

	virtual void IncrLife(float x) { life += x; }
	virtual void DecrLife(float x) { life -= x; }

	virtual float GetLife() { return life; }

	virtual GameObject* GetGameObject() { return playerGameObject; }	//Pointer to the Player's Gameobject

	virtual void Input(float time);		// Takes the keyboard input to control ball

	static bool SetCanJump(PhysicsNode* self, PhysicsNode* collidingObject);	//Sets player's ability to jump

	~Player();

};

