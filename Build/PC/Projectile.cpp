#include "Projectile.h"

Projectile::Projectile() {

}

Projectile::Projectile(Colour col, Vector4 RGBA, Vector3 pos, Vector3 vel, float size, GameObject * go) : GameObject(*go) {
	this->Physics()->SetLinearVelocity(vel);
	this->Physics()->SetPosition(pos);
	colour = col;
	this->Render()->GetChild()->SetBaseColor(RGBA);

	Physics()->SetOnCollisionCallback(
		std::bind(&Projectile::ProjectileCallbackFunction,
			this,							//Any non-placeholder param will be passed into the function each time it is called
			std::placeholders::_1,			//The placeholders correlate to the expected parameters being passed to the callback
			std::placeholders::_2
		)
	);
}

Projectile::Projectile(Colour col, Vector4 RGBA, Vector3 pos, Vector3 vel, float size, const std::string& name, RenderNode* renderNde, PhysicsNode* physicsNde) : GameObject(name, renderNde, physicsNde) {

	this->Physics()->SetLinearVelocity(vel);
	this->Physics()->SetPosition(pos);
	colour = col;
	this->Render()->GetChild()->SetBaseColor(RGBA);

	Physics()->SetOnCollisionCallback(
		std::bind(&Projectile::ProjectileCallbackFunction,
			this,							//Any non-placeholder param will be passed into the function each time it is called
			std::placeholders::_1,			//The placeholders correlate to the expected parameters being passed to the callback
			std::placeholders::_2
		)
	);
}

Projectile::~Projectile() {

}

//projectiles go through players and pickups currently.
bool Projectile::ProjectileCallbackFunction(PhysicsNode * self, PhysicsNode * collidingObject) {
	if (collidingObject->GetType() == PLAYER) {
		return false;
	}
	else if (collidingObject->GetType() == PICKUP) {
		return false;
	}
	else {
		SceneManager::Instance()->GetCurrentScene()->RemoveGameObject(this);
		collidingObject->GetParent()->Render()->GetChild()->SetBaseColor(this->Render()->GetChild()->GetBaseColor());
		delete this;
	}
	return false;
}