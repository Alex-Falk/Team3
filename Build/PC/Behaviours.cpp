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
//88  888             Behaviours Namespace             888  88
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
#include "Behaviours.h"
using namespace Behaviours;

Vector3 Behaviours::Seek(Vector3 targetPos, Vector3 currentPos, Vector3 currentVelocity, bool grounded, float weight, float maxMagnitude)
{
	// Applies wall avoid accelleration
	Vector3 behaviourAccn(0,0,0); // = WallAvoid(currentPos, currentVelocity);

	// If no wall avoidance was needed
	if (grounded && currentVelocity.LengthSQ() < 2)
	{
		// Create a vector to the target point 
		Vector3 desiredDirection = (targetPos - currentPos).Normalise();

		behaviourAccn = (desiredDirection * maxMagnitude) + Vector3(0.0f,8.0f,0.0f);
	}

	return behaviourAccn * weight;
}

Vector3 Behaviours::Flee(Vector3 targetPos, Vector3 currentPos, Vector3 currentVelocity, bool grounded, float weight, float maxMagnitude)
{
	Seek(currentPos, targetPos, currentVelocity, grounded, weight, maxMagnitude);
}

Vector3 Behaviours::Pursue(Vector3 targetPos, Vector3 targetVelocity, Vector3 currentPos, Vector3 currentVelocity, bool grounded, float weight, float maxMagnitude)
{
	// Calculates the distance from the target and the time taken to reach it at max speed
	double distance = (targetPos - currentPos).Length();
	double time = distance / targetVelocity.Length();

	// Creates a vector to the target interception point
	Vector3 target = targetPos + targetVelocity * (float)time;

	// Seeks to the target interception point
	return Seek(target, currentPos, currentVelocity, grounded, weight, maxMagnitude);
}


Vector3 Behaviours::Evade(Vector3 targetPos, Vector3 targetVelocity, Vector3 currentPos, Vector3 currentVelocity, bool grounded, float weight, float maxMagnitude)
{
	//// Calculates the distance from the target and the time taken to reach it at max speed
	//double distance = (targetPos - currentPos).magnitude();
	//double time = distance / MAXBOTSPEED;

	//// Creates a vector to the target interception point
	//Vector3 target = targetPos + targetVelocity * (float)time;

	//// Flees from the target interception point
	//return Flee(target, currentPos, currentVelocity, weight);
}

Vector3 Behaviours::WallAvoid(Vector3 currentPos, Vector3 currentVelocity, float weight)
{
	//// Circle used to check wall collisions
	//Circle2D collisionCircle(currentPos, 55.0f);

	//// If the circle collides with a wall
	//if (StaticMap::GetInstance()->IsInsideBlock(collisionCircle))
	//{
	//	//Creates a vector away from the wall
	//	Vector3 MoveAwayVector = (2.5f * StaticMap::GetInstance()->GetNormalToSurface(collisionCircle, false));
	//	Renderer::GetInstance()->DrawLine(currentPos, currentPos + MoveAwayVector, 6);
	//	return MoveAwayVector * weight;
	//}

	//// If no wall collision occurs
	//else
	//{
	//	return Vector3(0.0f, 0.0f);
	//}

}

