#include "PhysicsNode.h"
#include "PhysicsEngine.h"


void PhysicsNode::IntegrateForVelocity(float dt, Integrator integrator)
{
	Integrator i;
	if (this->integrator == ZERO) {i = integrator; }
	else { i = this->integrator; }
	/* TUTORIAL 2 CODE */
	if (invMass > 0.0f) {
		if (type != PROJECTILE) {
			linVelocity += PhysicsEngine::Instance()->GetGravity() * dt;
		}
	}

	switch (i) {
	case RK2:
		linHalfVel = linVelocity + (force * invMass * dt * .5f);
		linHalfVel = linHalfVel * PhysicsEngine::Instance()->GetDampingFactor();

		angHalfVel = angVelocity + (invInertia * torque * dt * .5f);
		angHalfVel = angHalfVel * PhysicsEngine::Instance()->GetDampingFactor();
		break;

	case RK4:
		v1 = linVelocity;
		v2 = v1 + (force * invMass * dt * .5f);
		v3 = v2 + (force * invMass * dt * .5f);
		v4 = v3 + (force * invMass * dt);

		linHalfVel = linVelocity + (v1 + (v2 + v3) * 2.0f + v4) * (1.0f / 6.0f) * dt * PhysicsEngine::Instance()->GetDampingFactor();


		v1 = angVelocity * PhysicsEngine::Instance()->GetDampingFactor();
		v2 = v1 + (invInertia * torque * dt * .5f);
		v3 = v2 + (invInertia * torque * dt * .5f);
		v4 = v3 + (invInertia * torque * dt);

		angHalfVel = (v1 + (v2 + v3) * 2.0f + v4) * (dt / 6.0f);
		angHalfVel = angVelocity + angHalfVel * PhysicsEngine::Instance()->GetDampingFactor();
		break;
	}

	linVelocity += force * invMass * dt;
	linVelocity = linVelocity * PhysicsEngine::Instance()->GetDampingFactor();

	angVelocity += invInertia * torque * dt;
	angVelocity = angVelocity * PhysicsEngine::Instance()->GetDampingFactor();

}





/* Between these two functions the physics engine will solve for velocity
   based on collisions/constraints etc. So we need to integrate velocity, solve 
   constraints, then use final velocity to update position. 
*/

void PhysicsNode::IntegrateForPosition(float dt, Integrator integrator)
{
	/* TUTORIAL 2 CODE */

	Integrator i;
	if (this->integrator == ZERO) { i = integrator; }
	else { i = this->integrator; }

	switch (i) {
	case RK2:
	case RK4:
		position += linHalfVel * dt;
		orientation = orientation + Quaternion(angHalfVel * dt * 0.5f, 0.0f) * orientation;
		break;
	case SYMPLETIC:
		position += linVelocity * dt;
		orientation = orientation + Quaternion(angVelocity * dt * 0.5f, 0.0f) * orientation;
		break;
	}
	
	orientation.Normalise();

	//Finally: Notify any listener's that this PhysicsNode has a new world transform.
	// - This is used by GameObject to set the worldTransform of any RenderNode's. 
	//   Please don't delete this!!!!!
	FireOnUpdateCallback();
}

