//Nikos Fragkas
//Date 01/03
//Paint Pool that will have your colour as soon as you capture the objects arround

#include "MultiPaintPool.h"
#include "Avatar.h"

MultiPaintPool::MultiPaintPool() : Pickup() {}


MultiPaintPool::MultiPaintPool(Vector3 pos, string unique_name, Colour colour, float respawnTime) : Pickup(pos, PAINTPOOL, unique_name, respawnTime)
{
	ChangeColour(colour);
	ChangeSize({ 3, 2.5, 3 });

	Physics()->SetOnCollisionCallback(
		std::bind(&MultiPaintPool::PickupCallbackFunction,
			this,							//Any non-placeholder param will be passed into the function each time it is called
			std::placeholders::_1,			//The placeholders correlate to the expected parameters being passed to the callback
			std::placeholders::_2
		)
	);
}

bool MultiPaintPool::PickupCallbackFunction(PhysicsNode* self, PhysicsNode* collidingObject)
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


void MultiPaintPool::ChangeColour(Colour newColour)
{
	Vector4 paintColour;

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

	Render()->SetChildBaseColor(paintColour);

	colour = newColour;
}

void MultiPaintPool::ChangeSize(Vector3 newSize)
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


void MultiPaintPool::UpdateMultiColourToPool() {

	if (poolCaptArea.size() > 1) {
		for (uint i = 1; i < poolCaptArea.size(); i++)
		{
			if (poolCaptArea[i]->GetColour() != poolCaptArea[i - 1]->GetColour())
				return;
		}
		ChangeColour(poolCaptArea[1]->GetColour());
	}
	else if (poolCaptArea.size() == 1) {
		ChangeColour(poolCaptArea[0]->GetColour());
	}
}

MultiPaintPool::~MultiPaintPool()
{
}