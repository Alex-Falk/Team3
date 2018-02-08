#include "RenderNode.h"
#include "../ncltech/TextureManager.h"

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

void RenderNode::AutoSetBoundingRadius()
{
	float farrestDistance = 0;
	for (int i = 0; i < mesh->numVertices; i++)
	{
		float length = mesh->vertices[i].Length();
		if (farrestDistance < length)
			farrestDistance = length;
	}
	boundingRadius = farrestDistance;
}

void RenderNode::DrawOpenGL(bool isShadowPass)
{
	if (!(this->mesh && awake))
		return;

	GraphicsPipeline* graphicsPipeline = GraphicsPipeline::Instance();
	TextureManager* textureManager = TextureManager::Instance();
	Shader* shader = graphicsPipeline->GetAllShaders()[SHADERTYPE::Forward_Lighting];

	glUseProgram(shader->GetProgram()); 
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "uProjViewMtx"), 1, GL_FALSE, (float*)&graphicsPipeline->GetProjViewMtx());

	glActiveTexture(GL_TEXTURE0); 
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "uDiffuseTex"), 0);
	glBindTexture(GL_TEXTURE_2D, textureManager->GetTexture(TEXTURETYPE::Checker_Board));

	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "uCameraPos"), 1, (float*)&graphicsPipeline->GetCamera()->GetPosition());
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "uAmbientColor"), 1, (float*)&graphicsPipeline->GetAmbientColor());
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "uLightDirection"), 1, (float*)&graphicsPipeline->GetLightDirection());
	glUniform1fv(glGetUniformLocation(shader->GetProgram(), "uSpecularFactor"), 1, &graphicsPipeline->GetSpecularFactor());

	glUniform1fv(glGetUniformLocation(shader->GetProgram(), "uNormalizedFarPlanes[0]"), SHADOWMAP_NUM - 1, (float*)&graphicsPipeline->GetNormalizedFarPlanes()[0]);
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "uShadowTransform[0]"), SHADOWMAP_NUM, GL_FALSE, (float*)&graphicsPipeline->GetShadowProjViewMatrices()[0]);
	glUniform2f(glGetUniformLocation(shader->GetProgram(), "uShadowSinglePixel"), 1.f / SHADOWMAP_SIZE, 1.f / SHADOWMAP_SIZE);

	glActiveTexture(GL_TEXTURE4);	
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "uShadowTex"), 4);
	glBindTexture(GL_TEXTURE_2D_ARRAY, graphicsPipeline->GetShadowTex());

	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "uModelMtx"), 1, GL_FALSE, (float*)&worldTransform);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "uColor"), 1, (float*)&color);

	this->mesh->Draw();
}