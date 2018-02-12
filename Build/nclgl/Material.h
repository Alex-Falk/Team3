#pragma once
#include <nclgl\RenderNode.h>

class Material
{
public :
	Material(){}
	~Material(){}

	void			SetRenderNode(RenderNode* renderNode) { this->renderNode = renderNode; }
	RenderNode*		GetRenderNode()const { return renderNode; }

	virtual	bool Apply() = 0;

protected:
	RenderNode*		renderNode;
};
