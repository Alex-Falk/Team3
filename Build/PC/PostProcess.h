//Jeffery 26/02/2018

#pragma once

#include <nclgl/TSingleton.h>
#include <nclgl/Shader.h>
#include <nclgl/NCLDebug.h>
#include <ncltech/GraphicsPipeline.h>

enum PostProcessType{
	NORMAL				= 0,
	BLUR				= 1,
	INVERSION			= 2,
	GRAYSCALE			= 3,
	SHARPEN				= 4,
	EDGE_DETECTION		= 5,
	HDR_BLOOM			= 6,
	NUMBER_OF_SHADER
};

class PostProcess : public TSingleton<PostProcess>
{
public:
	friend class TSingleton<PostProcess>;
	
	PostProcess();
	~PostProcess();

	Shader* GetCurrentPostProcessShader() { return postProcessShaders[currentPostProcessType]; }
	PostProcessType GetCurrentPostProcessType() { return currentPostProcessType; }
	void SetPostProcessType(PostProcessType type) { this->currentPostProcessType = type; }
	void RenderPostProcess();

private:
	Shader** postProcessShaders;
	PostProcessType currentPostProcessType;
};