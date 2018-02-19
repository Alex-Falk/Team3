//Michael Davis 16/02/2018
#include "Minion.h"

Minion::Minion() : GameObject() {
	colour = START_COLOUR;
	RGBA = DEFAULT_COLOUR;
	life = 50;
	size = 0.5f;
}

Minion::Minion(Colour c, Vector4 RGBA, Vector3 position, const string name) : GameObject(name) {
	size = 0.5f;
	RenderNode * rnode = new RenderNode();
	PhysicsNode * pnode = new PhysicsNode();

	RenderNode* dummy = new RenderNode(CommonMeshes::Sphere(), RGBA);
	dummy->SetTransform(Matrix4::Scale(Vector3(size, size, size)));
	dummy->SetMaterial(GraphicsPipeline::Instance()->GetAllMaterials()[MATERIALTYPE::Forward_Lighting]);
	rnode->AddChild(dummy);

	rnode->GetChild()->SetBaseColor(RGBA);
	rnode->SetTransform(Matrix4::Translation(position));

	pnode->SetBoundingRadius(size);
	rnode->SetBoundingRadius(size);
	pnode->SetLinearVelocity({ 7.0f,0.0f,0.0f});
	pnode->SetPosition(position);
	lastPos = position;
	pnode->SetType(MINION);
	pnode->SetInverseMass(2.0f);

	CollisionShape* pColshape = new SphereCollisionShape(size);
	pnode->SetCollisionShape(pColshape);
	pnode->SetInverseInertia(pColshape->BuildInverseInertia(2.0f));

	pnode->SetOnCollisionCallback(
		std::bind(&Minion::MinionCallbackFunction,
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

	dead = false;
	life = 50;
	colour = c;
	this->RGBA = RGBA;
}

Minion::~Minion() {

}

void Minion::Update(float dt) {

	float lifeLoss = (Physics()->GetPosition() - lastPos).LengthSQ();
	life -= lifeLoss / (dt * 10);
	lastPos = Physics()->GetPosition();

	size = 0.5f * (life / 50);

	ChangeSize(size);

	if (life <= 10) {
		dead = true;
	}
	else {
		//TODO implement AI
		//if no players in range {
		//	state = PATROL;
		//	roam and paint;
		//}
		//if player in range {
		//	state = CHASE;
		//	will try to hit player, either healing if friendly or damaging if enemy
		//}
	}
}

bool Minion::MinionCallbackFunction(PhysicsNode * self, PhysicsNode * collidingObject) {
	if (collidingObject->GetType() == PLAYER) {
		/*if (((Avatar*)(collidingObject->GetParent()))->GetColour() != this->colour) {
			((Avatar*)collidingObject->GetParent())->ChangeLife(-(life / 5));
		}
		else ((Avatar*)(collidingObject->GetParent()))->ChangeLife(life / 5);*/
		dead = true;
		return false;
	}
	else if (collidingObject->GetType() == MINION) {
		/*if (((Minion*)(collidingObject->GetParent()))->GetColour() != this->colour) {
			float tempLife = life;
			ChangeLife(-((Minion*)collidingObject->GetParent())->GetLife());
			((Minion*)(collidingObject->GetParent()))->ChangeLife(-(tempLife));
		}*/
		return false;
	}
	else if (collidingObject->GetType() == BIG_NODE) {
		//TODO apply texture to surface like the avatar
		return true;
	}

	return true;
}

void Minion::ChangeSize(float newSize) {
	Render()->GetChild()->SetBoundingRadius(newSize);
	Render()->SetBoundingRadius(newSize);
	Physics()->SetBoundingRadius(newSize);
	((SphereCollisionShape*)Physics()->GetCollisionShape())->SetRadius(newSize);

	Render()->GetChild()->SetTransform(Matrix4::Scale(Vector3(newSize, newSize, newSize)));
}