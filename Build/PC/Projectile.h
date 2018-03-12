#pragma once
//Michael Davis 14/02/2018

#include <ncltech/GameObject.h>
#include "Gameplay.h"
//#include "Game.h"


class Projectile : public GameObject {
protected:
	Colour colour;
	float projectileWorth; //can be used to determine score increase on collision or life loss on player collision
	float siz; //Nikos Fragkas, 16/02, Needed for score 
	bool exploded; //prevent double explosion at lower framerates

	Vector3 predictedCollisionPosition; //position of pnode, not position of contact point
public:
	Projectile();
	Projectile(Colour col, const Vector4& RGBA, Vector3 pos, Vector3 Velocity, float size, float inverseMass, PhysNodeType type, int projectileWorth, const std::string& name = ""); //spherical projectile
	Projectile(Colour col, const Vector4& RGBA, Vector3 pos, Vector3 Velocity, Vector3 size, float inverseMass, PhysNodeType type, int projectileWorth, const std::string& name = ""); //cuboid projectile
	inline float GetSize() { return siz; }
	inline Colour GetColour() { return colour; }

	Vector3 GetPredictedCollisionPosition() {
		return predictedCollisionPosition;
	}
	void SetPredictedCollisionPosition(Vector3 pcp) {
		predictedCollisionPosition = pcp;
	}

	float GetProjectileWorth() { return projectileWorth; }

	void predictCollisionPosition(Vector3 pos, Vector3 vel);
	
	virtual ~Projectile();

	void Explode(); //paint spray comes out of it

	bool ProjectileCallbackFunction(PhysicsNode* self, PhysicsNode* collidingObject);
	virtual void OnDetachedFromScene();
};