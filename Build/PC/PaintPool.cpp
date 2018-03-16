//					 ,.ood888888888888boo.,
//              .od888P^""            ""^Y888bo.
//          .od8P''   ..oood88888888booo.    ``Y8bo.
//       .odP'"  .ood8888888888888888888888boo.  "`Ybo.
//     .d8'   od8'd888888888f`8888't888888888b`8bo   `Yb.
//    d8'  od8^   8888888888[  `'  ]8888888888   ^8bo  `8b
//  .8P  d88'     8888888888P      Y8888888888     `88b  Y8.
// d8' .d8'       `Y88888888'      `88888888P'       `8b. `8b
//.8P .88P            """"            """"            Y88. Y8.
//88  888                 Nick Bedford                 888  88
//88  888                PaintPool Class               888  88
//88  888.        ..       15/02/2018       ..        .888  88
//`8b `88b,     d8888b.od8bo.      .od8bo.d8888b     ,d88' d8'
// Y8. `Y88.    8888888888888b    d8888888888888    .88P' .8P
//  `8b  Y88b.  `88888888888888  88888888888888'  .d88P  d8'
//    Y8.  ^Y88bod8888888888888..8888888888888bod88P^  .8P
//     `Y8.   ^Y888888888888888LS888888888888888P^   .8P'
//       `^Yb.,  `^^Y8888888888888888888888P^^'  ,.dP^'
//          `^Y8b..   ``^^^Y88888888P^^^'    ..d8P^'
//              `^Y888bo.,            ,.od888P^'
//                   "`^^Y888888888888P^^'"  
// Adapted by Alex Falk

#include "PaintPool.h"
#include <string.h>
#include "Avatar.h"
#include <ncltech\GraphicsPipeline.h>
#include "AudioSystem.h"

//I blame Microsoft...
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

PaintPool::PaintPool()
{
	Physics()->SetType(PAINTPOOL_PHYS_NODE);
	soundCooldown = 0;
}

PaintPool::PaintPool(Vector3 pos, Colour colour, string unique_name, float respawnTime)
{
	Vector3 halfdims = Vector3(3, 0.5, 3);

	friendlyName = unique_name;
	this->colour = colour;

	//-Alex Falk----------------------------------------------------------//
	RenderNode* rnode = new RenderNode();
	// Create a 2x2 Patch of quads that will be tesselated
	Mesh * m = Mesh::GeneratePlane(2, 2);
	m->type = GL_PATCHES;
	Vector4 col = EnumToVectorColour(colour);
	col.w = 0.7f;

	// The render node that is to be tesselated
	RenderNode* dummy = new RenderNode(m, "PickUp", col);
	dummy->SetCulling(false);
	dummy->SetTransform(Matrix4::Translation(Vector3(-halfdims.x,0.7f,-halfdims.z)) * Matrix4::Scale(halfdims) * Matrix4::Rotation(angle, Vector3(1, 0, 0)));
	dummy->SetMaterial(GraphicsPipeline::Instance()->GetAllMaterials()[MATERIALTYPE::ColorPool]);
	rnode->AddChild(dummy);

	// Side walls for the paint pool
	RenderNode* dummy2 = new ChangeColorRenderNode(CommonMeshes::Cube(), "lower", EnumToVectorColour(colour));
	dummy2->SetTransform(Matrix4::Translation(Vector3(0.0f, 0.3f, -halfdims.z)) * Matrix4::Scale(Vector3(3.0f,0.6f,0.05f)));
	dummy2->SetMaterial(GraphicsPipeline::Instance()->GetAllMaterials()[MATERIALTYPE::ChangeColorObject]);
	rnode->AddChild(dummy2);

	RenderNode* dummy3 = new ChangeColorRenderNode(CommonMeshes::Cube(), "lower", EnumToVectorColour(colour));
	dummy3->SetTransform(Matrix4::Translation(Vector3(0.0f, 0.3f, halfdims.z)) * Matrix4::Scale(Vector3(3.0f, 0.6f, 0.05f)));
	dummy3->SetMaterial(GraphicsPipeline::Instance()->GetAllMaterials()[MATERIALTYPE::ChangeColorObject]);
	rnode->AddChild(dummy3);

	RenderNode* dummy4 = new ChangeColorRenderNode(CommonMeshes::Cube(), "lower", EnumToVectorColour(colour));
	dummy4->SetTransform(Matrix4::Translation(Vector3(-halfdims.x, 0.3f, 0.0f)) * Matrix4::Scale(Vector3(0.05f, 0.6f, 3.0f)));
	dummy4->SetMaterial(GraphicsPipeline::Instance()->GetAllMaterials()[MATERIALTYPE::ChangeColorObject]);
	rnode->AddChild(dummy4);

	RenderNode* dummy5 = new ChangeColorRenderNode(CommonMeshes::Cube(), "lower", EnumToVectorColour(colour));
	dummy5->SetTransform(Matrix4::Translation(Vector3(halfdims.x, 0.3f, 0.0f)) * Matrix4::Scale(Vector3(0.05f, 0.6f, 3.0f)));
	dummy5->SetMaterial(GraphicsPipeline::Instance()->GetAllMaterials()[MATERIALTYPE::ChangeColorObject]);
	rnode->AddChild(dummy5);

	// Bottom ground of the paintpool - so that the floor cannot be seen
	RenderNode* dummy6 = new ChangeColorRenderNode(CommonMeshes::Cube(), "lower", EnumToVectorColour(colour));
	dummy6->SetTransform(Matrix4::Translation(Vector3(0, 0.4f, 0.0f)) * Matrix4::Scale(Vector3(3.0f, 0.2f, 3.0f)));
	dummy6->SetMaterial(GraphicsPipeline::Instance()->GetAllMaterials()[MATERIALTYPE::ChangeColorObject]);
	rnode->AddChild(dummy6);
//--------------------------------------------------------------------//
	//rnode->SetCulling(true);
	rnode->SetTransform(Matrix4::Translation(pos));
	rnode->SetBoundingRadius(Vector3(3.0f, 0.5f, 3.0f).Length());

	PhysicsNode* pnode = NULL;

	pnode = new PhysicsNode();
	pnode->SetPosition(pos);
	pnode->SetInverseMass(0.0f);
	pnode->SetType(PAINTPOOL_PHYS_NODE);
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
		std::bind(&PaintPool::PickupCallbackFunction,
			this,							//Any non-placeholder param will be passed into the function each time it is called
			std::placeholders::_1,			//The placeholders correlate to the expected parameters being passed to the callback
			std::placeholders::_2
		)
	);

	soundCooldown = 0;
}

bool PaintPool::PickupCallbackFunction(PhysicsNode* self, PhysicsNode* collidingObject)
{
	if (collidingObject->GetType() == PLAYER)
	{
		
		if (this->colour == ((Avatar*)collidingObject->GetParent())->GetColour())
		{
			if (soundCooldown >= AudioSystem::Instance()->GetSoundLength(PAINT_FILL_SOUND) * 0.001f) {
				AudioSystem::Instance()->PlayASound(PAINT_FILL_SOUND, false, collidingObject->GetPosition());
				soundCooldown = 0;
			}
			
			((Avatar*)collidingObject->GetParent())->ChangeLife(1);
		}
		else
		{
			((Avatar*)collidingObject->GetParent())->ChangeLife(-1);
		}

		return false;
	}

	//Return true to enable collision resolution, for Pickup just return false so we can drop the collision pair from the system
	return false;

}

void PaintPool::ChangeColour(Colour newColour)
{
	Vector4 newCol = EnumToVectorColour(newColour);
	newCol.w = 0.7f;

	Render()->GetChild()->SetBaseColor(newCol);

	for (auto itr = Render()->GetChildIteratorStart()+1; itr != Render()->GetChildIteratorEnd(); ++itr)
	{
		static_cast<ChangeColorRenderNode*>((*itr))->StartChangeColor();
		(*itr)->SetBaseColor(EnumToVectorColour(newColour));
		
	}
	colour = newColour;
}

void PaintPool::ChangeSize(Vector3 newSize)
{
	float maxSize = (float)max(max(newSize.x, newSize.y), newSize.z);
	Render()->GetChild()->SetBoundingRadius(maxSize);
	Render()->SetBoundingRadius(maxSize);
	Physics()->SetBoundingRadius(maxSize);

	((CuboidCollisionShape*)Physics()->GetCollisionShape())->SetHalfWidth(newSize.x);
	((CuboidCollisionShape*)Physics()->GetCollisionShape())->SetHalfHeight(newSize.y);
	((CuboidCollisionShape*)Physics()->GetCollisionShape())->SetHalfDepth(newSize.z);

	Render()->GetChild()->SetTransform(Matrix4::Scale(newSize));
}

void PaintPool::Update(float dt)
{
	soundCooldown += dt;
}

PaintPool::~PaintPool()
{
}