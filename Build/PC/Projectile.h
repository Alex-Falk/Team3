#pragma once

#include "../ncltech/GameObject.h"
#include "Gameplay.h"
#include "../ncltech/SceneManager.h"



class Projectile : public GameObject {
public:
	Projectile();
	Projectile(Colour col, Vector4 RGBA, Vector3 pos, Vector3 Velocity, float size, GameObject * go);
	Projectile(Colour col, Vector4 RGBA, Vector3 pos, Vector3 Velocity, float size, const std::string& name, RenderNode* renderNde, PhysicsNode* physicsNde = NULL);
	virtual ~Projectile();

	bool ProjectileCallbackFunction(PhysicsNode* self, PhysicsNode* collidingObject);

protected:
	Colour colour;
};