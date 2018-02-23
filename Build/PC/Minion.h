//Michael Davis 16/02/2018
#pragma once

#include "GamePlay.h"
#include "../ncltech/GameObject.h"
#include "../ncltech/SphereCollisionShape.h"
#include "Avatar.h"
#include "../ncltech/SceneManager.h"


class Minion : public GameObject {
protected:
	Colour colour;
	Vector4 RGBA;
	
	float life;
	float minLife;
	float maxLife;
	float size;

	Vector3 lastPos;

	bool dead; 

public:
	Minion();
	Minion(Colour c, Vector4 RGBA, Vector3 position, const string name = "");
	
	void SetColour(Colour c) { colour = c;	}
	Colour GetColour() { return colour; }

	void SetRGBA(Vector4 r) { RGBA = r; }
	Vector4 GetRGBA() { return RGBA; }

	void SetDead(bool b) { dead = b; }
	bool GetDead() { return dead; }

	void ChangeLife(float l);
	float GetLife() { return life; }

	void ChangeSize(float newSize);

	void Update(float dt);
	bool MinionCallbackFunction(PhysicsNode* self, PhysicsNode* collidingObject);
	~Minion();
};