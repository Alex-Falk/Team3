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
#pragma once

#include <ncltech\GameObject.h>
#include "GamePlay.h"

class Pickup : public GameObject
{
public:
	Pickup();
	Pickup(Vector3 pos, PickupType type, string unique_name, float respawnTime = 30.0);

	virtual void Update(float dt);
	
	//Example of member callback ('this' parameter is bound at bind time)
	virtual bool PickupCallbackFunction(PhysicsNode* self, PhysicsNode* collidingObject);
	

	virtual void Reactivate();

	PickupType GetPickupType() { return type; }
	bool GetActive() { return active; }
	float GetRespawnTime() { return respawnTime; }
	float GetCurrentRespawnTimer() { return currentRespawnTimer; }

	void SetType(PickupType newType) { type = newType; }
	void SetActive(bool b) { active = b; }
	void SetRespawnTime(float f) { respawnTime = f; }

	~Pickup();

//Name: Nick Bedford
//Date: 07/02/2018
//Changed to protected
protected:
	
	PickupType type;

	bool active;
	float respawnTime;
	float currentRespawnTimer;
};

