#pragma once
#include <nclgl\RenderNode.h>

class Material
{
public :
	Material(){}
	~Material(){}

	//void			SetShader(Shader* shader)	{ this->shader = shader; }
	//Shader*			GetShader()const			{ return shader; }
	void			SetRenderNode(RenderNode* renderNode) { this->renderNode = renderNode; }
	RenderNode*		GetRenderNode()const { return renderNode; }

	virtual	bool Apply() = 0;

protected:
	//Shader*			shader;
	RenderNode*		renderNode;
};
