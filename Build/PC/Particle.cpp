#include <ncltech\SceneManager.h>
#include "Particle.h"
#include <nclgl\Camera.h>

Particle::Particle(Colour col, Vector3 pos, Vector3 vel, float size, string name)
{
	RenderNode * rnode = new RenderNode();

	RGB = EnumToVectorColour(col).ToVector3();

	RenderNode* dummy = new PlayerRenderNode(CommonMeshes::Sphere(), "Particle", EnumToVectorColour(col));
	dummy->SetTransform(Matrix4::Scale(Vector3(size, size, size)));
	dummy->SetMaterial(GraphicsPipeline::Instance()->GetAllMaterials()[MATERIALTYPE::Forward_Lighting]);
	rnode->AddChild(dummy);

	rnode->GetChild()->SetBaseColor(EnumToVectorColour(col));
	rnode->SetTransform(Matrix4::Translation(pos));

	position = pos;
	spawnPos = pos;
	velocity = vel*2.0f;

	renderNode = rnode;
}

void Particle::Update(float dt)
{
	velocity += Vector3(0, -9.81f, 0) * dt;
	position += velocity * dt;	
	lifeTime -= dt;

	float dist = (spawnPos - position).Length();

	//float alpha = max((1.0f - (dist / 10.0f)), 0.0f);

	//renderNode->SetChildColor(Vector4(RGB.x, RGB.y, RGB.z, 0.1f));
	//renderNode->SetChildBaseColor(Vector4(RGB.x, RGB.y, RGB.z, 0.1f));
	//renderNode->SetColor(Vector4(RGB.x, RGB.y, RGB.z, 0.1f));
	//renderNode->SetBaseColor(Vector4(RGB.x, RGB.y, RGB.z, 0.1f));
	renderNode->SetTransform(Matrix4::Translation(position));

	if (dist > 3.0f || lifeTime <= 0)
	{
		SceneManager::Instance()->GetCurrentScene()->RemoveGameObject(this);
	}	
}