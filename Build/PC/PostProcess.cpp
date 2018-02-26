#include "PostProcess.h"

PostProcess::PostProcess()
{
	postProcessShaders = new Shader*[NUMBER_OF_SHADER];

	postProcessShaders[PostProcessType::NORMAL] = new Shader(
		SHADERDIR"SceneRenderer/TechVertexBasic.glsl",
		SHADERDIR"SceneRenderer/TechFragSuperSample.glsl");
	if (!postProcessShaders[PostProcessType::NORMAL]->LinkProgram())
	{
		NCLERROR("Could not link shader: Post Process Shader/Normal");
	}
	
	postProcessShaders[PostProcessType::INVERSION] = new Shader(
		SHADERDIR"SceneRenderer/TechVertexBasic.glsl",
		SHADERDIR"Post Process/INVERSION.glsl");
	if (!postProcessShaders[PostProcessType::INVERSION]->LinkProgram())
	{
		NCLERROR("Could not link shader: Post Process Shader/Blur");
	}
	
	postProcessShaders[PostProcessType::GRAYSCALE] = new Shader(
		SHADERDIR"SceneRenderer/TechVertexBasic.glsl",
		SHADERDIR"Post Process/GRAYSCALE.glsl");
	if (!postProcessShaders[PostProcessType::GRAYSCALE]->LinkProgram())
	{
		NCLERROR("Could not link shader: Post Process Shader/GRAYSCALE");
	}
}

PostProcess::~PostProcess()
{

}

void PostProcess::RenderPostprocessAndPresent()
{
	
}
