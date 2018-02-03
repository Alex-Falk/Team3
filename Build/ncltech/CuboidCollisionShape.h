/******************************************************************************
Class: CuboidCollisionShape
Implements: CollisionShape
Author: 
	Pieran Marris      <p.marris@newcastle.ac.uk> and YOU!
Description: 

	Extends CollisionShape to represent a cuboid geometric shape. 

	This is an example of physical collision shape that is defined by vertices/faces rather than
	being defined algorithmetrically. When searching for nearest points for collision testing it
	will search through a list of actual vertices/faces.

	To simplify some of the methods such as getting the min/max vertices and acquiring the reference face, this 
	class uses a cuboid 'Hull' object as it's basis. This keeps a list of all vertices, faces and their inter-connectivity,
	allowing easy access to adjacent faces which is required for retrieving the clipping planes later on. You
	can see the Hull class as a more in depth graphical mesh, where as well as the vertices/faces it also keeps
	track of all the adjacency information as well. So vertices know which edges/faces they belong too and each 
	face knows which faces it is adjoined too.

*//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CollisionShape.h"
#include "Hull.h"

class CuboidCollisionShape : public CollisionShape
{
public:
	CuboidCollisionShape();
	CuboidCollisionShape(const Vector3& halfdims);
	virtual ~CuboidCollisionShape();

	// Set Cuboid Dimensions
	void SetHalfWidth(float half_width) { halfDims.x = fabs(half_width); }
	void SetHalfHeight(float half_height) { halfDims.y = fabs(half_height); }
	void SetHalfDepth(float half_depth) { halfDims.z = fabs(half_depth); }

	// Get Cuboid Dimensions
	const Vector3& GetHalfDims() const { return halfDims; }
	float GetHalfWidth()	const { return halfDims.x; }
	float GetHalfHeight()	const { return halfDims.y; }
	float GetHalfDepth()	const { return halfDims.z; }

	// Debug Collision Shape
	virtual void DebugDraw() const override;


	// Build Inertia Matrix for rotational mass
	virtual Matrix3 BuildInverseInertia(float invMass) const override;


	// Generic Collision Detection Routines
	//  - Used in CollisionDetectionSAT to identify if two shapes overlap
	virtual void GetCollisionAxes(
		const PhysicsNode* otherObject,
		std::vector<Vector3>& out_axes) const override;

	virtual Vector3 GetClosestPoint(const Vector3& point) const override;

	virtual void GetMinMaxVertexOnAxis(
		const Vector3& axis,
		Vector3& out_min,
		Vector3& out_max) const override;

	virtual void GetIncidentReferencePolygon(
		const Vector3& axis,
		std::list<Vector3>& out_face,
		Vector3& out_normal,
		std::vector<Plane>& out_adjacent_planes) const override;



protected:
	//Constructs the static cube hull 
	static void ConstructCubeHull();

protected:
	Vector3				 halfDims;
	static Hull			 cubeHull;			//Static cube descriptor, as all cuboid instances will have the same underlying model format ([-1,-1,-1] - [1,1,1] axis aligned cuboid)
}; 

