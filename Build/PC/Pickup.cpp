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
#include <string.h>

Pickup::Pickup()
{
	active = true;
	respawnTime = 30.0;
	currentRespawnTimer = 0.0;
	object = CommonUtils::BuildCuboidObject("Pickup",
		Vector3(0.0f, 1.0f, 0.0f),
		Vector3(0.5f, 0.5f, 0.5f),
		true,									//Has Physics Object
		0.0f,
		true,									//Has Collision Shape
		false,									//Dragable by the user
		PICKUP,
		Vector4(0.0f, 1.0f, 0.0f, 1.0f));	        //Color
}

Pickup::Pickup(Vector3 pos, PickupType type, float respawnTime)
{
	active = true;
	this->respawnTime = respawnTime;
	currentRespawnTimer = 0.0;

	object = CommonUtils::BuildCuboidObject("Pickup",
		Vector3(0.0f, 1.0, 0.0f) + pos,
		Vector3(0.5f, 0.5f, 0.5f),				//Radius
		true,									//Has Physics Object
		0.0f,
		true,									//Has Collision Shape
		false,									//Dragable by the user
		PICKUP,
		Vector4(0.0f, 1.0f, 0.0f, 1.0f));		//Colour

	object->Physics()->SetOnCollisionCallback(
		std::bind(&Pickup::PickupCallbackFunction,
			this,							//Any non-placeholder param will be passed into the function each time it is called
			std::placeholders::_1,			//The placeholders correlate to the expected parameters being passed to the callback
			std::placeholders::_2
		)
	);
}

void Pickup::Reactivate()
{
	object->Render()->SetChildBaseColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	active = true;
	currentRespawnTimer = 0;
}

void Pickup::Update(float dt)
{
	if (!active)
	{
		object->Render()->SetChildBaseColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		
		object->Physics()->SetAngularVelocity(Vector3(0.1f, 0.4f, 0.3f));

		currentRespawnTimer += dt;
		if (currentRespawnTimer >= respawnTime)
		{
			Reactivate();
		}
	}
	else
	{
		object->Physics()->SetAngularVelocity(Vector3(0.3f, 1.2f, 0.9f));
	}
}

Pickup::~Pickup()
{
}