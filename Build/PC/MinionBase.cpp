#include "MinionBase.h"

MinionBase::MinionBase()
{
	colour = START_COLOUR;
	RGBA = DEFAULT_COLOUR;
	life = 50;
	size = 0.3f;
}

MinionBase::MinionBase(Colour c, Vector4 RGBA, Vector3 position, const string name)
{
	size = 0.3f;
	RenderNode * rnode = new RenderNode();
	PhysicsNode * pnode = new PhysicsNode();

	RenderNode* dummy = new PlayerRenderNode(CommonMeshes::Sphere(), "Minion", RGBA);
	dummy->SetTransform(Matrix4::Scale(Vector3(size, size, size)));
	dummy->SetMaterial(GraphicsPipeline::Instance()->GetAllMaterials()[MATERIALTYPE::Forward_Lighting]);
	rnode->AddChild(dummy);

	rnode->GetChild()->SetBaseColor(RGBA);
	rnode->SetTransform(Matrix4::Translation(position));

	pnode->SetBoundingRadius(size);
	rnode->SetBoundingRadius(size);

	pnode->SetPosition(position);
	lastPos = position;
	pnode->SetType(MINION);
	pnode->SetInverseMass(2.0f);

	CollisionShape* pColshape = new SphereCollisionShape(size);
	pnode->SetCollisionShape(pColshape);
	pnode->SetInverseInertia(pColshape->BuildInverseInertia(2.0f));
	pnode->SetFriction(0.0f);
	pnode->SetElasticity(1.0f);

	renderNode = rnode;

	RegisterPhysicsToRenderTransformCallback();

	SetPhysics(pnode);
	physicsNode->SetName(name);

	dead = false;

	minLife = 10;
	maxLife = 50;
	life = maxLife;

	colour = c;
	this->RGBA = RGBA;

	GraphicsPipeline::Instance()->AddPlayerRenderNode(dummy);
	((PlayerRenderNode*)Render()->GetChild())->SetIsInAir(false);

	isGrounded = false;
}

void MinionBase::ChangeLife(float l) {
	life += l;
	if (life < minLife) {
		dead = true;
		destroy = true;
	}
	if (life > maxLife) {
		life = maxLife;
	}
}

void MinionBase::ChangeSize(float newSize) {
	Render()->GetChild()->SetBoundingRadius(newSize);
	Render()->SetBoundingRadius(newSize);
	Physics()->SetBoundingRadius(newSize);
	((SphereCollisionShape*)Physics()->GetCollisionShape())->SetRadius(newSize);

	Render()->GetChild()->SetTransform(Matrix4::Scale(Vector3(newSize, newSize, newSize)));
}

void MinionBase::Update(float dt)
{
	lastPos = Physics()->GetPosition();

	size = 0.3f * (life / 50);

	ChangeSize(size);

	if (life < minLife) {
		dead = true;
		destroy = true;
	}
}