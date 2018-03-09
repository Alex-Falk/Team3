#include "MinionBase.h"
#include "Game.h"
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

	RenderNode* dummy = new PlayerRenderNode(CommonMeshes::Sphere(), name, RGBA);
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
	friendlyName = name;
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

	Physics()->SetOnCollisionCallback(
		std::bind(&MinionBase::MinionCallbackFunction,
			this,							//Any non-placeholder param will be passed into the function each time it is called
			std::placeholders::_1,			//The placeholders correlate to the expected parameters being passed to the callback
			std::placeholders::_2
		)
	);
}

bool MinionBase::MinionCallbackFunction(PhysicsNode * self, PhysicsNode * collidingObject) {
	if (collidingObject->GetType() == PLAYER) {
		if (!dead) {
			if (((Avatar*)(collidingObject->GetParent()))->GetColour() != this->colour) {
				((Avatar*)collidingObject->GetParent())->ChangeLife(-(life / 5));
			}
			else ((Avatar*)(collidingObject->GetParent()))->ChangeLife(life / 5);
			dead = true;
		}
		return false;
	}
	else if (collidingObject->GetType() == MINION) {
		if (!dead && ((MinionBase*)(collidingObject->GetParent()))->IsAlive()) {
			if (((MinionBase*)(collidingObject->GetParent()))->GetColour() != this->colour) {
				float tempLife = life;
				ChangeLife(-((MinionBase*)collidingObject->GetParent())->GetLife());
				((MinionBase*)(collidingObject->GetParent()))->ChangeLife(-(tempLife));
				return false;
			}
		}
		return true;
	}
	if (collidingObject->GetType() == BIG_NODE || collidingObject->GetType() == DEFAULT_PHYSICS) {
		isGrounded = true;
		ChangeLife(-1);
		return true;
	}
	if (collidingObject->GetType() == PAINTABLE_OBJECT)
	{
		if (((CaptureArea*)(collidingObject->GetParent()))->GetColour() != this->colour)
		{
			dead = true;
		}
		isGrounded = true;
		
	}
	if (collidingObject->GetType() == PICKUP) {
		return false;
	}
	return true;
}


void MinionBase::ChangeLife(float l) {
	life += l;
	if (life < minLife) {
		dead = true;
	}
	if (life > maxLife) {
		life = maxLife;
	}
}

void MinionBase::SetLife(float l)
{
	life = l;
	if (life < minLife) {
		dead = true;
	}
	if (life > maxLife) {
		life = maxLife;
	}

	size = 0.3f * (life / 50);
	ChangeSize(size);
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
	
	if (dead)
	{
		this->SetToDestroy();
		Game::Instance()->KillMinion(this);
		((Map*)Game::Instance()->GetMap())->RemoveMinion(this);
		return;
	}

	size = 0.3f * (life / 50);
	ChangeSize(size);
}
