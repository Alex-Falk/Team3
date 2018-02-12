#include "Weapons.h"
#include <ncltech\CommonMeshes.h>
#include <ncltech\SphereCollisionShape.h>

GameObject* Weapons::BuildPistol(const Vector4& color, float size, Vector3 pos)
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

GameObject* Weapons::BuildRocket(const Vector4& color, float size, Vector3 pos) {

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

void Weapons::ShootPistol(Vector3 pos, float size, Vector4 colour) {
	GameObject* bullet = Weapons::BuildPistol(colour, size, pos);

	float yaw = GraphicsPipeline::Instance()->GetCamera()->GetYaw();
	float pitch = GraphicsPipeline::Instance()->GetCamera()->GetPitch();
	Vector3 direction;

	direction = Matrix3::Rotation(pitch, Vector3(1, 0, 0)) * Matrix3::Rotation(yaw, Vector3(0, 10, 0)) * Vector3(0, 0, -10) * bulletPower;
	bullet->Physics()->SetLinearVelocity(direction);
//	sendbullet(pos, direction);  //to send bullet on network;

}

void Weapons::ShootRocket(Vector3 pos, float size, Vector4 colour) {
	GameObject* Rocket = Weapons::BuildPistol(colour, size, pos);

	float yaw = GraphicsPipeline::Instance()->GetCamera()->GetYaw();
	float pitch = GraphicsPipeline::Instance()->GetCamera()->GetPitch();
	Vector3 direction;

	direction = Matrix3::Rotation(pitch, Vector3(1, 0, 0)) * Matrix3::Rotation(yaw, Vector3(0, 10, 0)) * Vector3(0, 0, -10) * rocketPower;
	Rocket->Physics()->SetLinearVelocity(direction);
	//	SendRocket(pos, direction);  //to send rocket on network;
}