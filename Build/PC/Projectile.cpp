#include "Projectile.h"

Projectile::Projectile() {

}


Projectile::Projectile(Colour col, const Vector4& RGBA, Vector3 pos, Vector3 vel, float size, const std::string& name) : GameObject(name) {
	RenderNode * rnode = new RenderNode();
	PhysicsNode * pnode = new PhysicsNode();

	RenderNode* dummy = new RenderNode(CommonMeshes::Sphere(), RGBA);
	dummy->SetTransform(Matrix4::Scale(Vector3(size, size, size)));
	rnode->AddChild(dummy);

	rnode->GetChild()->SetBaseColor(RGBA);
	rnode->SetTransform(Matrix4::Translation(pos));

	pnode->SetBoundingRadius(size);
	rnode->SetBoundingRadius(size);
	pnode->SetLinearVelocity(vel);
	pnode->SetPosition(pos);
	pnode->SetType(PROJECTILE);
	pnode->SetInverseMass(0.1f);
	
	colour = col;

	CollisionShape* pColshape = new SphereCollisionShape(size);
	pnode->SetCollisionShape(pColshape);
	pnode->SetInverseInertia(pColshape->BuildInverseInertia(0.1f));

	pnode->SetOnCollisionCallback(
		std::bind(&Projectile::ProjectileCallbackFunction,
			this,							//Any non-placeholder param will be passed into the function each time it is called
			std::placeholders::_1,			//The placeholders correlate to the expected parameters being passed to the callback
			std::placeholders::_2
		)
	);

	renderNode = rnode;
	physicsNode = pnode;

	RegisterPhysicsToRenderTransformCallback();
	SetPhysics(physicsNode);
	physicsNode->SetName(name);
}

Projectile::~Projectile() {

}

//projectiles go through players and pickups currently.
bool Projectile::ProjectileCallbackFunction(PhysicsNode * self, PhysicsNode * collidingObject) {
	/*if (collidingObject->GetType() == PLAYER) {
		return false;
	}*/

	if (collidingObject->GetType() == PICKUP) {
		return false;
	}
	if (collidingObject->GetType() == BIG_NODE) {
		collidingObject->GetParent()->Render()->GetChild()->SetBaseColor(this->Render()->GetChild()->GetBaseColor());
		SceneManager::Instance()->GetCurrentScene()->RemoveGameObject(this);
		return false;
		
		
	}
	
	return true;
}

void Projectile::OnDetachedFromScene() 
{
	delete this;
};