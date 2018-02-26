#include "Explosion.h"

Explosion::Explosion() : Projectile() {

}

Explosion::Explosion(Colour col, const Vector4& RGBA, Vector3 pos, Vector3 velocity, float size, float inverseMass, PhysNodeType type, int projectileWorth, const std::string& name) : Projectile(col, RGBA, pos, velocity, size, inverseMass, type, projectileWorth, name) {
	physicsNode->SetOnCollisionCallback(
		std::bind(&Explosion::ExplosionCallbackFunction,
			this,							//Any non-placeholder param will be passed into the function each time it is called
			std::placeholders::_1,			//The placeholders correlate to the expected parameters being passed to the callback
			std::placeholders::_2
		)
	);
}

Explosion::~Explosion() {

}

bool Explosion::ExplosionCallbackFunction(PhysicsNode * self, PhysicsNode * collidingObject) {
	Vector3 direction = collidingObject->GetPosition() - self->GetPosition();
	float length = direction.Length();
	length = Physics()->GetBoundingRadius() - length;
	direction = direction.Normalise();
	
	length = length / self->GetBoundingRadius();

	if (length < 0.5f) {
		length = 0.5f;
	}
	else if (length > 1.0f) {
		length = 1.0f;
	}

	float forceMultiplier = 20 * length;
	
	if (collidingObject->GetType() == PLAYER) {
		((Avatar*)(collidingObject->GetParent()))->ChangeLife((float)(-projectileWorth * (length * 2)));
		((PlayerRenderNode*)((Avatar*)(collidingObject->GetParent()))->Render()->GetChild())->SetIsInAir(true);
		collidingObject->SetLinearVelocity((collidingObject->GetLinearVelocity() / 2) + direction * forceMultiplier);
		destroy = true;
		return false;
	}

	if (collidingObject->GetType() == MINION) {
		((Minion*)(collidingObject->GetParent()))->ChangeLife((float)(-projectileWorth * (length * 2)));
		collidingObject->SetLinearVelocity((collidingObject->GetLinearVelocity() / 2) + direction * forceMultiplier);
		destroy = true;
		return false;
	}

	if (collidingObject->GetType() == BIG_NODE) {
		destroy = true;
		return false;
	}
	if (collidingObject->GetType() == DEFAULT_PHYSICS) {
		destroy = true;
		return false;
	}

	if (collidingObject->GetType() == PICKUP) {
		return false;
	}

	if (collidingObject->GetType() == PROJECTILE || collidingObject->GetType() == SPRAY) {
		((Projectile*)collidingObject->GetParent())->SetToDestroy();
		return false;
	}
	destroy = true;
	return true;
}