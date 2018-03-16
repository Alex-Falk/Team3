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
//88  888                 Weapon Class                 888  88
//88  888.        ..       07/02/2018       ..        .888  88
//`8b `88b,     d8888b.od8bo.      .od8bo.d8888b     ,d88' d8'
// Y8. `Y88.    8888888888888b    d8888888888888    .88P' .8P
//  `8b  Y88b.  `88888888888888  88888888888888'  .d88P  d8'
//    Y8.  ^Y88bod8888888888888..8888888888888bod88P^  .8P
//     `Y8.   ^Y888888888888888LS888888888888888P^   .8P'
//       `^Yb.,  `^^Y8888888888888888888888P^^'  ,.dP^'
//          `^Y8b..   ``^^^Y88888888P^^^'    ..d8P^'
//              `^Y888bo.,            ,.od888P^'
//                   "`^^Y888888888888P^^'"  
#include "WeaponPickup.h"
#include <string.h>
#include "Avatar.h"
#include "AudioSystem.h"

WeaponPickup::WeaponPickup() : Pickup()
{
}

WeaponPickup::WeaponPickup(Vector3 pos, WeaponType type, string unique_name, float respawnTime) : Pickup(pos, WEAPON, unique_name, respawnTime)
{
	friendlyName = "WeaponPickup";
	Physics()->SetName("WeaponPickup");
	weaponType = type;

	Physics()->SetOnCollisionCallback(
		std::bind(&WeaponPickup::PickupCallbackFunction,
			this,							//Any non-placeholder param will be passed into the function each time it is called
			std::placeholders::_1,			//The placeholders correlate to the expected parameters being passed to the callback
			std::placeholders::_2
		)
	);
}

bool WeaponPickup::PickupCallbackFunction(PhysicsNode* self, PhysicsNode* collidingObject)
{
	if (collidingObject->GetType() == PLAYER)
	{
		if (this->active)
		{
			((Avatar*)collidingObject->GetParent())->PickUpBuffActivated(this->type);
			AudioSystem::Instance()->PlayASound(PICKUP_COLLECTED_SOUND, false, physicsNode->GetPosition());
			((Avatar*)collidingObject->GetParent())->SetWeapon(weaponType);
		}

		this->active = false;
	}

	return false;

}

void WeaponPickup::Reactivate()
{
	Render()->SetChildBaseColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	active = true;
	currentRespawnTimer = 0;
	weaponType = static_cast<WeaponType>(rand() % NUM_OF_WEAPONS);
}

WeaponPickup::~WeaponPickup()
{
}