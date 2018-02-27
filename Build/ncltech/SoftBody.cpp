#include "SoftBody.h"

SoftBody::SoftBody(int w, int h, float s, Vector3 pos,GLuint tex)
{
	this->w = w;
	this->h = h;
	this->pos = new Vector3(pos);
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {			
			PhysicsNode * pnode = new PhysicsNode();
			pnode->SetPosition(Vector3(pos.x + (s*j), pos.y + (s*i), pos.z));
			
			pnode->SetInverseMass(1.0f*4.0f);
			
			if (i == h -1 && j == 0 || (i == h - 1 && j == w - 1)) {
				pnode->SetInverseMass(0.0f);
			}
			
			CollisionShape* pColshape = new SphereCollisionShape(0.7f*s);
			pnode->SetCollisionShape(pColshape);
			physicsnodes.push_back(pnode);

		}
	}

	RenderNode* rnode = new RenderNode();

	m = Mesh::GenerateMesh(w - 1, h - 1, s);
	//m->SetTexture(tex);
	RenderNode* dummy = new RenderNode(m, "SoftBody",Vector4(1, 0, 0, 1));
	//dummy->SetTransform(Matrix4::Scale(Vector3(0.1,0.1,0.1)));
	rnode->AddChild(dummy);
	dummy->SetCulling(false);

	rnode->SetTransform(Matrix4::Translation(Vector3(pos.x, pos.y, pos.z)));
	rnode->SetBoundingRadius(10);

	mgo = new MultiGameObject("softBody", rnode, physicsnodes,this->pos);
	mgo->SetSiblingsCollide(false);
	mgo->RotateObject(Vector3(0, 1, 0), -90);

	ScreenPicker::Instance()->RegisterNodeForMouseCallback(
		dummy, //Dummy is the rendernode that actually contains the drawable mesh, and the one we can to 'drag'
		std::bind(&CommonUtils::DragableObjectCallback, mgo, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)
	);

	physicsnodes[0]->SetOnUpdateCallback(
		std::bind(
			&SoftBody::UpdateMesh,
			this, 
			std::placeholders::_1)
	);

	 //Adding Constraints to Each of the physicsnodes
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
		
			//Right Edge (apart from Top Right)
			if (j == 0 && i != h - 1)				
			{
				//Self->Left
				SpringConstraint* constraint1 = new SpringConstraint(
					physicsnodes[(i*w) + j], physicsnodes[(i * w) + j + 1],
					physicsnodes[(i*w) + j]->GetPosition(), physicsnodes[(i * w) + j + 1]->GetPosition()
				);
				PhysicsEngine::Instance()->AddConstraint(constraint1);

				//Self->Up
				SpringConstraint* constraint2 = new SpringConstraint(
					physicsnodes[(i*w) + j], physicsnodes[((i + 1) * w) + j],
					physicsnodes[(i*w) + j]->GetPosition(), physicsnodes[((i + 1) *w) + j]->GetPosition()
				);
				PhysicsEngine::Instance()->AddConstraint(constraint2);

				//Self->Top_Left
				SpringConstraint* constraint3 = new SpringConstraint(
					physicsnodes[(i*w) + j], physicsnodes[((i + 1)*w) + j + 1],
					physicsnodes[(i*w) + j]->GetPosition(), physicsnodes[((i + 1)*w) + j + 1]->GetPosition()
				);
				PhysicsEngine::Instance()->AddConstraint(constraint3);

			}
			//Left Edge (apart from Top Left)
			else if (j == w - 1 && i != h - 1)
			{	
				//Self->Up
				SpringConstraint* constraint1 = new SpringConstraint(
					physicsnodes[(i*w) + j], physicsnodes[((i + 1) * w) + j],
					physicsnodes[(i*w) + j]->GetPosition(), physicsnodes[((i + 1) * w) + j]->GetPosition()
				);
				PhysicsEngine::Instance()->AddConstraint(constraint1);

				//Self->Top_Right
				SpringConstraint* constraint2 = new SpringConstraint(
					physicsnodes[(i*w) + j], physicsnodes[((i + 1)*w) + j - 1],
					physicsnodes[(i*w) + j]->GetPosition(), physicsnodes[((i + 1)*w) + j - 1]->GetPosition()
				);
				PhysicsEngine::Instance()->AddConstraint(constraint2);

			}
			//Center (apart from top line)
			else if (i != h - 1) 
			{
				//Self->Left
				SpringConstraint* constraint1 = new SpringConstraint(
					physicsnodes[(i*w) + j], physicsnodes[(i * w) + j + 1],
					physicsnodes[(i*w) + j]->GetPosition(), physicsnodes[(i *w) + j + 1]->GetPosition()
				);
				PhysicsEngine::Instance()->AddConstraint(constraint1);

				//Self->Up
				SpringConstraint* constraint2 = new SpringConstraint(
					physicsnodes[(i*w) + j], physicsnodes[((i + 1) * w) + j],
					physicsnodes[(i*w) + j]->GetPosition(), physicsnodes[((i + 1) * w) + j]->GetPosition()
				);
				PhysicsEngine::Instance()->AddConstraint(constraint2);

				//Self->Up_left
				SpringConstraint* constraint3 = new SpringConstraint(
					physicsnodes[(i*w) + j], physicsnodes[((i + 1)*w) + j + 1],
					physicsnodes[(i*w) + j]->GetPosition(), physicsnodes[((i + 1)*w) + j + 1]->GetPosition()
				);
				PhysicsEngine::Instance()->AddConstraint(constraint3);

				//Self->Up_right
				SpringConstraint* constraint4 = new SpringConstraint(
					physicsnodes[(i*w) + j], physicsnodes[((i + 1)*w) + j - 1],
					physicsnodes[(i*w) + j]->GetPosition(), physicsnodes[((i + 1)*w) + j - 1]->GetPosition()
				);
				PhysicsEngine::Instance()->AddConstraint(constraint4);

			}
			//Top Edge (apart from top Left)
			else if (j != w - 1 && i == h - 1)
			{
				//Self->Left
				SpringConstraint* constraint1 = new SpringConstraint(
					physicsnodes[(i*w) + j], physicsnodes[(i*w) + j + 1],
					physicsnodes[(i*w) + j]->GetPosition(), physicsnodes[(i*w) + j + 1]->GetPosition()
				);
				PhysicsEngine::Instance()->AddConstraint(constraint1);

			}
			//Bottom Right corner has no more attachments needed
		}
	}
}

SoftBody::~SoftBody()
{
}

void SoftBody::UpdateMesh(const Matrix4 &matrix) {
	int k = 0;
	for (int i = 0; i < h-1; ++i) {
		for (int j = 0; j < w - 1; ++j) {
			m->vertices[k] = physicsnodes[(i * w) + j]->GetPosition() - *pos;
			m->vertices[k + 1] = physicsnodes[(i * w) + j + 1]->GetPosition() - *pos;
			m->vertices[k + 2] = physicsnodes[((i + 1) * w) + j]->GetPosition() - *pos;

			m->vertices[k + 3] = physicsnodes[(i * w) + j + 1]->GetPosition() - *pos;
			m->vertices[k + 4] = physicsnodes[((i + 1) * w) + j + 1]->GetPosition() - *pos;
			m->vertices[k + 5] = physicsnodes[((i + 1) * w) + j]->GetPosition() - *pos;

			k += 6;
		}
	}
	m->GenerateNormals();
	m->GenerateTangents();
	m->ClearBuffers();
	m->BufferData();
}
