#include "SphereCollisionShape.h"
#include "PhysicsNode.h"
#include <nclgl\NCLDebug.h>
#include <nclgl\Matrix3.h>
#include <nclgl\Vector3.h>


SphereCollisionShape::SphereCollisionShape()
{
	m_Radius = 1.0f;
}

SphereCollisionShape::SphereCollisionShape(float radius)
{
	m_Radius = radius;
}

SphereCollisionShape::~SphereCollisionShape()
{

}

Matrix3 SphereCollisionShape::BuildInverseInertia(float invMass) const
{
	//https://en.wikipedia.org/wiki/List_of_moments_of_inertia
	float i = 2.5f * invMass / (m_Radius * m_Radius); //SOLID
	//float i = 1.5f * invMass / (m_Radius * m_Radius); //HOLLOW

	Matrix3 inertia;
	inertia._11 = i;
	inertia._22 = i;
	inertia._33 = i;

	return inertia;
}


//TUTORIAL 4 CODE
void SphereCollisionShape::GetCollisionAxes(const PhysicsNode* otherObject, std::vector<Vector3>& out_axes) const
{
	/* There are infinite possible axes on a sphere so we MUST handle it seperately
		- Luckily we can just get the closest point on the opposite object to our centre and use that.
	*/
	Vector3 dir = (otherObject->GetPosition() - Parent()->GetPosition()).Normalise();

	Vector3 p1 = Parent()->GetPosition();
	Vector3 p2 = otherObject->GetCollisionShape()->GetClosestPoint(p1);

	out_axes.push_back((p1 - p2).Normalise());
}

Vector3 SphereCollisionShape::GetClosestPoint(const Vector3& point) const
{
	Vector3 diff = (point - Parent()->GetPosition()).Normalise();
	return Parent()->GetPosition() + diff * m_Radius;
}

void SphereCollisionShape::GetMinMaxVertexOnAxis(
	const Vector3& axis,
	Vector3& out_min,
	Vector3& out_max) const
{
	out_min = Parent()->GetPosition() - axis * m_Radius;
	out_max = Parent()->GetPosition() + axis * m_Radius;
}
//-------------


void SphereCollisionShape::GetIncidentReferencePolygon(
	const Vector3& axis,
	std::list<Vector3>& out_face,
	Vector3& out_normal,
	std::vector<Plane>& out_adjacent_planes) const
{
	//This is used in Tutorial 5
	out_face.push_back(Parent()->GetPosition() + axis * m_Radius);
	out_normal = axis;	
}

void SphereCollisionShape::DebugDraw() const
{
	Vector3 pos = Parent()->GetPosition();

	//Draw Filled Circle
	NCLDebug::DrawPointNDT(pos, m_Radius, Vector4(1.0f, 1.0f, 1.0f, 0.2f));

	//Draw Perimeter Axes
	Vector3 lastX = pos + Vector3(0.0f, 1.0f, 0.0f) * m_Radius;
	Vector3 lastY = pos + Vector3(1.0f, 0.0f, 0.0f) * m_Radius;
	Vector3 lastZ = pos + Vector3(1.0f, 0.0f, 0.0f) * m_Radius;
	const int nSubdivisions = 20;
	for (int itr = 1; itr <= nSubdivisions; ++itr)
	{
		float angle = itr / float(nSubdivisions) * PI * 2.f;
		float alpha = cosf(angle) * m_Radius;
		float beta = sinf(angle) * m_Radius;

		Vector3 newX = pos + Vector3(0.0f, alpha, beta);
		Vector3 newY = pos + Vector3(alpha, 0.0f, beta);
		Vector3 newZ = pos + Vector3(alpha, beta, 0.0f);

		NCLDebug::DrawThickLineNDT(lastX, newX, 0.02f, Vector4(1.0f, 0.3f, 1.0f, 1.0f));
		NCLDebug::DrawThickLineNDT(lastY, newY, 0.02f, Vector4(1.0f, 0.3f, 1.0f, 1.0f));
		NCLDebug::DrawThickLineNDT(lastZ, newZ, 0.02f, Vector4(1.0f, 0.3f, 1.0f, 1.0f));

		lastX = newX;
		lastY = newY;
		lastZ = newZ;
	}
}