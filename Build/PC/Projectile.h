#pragma once

#include "../ncltech/GameObject.h"
#include "Gameplay.h"
#include "../ncltech/SceneManager.h"
#include "../ncltech/SphereCollisionShape.h"
#include "../ncltech/CuboidCollisionShape.h"


class Projectile : public GameObject {
public:
	Projectile();
	Projectile(Colour col, const Vector4& RGBA, Vector3 pos, Vector3 Velocity, float size, const std::string& name = "");
	virtual ~Projectile();

	bool ProjectileCallbackFunction(PhysicsNode* self, PhysicsNode* collidingObject);
	virtual void OnDetachedFromScene();

protected:
	Colour colour;
};