//					,.ood888888888888boo.,
//              .od888P^""            ""^Y888bo.
//          .od8P''   ..oood88888888booo.    ``Y8bo.
//       .odP'"  .ood8888888888888888888888boo.  "`Ybo.
//     .d8'   od8'd888888888f`8888't888888888b`8bo   `Yb.
//    d8'  od8^   8888888888[  `'  ]8888888888   ^8bo  `8b
//  .8P  d88'     8888888888P      Y8888888888     `88b  Y8.
// d8' .d8'       `Y88888888'      `88888888P'       `8b. `8b
//.8P .88P            """"            """"            Y88. Y8.
//88  888                 Nick Bedford                 888  88
//88  888                 Pickup Class                 888  88
//88  888.        ..       06/02/2018       ..        .888  88
//`8b `88b,     d8888b.od8bo.      .od8bo.d8888b     ,d88' d8'
// Y8. `Y88.    8888888888888b    d8888888888888    .88P' .8P
//  `8b  Y88b.  `88888888888888  88888888888888'  .d88P  d8'
//    Y8.  ^Y88bod8888888888888..8888888888888bod88P^  .8P
//     `Y8.   ^Y888888888888888LS888888888888888P^   .8P'
//       `^Yb.,  `^^Y8888888888888888888888P^^'  ,.dP^'
//          `^Y8b..   ``^^^Y88888888P^^^'    ..d8P^'
//              `^Y888bo.,            ,.od888P^'
//                   "`^^Y888888888888P^^'"      


#include "Pickup.h"
#include <ncltech\CuboidCollisionShape.h>
#include <string.h>
#include "Avatar.h"

Pickup::Pickup()
{
	active = true;
	respawnTime = 30.0;
	currentRespawnTimer = 0.0;

	Vector3 halfdims(0.5f, 0.5f, 0.5f);
	
	RenderNode* rnode = new RenderNode();
	RenderNode* dummy = new RenderNode(CommonMeshes::Cube(), "Pickup", Vector4(0.0f, 1.0f, 0.0f, 1.0f));

	dummy->SetTransform(Matrix4::Scale(halfdims));

	dummy->SetMaterial(GraphicsPipeline::Instance()->GetAllMaterials()[MATERIALTYPE::Forward_Lighting]);

	rnode->AddChild(dummy);

	rnode->SetTransform(Matrix4::Translation({0,0,0}));
	rnode->SetBoundingRadius(Vector3(3.0f, 0.5f, 3.0f).Length());

	PhysicsNode* pnode = NULL;
	
	pnode = new PhysicsNode();
	pnode->SetPosition({ 0,0,0 });
	pnode->SetInverseMass(0.0f);
	pnode->SetType(PICKUP);

	float x = halfdims.x*2.0f;
	float y = halfdims.y*2.0f;
	float z = halfdims.z*2.0f;
	float a;
	if (x >= y && x >= z) { a = x; }
	else if (y > x && y >= z) { a = y; }
	else { a = z; }

	pnode->SetBoundingRadius(a * sqrt(3.0f) / 2.0f);

	CollisionShape* pColshape = new CuboidCollisionShape(halfdims);
	pnode->SetCollisionShape(pColshape);
	pnode->SetInverseInertia(pColshape->BuildInverseInertia(0.0f));
	
	Physics()->SetOnCollisionCallback(
		std::bind(&Pickup::PickupCallbackFunction,
			this,							//Any non-placeholder param will be passed into the function each time it is called
			std::placeholders::_1,			//The placeholders correlate to the expected parameters being passed to the callback
			std::placeholders::_2
		)
	);

}

Pickup::Pickup(Vector3 pos, PickupType type, string unique_name, float respawnTime)
{
	active = true;
	this->respawnTime = respawnTime;
	currentRespawnTimer = 0.0;

	this->type = type;

	active = true;
	respawnTime = 30.0;
	currentRespawnTimer = 0.0;

	Vector3 halfdims(0.5f, 0.5f, 0.5f);

	friendlyName = unique_name;

	RenderNode* rnode = new RenderNode();
	RenderNode* dummy = new RenderNode(CommonMeshes::Cube(), "PickUp", Vector4(0.0f, 1.0f, 0.0f, 1.0f));

	dummy->SetTransform(Matrix4::Scale(halfdims));

	dummy->SetMaterial(GraphicsPipeline::Instance()->GetAllMaterials()[MATERIALTYPE::Forward_Lighting]);

	rnode->AddChild(dummy);

	rnode->SetTransform(Matrix4::Translation(pos));
	rnode->SetBoundingRadius(Vector3(3.0f, 0.5f, 3.0f).Length());

	PhysicsNode* pnode = NULL;

	pnode = new PhysicsNode();
	pnode->SetPosition(pos);
	pnode->SetInverseMass(0.0f);
	pnode->SetType(PICKUP);
	pnode->SetName(unique_name);

	float x = halfdims.x*2.0f;
	float y = halfdims.y*2.0f;
	float z = halfdims.z*2.0f;
	float a;
	if (x >= y && x >= z) { a = x; }
	else if (y > x && y >= z) { a = y; }
	else { a = z; }

	pnode->SetBoundingRadius(a * sqrt(3.0f) / 2.0f);

	CollisionShape* pColshape = new CuboidCollisionShape(halfdims);
	pnode->SetCollisionShape(pColshape);
	pnode->SetInverseInertia(pColshape->BuildInverseInertia(0.0f));

	this->renderNode = rnode;
	this->physicsNode = pnode;
	RegisterPhysicsToRenderTransformCallback();
	SetPhysics(pnode);

	Physics()->SetOnCollisionCallback(
		std::bind(&Pickup::PickupCallbackFunction,
			this,							//Any non-placeholder param will be passed into the function each time it is called
			std::placeholders::_1,			//The placeholders correlate to the expected parameters being passed to the callback
			std::placeholders::_2
		)
	);
}

void Pickup::Reactivate()
{
	Render()->SetChildBaseColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	active = true;
	currentRespawnTimer = 0;
}

void Pickup::Update(float dt)
{
	if (!active)
	{
		Render()->SetChildBaseColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		
		Physics()->SetAngularVelocity(Vector3(0.1f, 0.4f, 0.3f));

		currentRespawnTimer += dt;
		if (currentRespawnTimer >= respawnTime)
		{
			Reactivate();
		}
	}
	else
	{
		Physics()->SetAngularVelocity(Vector3(0.3f, 1.2f, 0.9f));
	}
}

bool Pickup::PickupCallbackFunction(PhysicsNode* self, PhysicsNode* collidingObject)
{
	return false;
}

Pickup::~Pickup()
{
}