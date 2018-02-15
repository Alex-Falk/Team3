#include "RenderNode.h"
#include "../nclgl/Material.h"

#include "../ncltech/GraphicsPipeline.h"
RenderNode::RenderNode(Mesh*mesh, Vector4 colour)	{
	awake				= true;
	this->mesh			= mesh;
	this->color			= colour;
	baseColor = colour;
	collideColor = Vector4(1, 0, 0, 0.8f);
	parent				= NULL;
	boundingRadius		= 100.0f;
	distanceFromCamera	= 0.0f;
	modelScale			= Vector3(1,1,1);
	smoothness = 0.3f;
}

RenderNode::~RenderNode(void)	{
	for(unsigned int i = 0; i < children.size(); ++i) {
		delete children[i];
	}
}

void RenderNode::AddChild( RenderNode* s )	{
	children.push_back(s);
	s->parent = this;
}

RenderNode * RenderNode::GetChildWithName(string s)
{
	for (vector<RenderNode*>::iterator i = children.begin(); i != children.end(); ++i) {
		if ((*i)->GetName() == s) {
			return (*i);
		}
	}

	return nullptr;
}

bool	RenderNode::CompareByCameraDistance(RenderNode*a,RenderNode*b)  {
	return (a->distanceFromCamera < b->distanceFromCamera) ? true : false;
}

bool	RenderNode::CompareByZ(RenderNode*a,RenderNode*b)  {
	return (a->GetWorldTransform().GetPositionVector().z < b->GetWorldTransform().GetPositionVector().z) ? true : false;
}

void RenderNode::RecursiveSetMaterial(Material * mat, RenderNode * renderNode)
{
	renderNode->SetMaterial(mat);
	for (int i = 0; i < renderNode->children.size(); i++)
	{
		RecursiveSetMaterial(mat, renderNode->children[i]);
	}
}

void	RenderNode::Update(float msec)	 {
	if(parent) {
		worldTransform = parent->worldTransform * transform;
	}
	else{
		worldTransform = transform;
	}

	if (isCollided) {
		color = collideColor;
	}
	else { color = baseColor; }


	for(vector<RenderNode*>::iterator i = children.begin(); i != children.end(); ++i) {
		(*i)->Update(msec);
	}
}

bool RenderNode::RemoveChild(RenderNode* s,bool recursive) {

	for(auto i = children.begin(); i != children.end(); ++i) {
		if((*i) == s) {
			i = children.erase(i);
			return true;
		}
	}

	if(recursive) {
		for(auto i = children.begin(); i != children.end(); ++i) {
			if((*i)->RemoveChild(s,recursive)) {
				return true;
			}
		}
	}
	return false;
}

void RenderNode::AutoSetBoundingRadius()
{
	float farrestDistance = 0;
	for (uint i = 0; i < mesh->numVertices; i++)
	{
		float length = mesh->vertices[i].Length();
		if (farrestDistance < length)
			farrestDistance = length;
	}
	boundingRadius = farrestDistance;
}

void RenderNode::SetMaterial(Material * mat, bool isSetChild)
{
	if (!isSetChild)
		material = mat;
	else
		RecursiveSetMaterial(mat,this);
}

void RenderNode::DrawOpenGL(bool isShadowPass, Material* tempMat)
{
	if (!(this->mesh && awake))
		return;

	if (tempMat != nullptr)
	{
		tempMat->SetRenderNode(this);
		tempMat->Apply();
	}
	else
	{
		material->SetRenderNode(this);
		material->Apply();
	}

	this->mesh->Draw();
}