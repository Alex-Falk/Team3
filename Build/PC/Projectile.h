#pragma once

#include "../ncltech/GameObject.h"
#include "Gameplay.h"
#include "../ncltech/SceneManager.h"
#include "../ncltech/SphereCollisionShape.h"
#include "../ncltech/CuboidCollisionShape.h"


class Projectile : public GameObject {
public:
	Projectile();
	Projectile(Colour col, const Vector4& RGBA, Vector3 pos, Vector3 Velocity, float size, float inverseMass, const std::string& name = ""); //spherical projectile
	Projectile(Colour col, const Vector4& RGBA, Vector3 pos, Vector3 Velocity, Vector3 size, float inverseMass, const std::string& name = ""); //cuboid projectile
	virtual ~Projectile();

	bool ProjectileCallbackFunction(PhysicsNode* self, PhysicsNode* collidingObject);
	virtual void OnDetachedFromScene();

protected:
	Colour colour;
};