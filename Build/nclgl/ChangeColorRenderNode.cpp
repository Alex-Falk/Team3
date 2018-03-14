#include "ChangeColorRenderNode.h"

ChangeColorRenderNode::ChangeColorRenderNode(Mesh * m, string name, Vector4 colour)
	:RenderNode(m, name, colour)
{
	maxTime = 3.0f;
	dt = maxTime;
	currentColorPercentage = 1.0f;
}


ChangeColorRenderNode::~ChangeColorRenderNode()
{
}

void ChangeColorRenderNode::StartChangeColor(Vector4 priviousColor)
{

	cout << "change color" << endl;
}

void ChangeColorRenderNode::Update(float msec)
{
	RenderNode::Update(msec);
	dt += msec;
	currentColorPercentage = min(dt / maxTime, 1.0f);
}
