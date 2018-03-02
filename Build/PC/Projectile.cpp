//Michael Davis 14/02/2018

#include "Projectile.h"
#include "Avatar.h"
#include "Explosion.h"

Projectile::Projectile() : GameObject() {
	colour = START_COLOUR;
	projectileWorth = 0;
	predictedCollisionPosition = {0,0,0};
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
	exploded = false;

	predictCollisionPosition(pos, vel);
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

	GraphicsPipeline::Instance()->AddPlayerRenderNode(dummy);
	((PlayerRenderNode*)Render()->GetChild())->SetIsInAir(true);
	exploded = false;


	predictCollisionPosition(pos, vel);
}

Projectile::~Projectile() {

}

void Projectile::predictCollisionPosition(Vector3 pos, Vector3 vel) {
	LineCollision l = PhysicsEngine::Instance()->CastRay(pos, vel.Normalise(), physicsNode);
	if (l.node == NULL) {
		predictedCollisionPosition = { 1000,1000,1000 };
	}
	else {
		predictedCollisionPosition = pos + vel.Normalise() * l.dist;
	}
}

void Projectile::Explode() {
	exploded = true;

	//turn into sphere for spherical paint splat
	Render()->GetChild()->SetTransform((Matrix4::Scale(Vector3(2.0f, 2.0f, 2.0f))));
	Render()->GetChild()->SetMesh(CommonMeshes::Sphere());

	Explosion * explosion = new Explosion(this->colour, Vector4{ 1.0f, 1.0f, 1.0f, 0.0f }, Physics()->GetPosition(), { 0,0,0 }, 2.0f, 5.0f, SPRAY, 4, "Spray");
	explosion->UnregisterPhysicsToRenderTransformCallback();
	explosion->Render()->SetTransform(Matrix4::Translation(Vector3{ 1000.f,1000.f,1000.f }));
	SceneManager::Instance()->GetCurrentScene()->AddGameObject(explosion);
	
	//move above the arena so we don't see the sphere for the frame it exists
	Physics()->SetPosition(Physics()->GetPosition() + Vector3{ 0,200,0 }); 
}


//projectiles go through players and pickups currently.
bool Projectile::ProjectileCallbackFunction(PhysicsNode * self, PhysicsNode * collidingObject) {
	if (collidingObject->GetType() == PLAYER) 
	{
		if (((Avatar*)(collidingObject->GetParent()))->GetColour() != this->colour)
		{	
			if (projectileWorth >= 5 && !exploded) Explode();
			else {
				((Avatar*)(collidingObject->GetParent()))->ChangeLife((float)(-projectileWorth));
			}
			((PlayerRenderNode*)Render()->GetChild())->SetIsInAir(false);
			destroy = true;
		}
		return false;
	}

	if (collidingObject->GetType() == MINION) {
		((PlayerRenderNode*)Render()->GetChild())->SetIsInAir(false);
		if (projectileWorth >= 5 && !exploded) Explode(); 
		else {
			if (((Minion*)(collidingObject->GetParent()))->GetColour() != this->colour) {
				((Minion*)(collidingObject->GetParent()))->ChangeLife((float)(-projectileWorth));
			}
		}
		destroy = true;
		return false;
	}

	if (collidingObject->GetType() == BIG_NODE || collidingObject->GetType() == DEFAULT_PHYSICS) {
		if (projectileWorth >= 5 && !exploded) Explode();
		((PlayerRenderNode*)Render()->GetChild())->SetIsInAir(false);
		destroy = true;
		return false;
	}

	if (collidingObject->GetType() == INVISIBLE_WALL) {
		if (projectileWorth >= 5 && !exploded) Explode();
		destroy = true;
		return false;
	}

	if (collidingObject->GetType() == PICKUP || collidingObject->GetType() == PROJECTILE || collidingObject->GetType() == SPRAY) {
		return false;
	}
	return true;
}

void Projectile::OnDetachedFromScene() 
{
	destroy = true;
};