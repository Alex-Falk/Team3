//Michael Davis 14/02/2018

#include "Projectile.h"
#include "Avatar.h"

Projectile::Projectile() : GameObject() {
	colour = START_COLOUR;
	projectileWorth = 0;
}


Projectile::Projectile(Colour col, const Vector4& RGBA, Vector3 pos, Vector3 vel, float size, float inverseMass, PhysNodeType type, int projWorth, const std::string& name) : GameObject(name) {
	RenderNode * rnode = new RenderNode();
	PhysicsNode * pnode = new PhysicsNode();


	RenderNode* dummy = new PlayerRenderNode(CommonMeshes::Sphere(), RGBA);
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
	projectileWorth = projWorth;


	GraphicsPipeline::Instance()->AddPlayerRenderNode(dummy);
	((PlayerRenderNode*)Render()->GetChild())->SetIsInAir(true);
}

Projectile::Projectile(Colour col, const Vector4& RGBA, Vector3 pos, Vector3 vel, Vector3 size, float inverseMass, PhysNodeType type, int projWorth, const std::string& name) : GameObject(name) {
	RenderNode * rnode = new RenderNode();
	PhysicsNode * pnode = new PhysicsNode();

	RenderNode* dummy = new PlayerRenderNode(CommonMeshes::Cube(), RGBA);
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
	projectileWorth = projWorth;

	GraphicsPipeline::Instance()->AddPlayerRenderNode(renderNode);
	((PlayerRenderNode*)Render()->GetChild())->SetIsInAir(true);
}

Projectile::~Projectile() {
	if (renderNode)  GraphicsPipeline::Instance()->RemoveRenderNode(renderNode);
	if (physicsNode) PhysicsEngine::Instance()->RemovePhysicsObject(physicsNode);

	SAFE_DELETE(renderNode);
	SAFE_DELETE(physicsNode);
}

void Projectile::Explode() {
	if (projectileWorth >= 5) {
		int randPitch;
		int randYaw;
		Vector3 direction;

		for (int i = 0; i < 10; ++i)
		{
			randPitch = rand() % 90 + 0;
			randYaw = rand() % 360;

			direction = Matrix3::Rotation(randPitch, Vector3(1, 0, 0)) * Matrix3::Rotation(randYaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * 10;

			Projectile * spray = new Projectile(this->colour, this->Render()->GetchildBaseColor(), Physics()->GetPosition(), direction, 0.15f, 5.0f, SPRAY, 1, "Spray");
			SceneManager::Instance()->GetCurrentScene()->AddGameObject(spray);
			
		}
	}
}


//projectiles go through players and pickups currently.
bool Projectile::ProjectileCallbackFunction(PhysicsNode * self, PhysicsNode * collidingObject) {
	if (collidingObject->GetType() == PLAYER) 
	{
		if (((Avatar*)(collidingObject->GetParent()))->GetColour() != this->colour)
		{
			
			((Avatar*)(collidingObject->GetParent()))->ChangeLife(-projectileWorth);		
			Explode();
			//SceneManager::Instance()->GetCurrentScene()->RemoveGameObject(this);
			//((PlayerRenderNode*)Render()->GetChild())->SetIsInAir(false);
			//((PlayerRenderNode*)Render()->GetChild())->SetDestroy(true);
			destroy = true;
		}
		return false;
	}

	if (collidingObject->GetType() == PICKUP || collidingObject->GetType() == PROJECTILE || collidingObject->GetType() == SPRAY) {
		return false;
	}

	if (collidingObject->GetType() == BIG_NODE) {
		Explode();
	//	SceneManager::Instance()->GetCurrentScene()->RemoveGameObject(this);
		//TODO fix memory leak
		((PlayerRenderNode*)Render()->GetChild())->SetIsInAir(false);
		destroy = true;
		
		
		return false;
	}
	
	return true;
}

void Projectile::OnDetachedFromScene() 
{
	destroy = true;
};