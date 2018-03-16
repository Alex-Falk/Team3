#include "LaunchPad.h"

LaunchPad::LaunchPad(Vector3 pos, Vector3 halfDims, string unique_name, float power):power(power) {
	friendlyName = unique_name;

	RenderNode* rnode = new RenderNode();
	RenderNode* dummy = new RenderNode(CommonMeshes::Cube(), "PickUp", Vector4(0.0f, 1.0f, 0.0f, 1.0f));

	dummy->SetTransform(Matrix4::Scale(halfDims));

	dummy->SetMaterial(GraphicsPipeline::Instance()->GetAllMaterials()[MATERIALTYPE::Forward_Lighting]);

	rnode->AddChild(dummy);

	rnode->SetTransform(Matrix4::Translation(pos));
	rnode->SetBoundingRadius(halfDims.Length());

	PhysicsNode* pnode = NULL;

	pnode = new PhysicsNode();
	pnode->SetPosition(pos);
	pnode->SetInverseMass(0.0f);
	pnode->SetType(PICKUP);
	pnode->SetName(unique_name);

	float x = halfDims.x*2.0f;
	float y = halfDims.y*2.0f;
	float z = halfDims.z*2.0f;
	float a;
	if (x >= y && x >= z) { a = x; }
	else if (y > x && y >= z) { a = y; }
	else { a = z; }

	pnode->SetBoundingRadius(a * sqrt(3.0f) / 2.0f);

	CollisionShape* pColshape = new CuboidCollisionShape(halfDims);
	pnode->SetCollisionShape(pColshape);
	pnode->SetInverseInertia(pColshape->BuildInverseInertia(0.0f));

	this->renderNode = rnode;
	this->physicsNode = pnode;
	RegisterPhysicsToRenderTransformCallback();
	SetPhysics(pnode);

	Physics()->SetOnCollisionCallback(
		std::bind(&LaunchPad::LaunchPadCallbackFunction,
			this,							//Any non-placeholder param will be passed into the function each time it is called
			std::placeholders::_1,			//The placeholders correlate to the expected parameters being passed to the callback
			std::placeholders::_2
		)
	);
}

bool LaunchPad::LaunchPadCallbackFunction(PhysicsNode* self, PhysicsNode* collidingObject) {
	if (collidingObject->GetInverseMass() > 0) {
		collidingObject->SetLinearVelocity(collidingObject->GetLinearVelocity() +  Vector3(0, 5, 0));
		//Vector3 scale = Render()->GetModelScale();
	}
	return true;
}

void LaunchPad::Update(float dt) {
}