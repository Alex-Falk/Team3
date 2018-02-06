#include "RenderNode.h"

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

void RenderNode::SetBoundingRadius()
{
}