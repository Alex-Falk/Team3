#include "Projectile.h"

Projectile::Projectile() : GameObject() {
	colour = START_COLOUR;
}


Projectile::Projectile(Colour col, const Vector4& RGBA, Vector3 pos, Vector3 vel, float size, float inverseMass, PhysNodeType type, const std::string& name) : GameObject(name) {
	RenderNode * rnode = new RenderNode();
	PhysicsNode * pnode = new PhysicsNode();

	RenderNode* dummy = new RenderNode(CommonMeshes::Sphere(), RGBA);
	dummy->SetTransform(Matrix4::Scale(Vector3(size, size, size)));
	dummy->SetMaterial(GraphicsPipeline::Instance()->GetAllMaterials()[MATERIALTYPE::Forward_Lighting]);
	rnode->AddChild(dummy);

	rnode->GetChild()->SetBaseColor(RGBA);
	rnode->SetTransform(Matrix4::Translation(pos));

	pnode->SetBoundingRadius(size);
	rnode->SetBoundingRadius(size);
	pnode->SetLinearVelocity(vel);
	pnode->SetPosition(pos);
	pnode->SetType(type);
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
	destroy = false;
}

Projectile::Projectile(Colour col, const Vector4& RGBA, Vector3 pos, Vector3 vel, Vector3 size, float inverseMass, PhysNodeType type, const std::string& name) : GameObject(name) {
	RenderNode * rnode = new RenderNode();
	PhysicsNode * pnode = new PhysicsNode();

	RenderNode* dummy = new RenderNode(CommonMeshes::Cube(), RGBA);
	dummy->SetTransform(Matrix4::Scale(size));
	dummy->SetMaterial(GraphicsPipeline::Instance()->GetAllMaterials()[MATERIALTYPE::Forward_Lighting]);
	rnode->AddChild(dummy);

	rnode->GetChild()->SetBaseColor(RGBA);
	rnode->SetTransform(Matrix4::Translation(pos));

	float x = size.x*2.0f;
	float y = size.y*2.0f;
	float z = size.z*2.0f;
	float a;
	if (x >= y && x >= z) { a = x; }
	else if (y > x && y >= z) { a = y; }
	else { a = z; }

	pnode->SetBoundingRadius(a * sqrt(3.0f) / 2.0f);
	rnode->SetBoundingRadius(size.Length());
	pnode->SetLinearVelocity(vel);
	pnode->SetPosition(pos);
	pnode->SetType(type);
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
	destroy = false;
}

Projectile::~Projectile() {
	
}


//projectiles go through players and pickups currently.
bool Projectile::ProjectileCallbackFunction(PhysicsNode * self, PhysicsNode * collidingObject) {
	//TODO what happens when a projectile hits another player
	if (collidingObject->GetType() == PLAYER) {
		return false;
	}

	if (collidingObject->GetType() == PICKUP) {
		return false;
	}
	if (collidingObject->GetType() == PROJECTILE) {
		return false;
	}
	if (collidingObject->GetType() == SPRAY) {
		return false;
	}
	if (collidingObject->GetType() == BIG_NODE) {
		//TODO leave a texture behind on the wall instead of colouring the entire object
		//collidingObject->GetParent()->Render()->GetChild()->SetBaseColor(this->Render()->GetChild()->GetBaseColor());
		SceneManager::Instance()->GetCurrentScene()->RemoveGameObject(this);
		
		return false;
	}
	
	return true;
}

void Projectile::OnDetachedFromScene() 
{
	destroy = true;
};