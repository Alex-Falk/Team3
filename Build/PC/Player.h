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

	int Life;
	
	float size;					//Player size
	float jumpImpulse = 10.f;	// Jump Power
	float speed = 10;			// movement speed
	float Jumptime = 5;			// Testing stop double jump
	float Jtime = 0;			// Only for Testing
	float maxForce = 5;			// Sets Maximum applied Force 

	bool canJump;

public:
	Player();
	Player(Vector3 pos, Colour c, float s); //Build Player using starting possition Colour and size
	~Player();

	virtual Colour GetColour() { return colour; }
	virtual void SetColour(Colour c) { colour = c; }

	virtual	float GetSize() { return size; }
	virtual void SetSize(float s) { size = s; }

	virtual GameObject* GetGameObject() { return playerGameObject; }	//Pointer to the Player's Gameobject

	virtual void Input(float time);		// Takes the keyboard input to control ball

};