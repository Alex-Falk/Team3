#include "PostProcess.h"

PostProcess::PostProcess()
{
	postProcessShaders = new Shader*[NUMBER_OF_SHADER];
	for (int i = 0; i < NUMBER_OF_SHADER; ++i) {
		postProcessShaders[i] = NULL;
	}

	postProcessShaders[PostProcessType::NORMAL] = new Shader(
		SHADERDIR"SceneRenderer/TechVertexBasic.glsl",
		SHADERDIR"SceneRenderer/TechFragSuperSample.glsl");
	if (!postProcessShaders[PostProcessType::NORMAL]->LinkProgram())
	{
		NCLERROR("Could not link shader: Post Process Shader/Normal");
	}
	
	postProcessShaders[PostProcessType::BLUR] = new Shader(
		SHADERDIR"SceneRenderer/TechVertexBasic.glsl",
		SHADERDIR"Post Process/BLUR.glsl");
	if (!postProcessShaders[PostProcessType::BLUR]->LinkProgram())
	{
		NCLERROR("Could not link shader: Post Process Shader/BLUR");
	}

	postProcessShaders[PostProcessType::SHARPEN] = new Shader(
		SHADERDIR"SceneRenderer/TechVertexBasic.glsl",
		SHADERDIR"Post Process/SHARPEN.glsl");
	if (!postProcessShaders[PostProcessType::SHARPEN]->LinkProgram())
	{
		NCLERROR("Could not link shader: Post Process Shader/SHARPEN");
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

	postProcessShaders[PostProcessType::EDGE_DETECTION] = new Shader(
		SHADERDIR"SceneRenderer/TechVertexBasic.glsl",
		SHADERDIR"Post Process/EDGE_DETECTION.glsl");
	if (!postProcessShaders[PostProcessType::EDGE_DETECTION]->LinkProgram())
	{
		NCLERROR("Could not link shader: Post Process Shader/EDGE_DETECTION");
	}

	postProcessShaders[PostProcessType::HDR_BLOOM] = new Shader(
		SHADERDIR"SceneRenderer/TechVertexBasic.glsl",
		SHADERDIR"Post Process/HDR+Bloom.glsl");
	if (!postProcessShaders[PostProcessType::HDR_BLOOM]->LinkProgram())
	{
		NCLERROR("Could not link shader: Post Process Shader/HDR+Bloom");
	}
}

PostProcess::~PostProcess()
{
	for (int i = 0; i < NUMBER_OF_SHADER; ++i) {
		if (postProcessShaders[i] != NULL) {
			SAFE_DELETE(postProcessShaders[i]);
		}
	}
}

void PostProcess::RenderPostProcess()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, GraphicsPipeline::Instance()->GetWidth(),
		GraphicsPipeline::Instance()->GetHeight());
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	float superSamples = (float)(GraphicsPipeline::Instance()->GetNumSuperSamples());
	glUseProgram(GetCurrentPostProcessShader()->GetProgram());
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(GetCurrentPostProcessShader()->GetProgram(), "uColorTex"), 0);
	glUniform1i(glGetUniformLocation(GetCurrentPostProcessShader()->GetProgram(), "uColorTex"), 0);

	glBindTexture(GL_TEXTURE_2D, GraphicsPipeline::Instance()->GetScreenTexColor2());
	glUniform1f(glGetUniformLocation(GetCurrentPostProcessShader()->GetProgram(), "uGammaCorrection"), 
		GraphicsPipeline::Instance()->GetGammaCorrection());
	glUniform1f(glGetUniformLocation(GetCurrentPostProcessShader()->GetProgram(), "uNumSuperSamples"), superSamples);
	glUniform2f(glGetUniformLocation(GetCurrentPostProcessShader()->GetProgram(), "uSinglepixel"), 
		1.f / GraphicsPipeline::Instance()->GetScreenTexWidth(), 
		1.f / GraphicsPipeline::Instance()->GetScreenTexHeight());
}
