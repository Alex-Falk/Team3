/******************************************************************************
Class: PhysicsNode
Implements:
Author: 
	Author: Pieran Marris      <p.marris@newcastle.ac.uk> and YOU!
Description: 

	A class to store all of the properties that define a physical object in the world.

	This class shouldn't need to be changed for the course of the physics tutorials, though
	will be used extensively. 

	As we go through the physics tutorials, we will be adding more and more of these parameters into
	our physics engine and processing them as required. 

	The first ones (and all that is needed to give an object movement) we will use are:
		<---------LINEAR-------------->
		Vector3		position;						
		Vector3		linVelocity;
		Vector3		force;
		float		invMass;

		<----------ANGULAR-------------->
		Quaternion  orientation;
		Vector3		angVelocity;
		Vector3		torque;
		Matrix3     invInertia;


*//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <nclgl\Quaternion.h>
#include <nclgl\Matrix3.h>
#include "CollisionShape.h"
#include <functional>

enum Integrator { ZERO, SYMPLETIC, RK2, RK4 };

//Nick Bedford
//Date: 12/02/2018
//Added an enum to sort nodes
enum PhysNodeType {DEFAULT_PHYSICS, PROJECTILE, SPRAY, BIG_NODE, INVISIBLE_WALL, PICKUP, PLAYER, MINION, PAINTABLE_OBJECT};

class PhysicsNode;

//Callback function called whenever a collision is detected between two objects
//Params:
//	PhysicsNode* this_obj			- The current object class that contains the callback
//	PhysicsNode* colliding_obj	- The object that is colliding with the given object
//Return:
//  True	- The physics engine should process the collision as normal
//	False	- The physics engine should drop the collision pair and not do any further collision resolution/manifold generation
//			  > This can be useful for AI to see if a player/agent is inside an area/collision volume
typedef std::function<bool(PhysicsNode* this_obj, PhysicsNode* colliding_obj)> PhysicsCollisionCallback;


//Callback function called whenever this physicsnode's world transform is updated
//Params:
//	const Matrix4& transform - New World transform of the physics node
typedef std::function<void(const Matrix4& transform)> PhysicsUpdateCallback;


class GameObject;
class PhysicsNode
{
public:
	PhysicsNode(PhysNodeType physType = DEFAULT_PHYSICS, Integrator i = ZERO)
		: position(0.0f, 0.0f, 0.0f)
		, linVelocity(0.0f, 0.0f, 0.0f)
		, force(0.0f, 0.0f, 0.0f)
		, invMass(0.0f)
		, orientation(0.0f, 0.0f, 0.0f, 1.0f)
		, angVelocity(0.0f, 0.0f, 0.0f)
		, torque(0.0f, 0.0f, 0.0f)
		, invInertia(Matrix3::ZeroMatrix)
		, collisionShape(NULL)
		, friction(0.5f)
		, elasticity(0.9f)
	{
		integrator = i;
		type = physType;
	}

	virtual ~PhysicsNode()
	{
		SAFE_DELETE(collisionShape);
	}


	//<-------- Integration --------->
	// Called automatically by PhysicsEngine on all physics nodes each frame
	void IntegrateForVelocity(float dt, Integrator integrator);
	//<-- Between calling these two functions the physics engine will solve velocity to get 'true' final velocity -->
	void IntegrateForPosition(float dt, Integrator integrator);


	//<--------- GETTERS ------------->
	inline GameObject*			GetParent()					const { return parent; }

	inline PhysNodeType			GetType()					const { return type; }

	inline float				GetElasticity()				const { return elasticity; }
	inline float				GetFriction()				const { return friction; }

	inline const Vector3&		GetPosition()				const { return position; }
	inline const Vector3&		GetLinearVelocity()			const { return linVelocity; }
	inline const Vector3&		GetForce()					const { return force; }
	inline float				GetInverseMass()			const { return invMass; }
	inline const Vector3		GetAcceleration()			const { return (force*invMass); }

	inline const Quaternion&	GetOrientation()			const { return orientation; }
	inline const Vector3&		GetAngularVelocity()		const { return angVelocity; }
	inline const Vector3&		GetTorque()					const { return torque; }
	inline const Matrix3&		GetInverseInertia()			const { return invInertia; }

	inline const float			GetBoundingRadius()			const { return boundingRadius; }

	inline CollisionShape*		GetCollisionShape()			const { return collisionShape; }

	const Matrix4&				GetWorldSpaceTransform()    const { return worldTransform; }

	inline bool					IsGood()					const { return isGood; }

	inline std::string			getName()					const { return friendly_name; }

	


	//<--------- SETTERS ------------->
	inline void SetParent(GameObject* obj)							{ parent = obj; }

	inline void SetElasticity(float elasticityCoeff)				{ elasticity = elasticityCoeff; }
	inline void SetFriction(float frictionCoeff)					{ friction = frictionCoeff; }

	inline void SetPosition(const Vector3& v)						{ position = v; FireOnUpdateCallback(); }
	inline void SetLinearVelocity(const Vector3& v)					{ linVelocity = v; }
	inline void SetForce(const Vector3& v)							{ force = v; }
	inline void SetInverseMass(const float& v)						{ invMass = v; }
	inline void SetAcceleration(const Vector3& v)					{ force = v / invMass; }

	inline void SetOrientation(const Quaternion& v)					{ orientation = v; FireOnUpdateCallback(); }
	inline void SetAngularVelocity(const Vector3& v)				{ angVelocity = v; }
	inline void SetTorque(const Vector3& v)							{ torque = v; }
	inline void SetInverseInertia(const Matrix3& v)					{ invInertia = v; }

	inline void	SetType(PhysNodeType newType)						{ type = newType; }

	inline void SetBoundingRadius(const float r)					{ boundingRadius = r; }

	inline void SetIntegrator(Integrator i)							{ integrator = i; }

	inline void SetGood(bool b)										{ isGood = b; }

	inline void SetName(std::string name)							{ friendly_name = name; }

	inline void SetCollisionShape(CollisionShape* colShape)
	{ 
		if (collisionShape) collisionShape->SetParent(NULL);
		collisionShape = colShape;
		if (collisionShape) collisionShape->SetParent(this);
	}

	//<---------- CALLBACKS ------------>
	inline void SetOnCollisionCallback(PhysicsCollisionCallback callback) { onCollisionCallback = callback; }
	inline bool FireOnCollisionEvent(PhysicsNode* obj_a, PhysicsNode* obj_b)
	{
		return (onCollisionCallback) ? onCollisionCallback(obj_a, obj_b) : true;
	}

	inline void SetOnUpdateCallback(PhysicsUpdateCallback callback) { onUpdateCallback = callback; }
	inline void FireOnUpdateCallback()
	{
		//Build world transform
		worldTransform = orientation.ToMatrix4();
		worldTransform.SetPositionVector(position);
			
		//Fire the OnUpdateCallback, notifying GameObject's and other potential
		// listeners that this PhysicsNode has a new world transform.
		if (onUpdateCallback) onUpdateCallback(worldTransform);
	}
	

protected:
	//Useful parameters
	PhysNodeType			type;
	GameObject*				parent;
	Matrix4					worldTransform;
	PhysicsUpdateCallback	onUpdateCallback;

	Integrator				integrator = ZERO;

	Vector3					v1;
	Vector3					v2;
	Vector3					v3;
	Vector3					v4;

//Added in Tutorial 2
	//<---------LINEAR-------------->
	Vector3		position;
	Vector3		linVelocity;
	Vector3		linHalfVel;
	Vector3		force;
	float		invMass;

	//<----------ANGULAR-------------->
	Quaternion  orientation;
	Vector3		angVelocity;
	Vector3		angHalfVel;
	Vector3		torque;
	Matrix3     invInertia;

//Added in Tutorial 4/5
	//<----------COLLISION------------>
	CollisionShape*				collisionShape;
	PhysicsCollisionCallback	onCollisionCallback;

	float		boundingRadius = 2;
//Added in Tutorial 5
	//<--------MATERIAL-------------->
	float				elasticity;		///Value from 0-1 definiing how much the object bounces off other objects
	float				friction;		///Value from 0-1 defining how much the object can slide off other objects


	bool				isGood = false;

	std::string			friendly_name;
};