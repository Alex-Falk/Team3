#include <ncltech\CommonMeshes.h>
#include <ncltech\SphereCollisionShape.h>
#include "Weapons.h"

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
	pnode->SetType(PROJECTILE);

	GameObject* obj = new GameObject("Bullet", rnode, pnode);

	return obj;
}

GameObject* Weapons::BuildRocket(const Vector4& color, float size, Vector3 pos) {

	Vector3 halfdims = Vector3(0.7, 0.7, 1.5) * size * 0.3;

	RenderNode* rnode = new RenderNode();

	Mesh* mesh = new Mesh();
	*mesh = *CommonMeshes::Cube();

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
	pnode->SetType(PROJECTILE);

	GameObject* obj = new GameObject("Rocket", rnode, pnode);
	
	return obj;
}

GameObject * Weapons::BuildPaintSpray(const Vector4& color, float size, Vector3 pos) {
	float bulletSize = size * 0.15;
	RenderNode* rnode = new RenderNode();

	RenderNode* dummy = new RenderNode(CommonMeshes::Sphere(), color);
	dummy->SetTransform(Matrix4::Scale(Vector3(bulletSize, bulletSize, bulletSize)));
	rnode->AddChild(dummy);

	rnode->SetTransform(Matrix4::Translation(pos));
	rnode->SetBoundingRadius(bulletSize);

	PhysicsNode* pnode = NULL;

	pnode = new PhysicsNode();
	pnode->SetPosition(pos);
	pnode->SetInverseMass(10);
	pnode->SetBoundingRadius(bulletSize);


	CollisionShape* pColshape = new SphereCollisionShape(bulletSize);
	pnode->SetCollisionShape(pColshape);
	pnode->SetInverseInertia(pColshape->BuildInverseInertia(1));
	pnode->SetType(PROJECTILE);

	GameObject* obj = new GameObject("Spray", rnode, pnode);

	return obj;
}

vector<GameObject*> Weapons::ShootPistol(Vector3 pos, float size, Vector4 colour) {
	vector<GameObject*> bullet;
	 bullet.push_back(Weapons::BuildPistol(colour, size, pos));

	float yaw = GraphicsPipeline::Instance()->GetCamera()->GetYaw();
	float pitch = GraphicsPipeline::Instance()->GetCamera()->GetPitch();
	Vector3 direction;

	direction = Matrix3::Rotation(pitch, Vector3(1, 0, 0)) * Matrix3::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * bulletPower;
	bullet[0]->Physics()->SetLinearVelocity(direction);
//	sendbullet(pos, direction);  //to send bullet on network;

	return bullet;
}

vector<GameObject*> Weapons::ShootRocket(Vector3 pos, float size, Vector4 colour) {
	vector<GameObject*> rocket;
	rocket.push_back( Weapons::BuildRocket(colour, size, pos));

	float yaw = GraphicsPipeline::Instance()->GetCamera()->GetYaw();
	float pitch = GraphicsPipeline::Instance()->GetCamera()->GetPitch();
	Vector3 direction;

	direction = Matrix3::Rotation(pitch, Vector3(1, 0, 0)) * Matrix3::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * rocketPower;
	rocket[0]->Physics()->SetLinearVelocity(direction);
	rocket[0]->Physics()->SetOrientation(Quaternion::EulerAnglesToQuaternion(pitch, yaw, 0));
	//	SendRocket(pos, direction);  //to send rocket on network;

	return rocket;
}

vector<GameObject*> Weapons::ShootPaintSpray(Vector3 pos, float size, Vector4 colour) {
	vector<GameObject*> spray;

	for (int i = 0; i < 15; i++) {
		int randPitch = rand() % 180 + -90;
		int randYaw = rand() % 360;
		spray.push_back( Weapons::BuildPaintSpray(colour, size, pos));

		Vector3 direction = Matrix3::Rotation(randPitch, Vector3(1, 0, 0)) * Matrix3::Rotation(randYaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1)*sprayPower;
		spray[i]->Physics()->SetLinearVelocity(direction * 15);
		spray[i]->Physics()->SetPosition(pos + direction * size);
		//SceneManager::Instance()->GetCurrentScene()->AddGameObject(spray);
	}

	return spray;
}