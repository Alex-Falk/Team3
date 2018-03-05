#pragma once

#include "GamePlay.h"
#include "../ncltech/GameObject.h"
#include "../ncltech/SphereCollisionShape.h"
#include "CaptureArea.h" 
#include "Avatar.h"
#include "../ncltech/SceneManager.h"

class MinionBase : public GameObject {
protected:
	Colour colour;
	Vector4 RGBA;

	float life;
	float minLife;
	float maxLife;
	float size;

	Vector3 lastPos;

	bool isGrounded;
	bool dead;

public:
	MinionBase();
	MinionBase(Colour c, Vector4 RGBA, Vector3 position, const string name = "");

	void SetColour(Colour c) { colour = c; }
	Colour GetColour() { return colour; }

	float GetMinLife() { return minLife; }

	void SetRGBA(Vector4 r) { RGBA = r; }
	Vector4 GetRGBA() { return RGBA; }

	void SetDead(bool b) { dead = b; }

	void SetIsGrounded(bool b) { isGrounded = b; }
	bool GetIsGrounded() { return isGrounded; }

	void ChangeLife(float l);
	float GetLife() { return life; }


	void ChangeSize(float newSize);

	virtual void Update(float dt);

	bool IsAlive() { return !dead; }
};