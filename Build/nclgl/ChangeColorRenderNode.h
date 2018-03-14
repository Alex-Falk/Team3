#pragma once
#include <nclgl\RenderNode.h>
class ChangeColorRenderNode : public RenderNode
{
public :
	ChangeColorRenderNode(Mesh*m = NULL, string name = "ChangeColorRenderNode", Vector4 colour = Vector4(1, 1, 1, 1));
	virtual ~ChangeColorRenderNode();

	void StartChangeColor(Vector4 priviousColor);
	float GetCurrentColorPercentage() { return currentColorPercentage; }

	virtual void Update(float msec);
protected :
	float dt;
	float maxTime;
	float currentColorPercentage;

	Vector4 priviousColor;
};