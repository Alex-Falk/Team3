#include "Projectile.h"

Projectile::Projectile() {

}


Projectile::Projectile(Colour col, const Vector4& RGBA, Vector3 pos, Vector3 vel, float size, float inverseMass, const std::string& name) : GameObject(name) {
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
	pnode->SetInverseMass(inverseMass);
	
	colour = col;

	CollisionShape* pColshape = new SphereCollisionShape(size);
	pnode->SetCollisionShape(pColshape);
	pnode->SetInverseInertia(pColshape->BuildInverseInertia(inverseMass));

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

Projectile::Projectile(Colour col, const Vector4& RGBA, Vector3 pos, Vector3 vel, Vector3 size, float inverseMass, const std::string& name) : GameObject(name) {
	RenderNode * rnode = new RenderNode();
	PhysicsNode * pnode = new PhysicsNode();

	RenderNode* dummy = new RenderNode(CommonMeshes::Cube(), RGBA);
	dummy->SetTransform(Matrix4::Scale(size));
	rnode->AddChild(dummy);

	rnode->GetChild()->SetBaseColor(RGBA);
	rnode->SetTransform(Matrix4::Translation(pos));

	float maxDimension = size.x;
	if (size.y > maxDimension) {
		maxDimension = size.y;
	}
	if (size.z > maxDimension) {
		maxDimension = size.z;
	}

	pnode->SetBoundingRadius(maxDimension);
	rnode->SetBoundingRadius(maxDimension);
	pnode->SetLinearVelocity(vel);
	pnode->SetPosition(pos);
	pnode->SetType(PROJECTILE);
	pnode->SetInverseMass(inverseMass);

	colour = col;

	CollisionShape* pColshape = new CuboidCollisionShape(size);
	pnode->SetCollisionShape(pColshape);
	pnode->SetInverseInertia(pColshape->BuildInverseInertia(inverseMass));

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
	//TODO what happens when a projectile hits another player
	/*if (collidingObject->GetType() == PLAYER) {
		return false;
	}*/

	if (collidingObject->GetType() == PICKUP) {
		return false;
	}
	if (collidingObject->GetType() == BIG_NODE) {
		//TODO leave a texture behind on the wall instead of colouring the entire object
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