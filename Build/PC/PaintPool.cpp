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
#include "PaintPool.h"
#include <string.h>
#include "Avatar.h"
#include <ncltech\GraphicsPipeline.h>
//I blame Microsoft...
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

PaintPool::PaintPool() : Pickup()
{
}

PaintPool::PaintPool(Vector3 pos, Colour colour, string unique_name, float respawnTime) : Pickup(pos, PAINTPOOL, unique_name, respawnTime)
{
	ChangeColour(colour);
	ChangeSize({3, 0.5, 3});
	
	Physics()->SetOnCollisionCallback(
		std::bind(&PaintPool::PickupCallbackFunction,
			this,							//Any non-placeholder param will be passed into the function each time it is called
			std::placeholders::_1,			//The placeholders correlate to the expected parameters being passed to the callback
			std::placeholders::_2
		)
	);

	Render()->GetChild()->SetMaterial(GraphicsPipeline::Instance()->GetAllMaterials()[MATERIALTYPE::ColorPool]);
}

bool PaintPool::PickupCallbackFunction(PhysicsNode* self, PhysicsNode* collidingObject)
{
	if (collidingObject->GetType() == PLAYER)
	{
		if (this->colour == ((Avatar*)collidingObject->GetParent())->GetColour())
		{
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
	Render()->SetChildBaseColor(EnumToVectorColour(newColour));

	switch (newColour)
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
	paintColour.w = 0.5f;
	Render()->SetChildBaseColor(paintColour);

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
}

PaintPool::~PaintPool()
{
}