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
//88  888             Capture Area Class               888  88
//88  888.        ..       16/02/2018       ..        .888  88
//`8b `88b,     d8888b.od8bo.      .od8bo.d8888b     ,d88' d8'
// Y8. `Y88.    8888888888888b    d8888888888888    .88P' .8P
//  `8b  Y88b.  `88888888888888  88888888888888'  .d88P  d8'
//    Y8.  ^Y88bod8888888888888..8888888888888bod88P^  .8P
//     `Y8.   ^Y888888888888888LS888888888888888P^   .8P'
//       `^Yb.,  `^^Y8888888888888888888888P^^'  ,.dP^'
//          `^Y8b..   ``^^^Y88888888P^^^'    ..d8P^'
//              `^Y888bo.,            ,.od888P^'
//                   "`^^Y888888888888P^^'"     

#include "CaptureArea.h"
#include <ncltech\CuboidCollisionShape.h>
#include <string.h>
#include "Avatar.h"
#include "Projectile.h"

CaptureArea::CaptureArea()
{
	CaptureArea({ 0,0,0 },"CAPTUREAREA");
}

CaptureArea::CaptureArea(Vector3 posit, string unique_name, Vector3 halfdims, int scoreValue, Colour colour)
{
	Vector3 pos = Vector3(posit.x + 0.0f, posit.y + 2.0f, posit.z + 0.0f);
	friendlyName = unique_name;
	Vector4 paintColour;

	switch (colour)
	{
	case RED:
		paintColour = RED_COLOUR;
		break;

	case GREEN:
		paintColour = GREEN_COLOUR;
		break;

	case BLUE:
		paintColour = BLUE_COLOUR;
		break;

	case PINK:
		paintColour = PINK_COLOUR;
		break;

	case START_COLOUR:
		paintColour = DEFAULT_COLOUR;
		break;

	default:
		paintColour = DEFAULT_COLOUR;
		break;
	}

	this->colour = colour;

	RenderNode* rnode = new RenderNode();
	RenderNode* dummy = new RenderNode(CommonMeshes::Cube(), paintColour);

	dummy->SetTransform(Matrix4::Scale(halfdims));

	dummy->SetMaterial(GraphicsPipeline::Instance()->GetAllMaterials()[MATERIALTYPE::Forward_Lighting]);

	rnode->AddChild(dummy);

	rnode->SetTransform(Matrix4::Translation(pos));
	rnode->SetBoundingRadius(halfdims.Length());

	PhysicsNode* pnode = NULL;

	pnode = new PhysicsNode();
	pnode->SetPosition(pos);
	pnode->SetInverseMass(0.0f);
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

	this->friendlyName = "CaptureArea";
	this->renderNode = rnode;
	this->physicsNode = pnode;
	RegisterPhysicsToRenderTransformCallback();
	SetPhysics(pnode);
	pnode->SetName("CaptureArea");

	Physics()->SetOnCollisionCallback(
		std::bind(&CaptureArea::CaptureAreaCallbackFunction,
			this,							//Any non-placeholder param will be passed into the function each time it is called
			std::placeholders::_1,			//The placeholders correlate to the expected parameters being passed to the callback
			std::placeholders::_2
		)
	);
}

void CaptureArea::SetColour(Colour c)
{
	colour = c; 

	Vector4 paintColour;

	switch (colour)
	{
	case RED:
		paintColour = RED_COLOUR;
		break;

	case GREEN:
		paintColour = GREEN_COLOUR;
		break;

	case BLUE:
		paintColour = BLUE_COLOUR;
		break;

	case PINK:
		paintColour = PINK_COLOUR;
		break;

	case START_COLOUR:
		paintColour = DEFAULT_COLOUR;
		break;

	default:
		paintColour = DEFAULT_COLOUR;
		break;
	}

	Render()->SetChildBaseColor(paintColour);
}


bool CaptureArea::CaptureAreaCallbackFunction(PhysicsNode* self, PhysicsNode* collidingObject)
{
	if (collidingObject->GetType() == PLAYER)
	{
		this->SetColour(((Avatar*)collidingObject->GetParent())->GetColour());
	}
	else if (collidingObject->GetType() == PROJECTILE || collidingObject->GetType() == SPRAY)
	{
		this->SetColour(((Projectile*)collidingObject->GetParent())->GetColour());
	}

	//Return true to enable collision resolution
	return true;

}

void CaptureArea::ChangeSize(Vector3 newSize)
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

CaptureArea::~CaptureArea()
{
}