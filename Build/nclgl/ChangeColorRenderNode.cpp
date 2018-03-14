#include "ChangeColorRenderNode.h"

ChangeColorRenderNode::ChangeColorRenderNode(Mesh * m, string name, Vector4 colour)
	:RenderNode(m, name, colour)
{
	maxTime = 3.0f;
	dt = maxTime;
	currentColorPercent = 1.0f;
	priviousColor = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
}


ChangeColorRenderNode::~ChangeColorRenderNode()
{
}

void ChangeColorRenderNode::StartChangeColor()
{
	dt = 0.0f;
	priviousColor = baseColor;
}

void ChangeColorRenderNode::Update(float msec)
{
	RenderNode::Update(msec);
	dt += msec;
	currentColorPercent = min(dt / maxTime, 1.0f);
}
