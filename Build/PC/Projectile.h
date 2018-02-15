#pragma once
//Michael Davis 14/02/2018

#include <ncltech/GameObject.h>
#include "Gameplay.h"
#include <ncltech/SceneManager.h>
#include <ncltech\SphereCollisionShape.h>
#include <ncltech\CuboidCollisionShape.h>
#include "Game.h"


class Projectile : public GameObject {
protected:
	Colour colour;
	int projectileWorth; //can be used to determine score increase on collision or life loss on player collision
public:
	Projectile();
	Projectile(Colour col, const Vector4& RGBA, Vector3 pos, Vector3 Velocity, float size, float inverseMass, PhysNodeType type, int projectileWorth, const std::string& name = ""); //spherical projectile
	Projectile(Colour col, const Vector4& RGBA, Vector3 pos, Vector3 Velocity, Vector3 size, float inverseMass, PhysNodeType type, int projectileWorth, const std::string& name = ""); //cuboid projectile
	virtual ~Projectile();

	bool ProjectileCallbackFunction(PhysicsNode* self, PhysicsNode* collidingObject);
	virtual void OnDetachedFromScene();
};