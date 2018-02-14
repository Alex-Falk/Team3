#pragma once

#include <nclgl\RenderNode.h>

class PlayerRenderNode : public RenderNode
{
public :
	PlayerRenderNode(Mesh*m = NULL, Vector4 colour = Vector4(1, 1, 1, 1));
	virtual ~PlayerRenderNode();

	void SetIsInAir(bool _IsInAir){}
	bool GetIsInAir()const { return isInAir; }
protected:
	bool isInAir;
};
