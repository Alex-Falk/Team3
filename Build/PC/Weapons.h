#pragma once
#include <ncltech\GameObject.h>

#include <ncltech\CommonMeshes.h>
#include <ncltech\SphereCollisionShape.h>

namespace Weapons
{
	GameObject* BuildPistol(const Vector4& color, float size, Vector3 pos)
	{
		float bulletSize = size *0.2;
		//Due to the way SceneNode/RenderNode's were setup, we have to make a dummy node which has the mesh and scaling transform
		// and a parent node that will contain the world transform/physics transform
		RenderNode* rnode = new RenderNode();

		RenderNode* dummy = new RenderNode(CommonMeshes::Sphere(), color);
		dummy->SetTransform(Matrix4::Scale(Vector3(bulletSize, bulletSize, bulletSize)));
		rnode->AddChild(dummy);

		rnode->SetTransform(Matrix4::Translation(pos));
		rnode->SetBoundingRadius(bulletSize);

		PhysicsNode* pnode = NULL;

		pnode = new PhysicsNode();
		pnode->SetPosition(pos);
		pnode->SetInverseMass(1);
		pnode->SetBoundingRadius(bulletSize);


		CollisionShape* pColshape = new SphereCollisionShape(bulletSize);
		pnode->SetCollisionShape(pColshape);
		pnode->SetInverseInertia(pColshape->BuildInverseInertia(1));


		GameObject* obj = new GameObject("Bullet", rnode, pnode);

		return obj;
	}

	GameObject* BuildRocket(const Vector4& color, float size, Vector3 pos) {

		Vector3 halfdims = Vector3(1, 1, 1) * size * 0.3;

		//Due to the way SceneNode/RenderNode's were setup, we have to make a dummy node which has the mesh and scaling transform
		// and a parent node that will contain the world transform/physics transform
		RenderNode* rnode = new RenderNode();

		Mesh* mesh = new Mesh();
		*mesh = *CommonMeshes::Cube();
		//if (tex) { mesh->SetTexture(tex); }

		RenderNode* dummy = new RenderNode(mesh, color);

		dummy->SetTransform(Matrix4::Scale(halfdims));



		rnode->AddChild(dummy);

		rnode->SetTransform(Matrix4::Translation(pos));
		rnode->SetBoundingRadius(halfdims.Length());

		PhysicsNode* pnode = NULL;

		pnode = new PhysicsNode();
		pnode->SetPosition(pos);
		pnode->SetInverseMass(1);
		float x = halfdims.x*2.0f;
		float y = halfdims.y*2.0f;
		float z = halfdims.z*2.0f;
		float a;
		if (x >= y && x >= z) { a = x; }
		else if (y > x && y >= z) { a = y; }
		else { a = z; }

		pnode->SetBoundingRadius(a * sqrt(3.0f) / 2.0f);

		CollisionShape* pColshape = new CuboidCollisionShape(halfdims);
		pnode->SetCollisionShape(pColshape);
		pnode->SetInverseInertia(pColshape->BuildInverseInertia(1));

		GameObject* obj = new GameObject("Rocket", rnode, pnode);

		return obj;
	}


	//GameObject* PAINT_SPRAY();

	//GameObject* AUTO_PAINT_LAUNCHER();


}