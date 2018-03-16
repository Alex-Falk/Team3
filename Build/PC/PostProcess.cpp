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
		SHADERDIR"Post Process/GaussianBlur.glsl");
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

	postProcessShaders[PostProcessType::BASIC] = new Shader(
		SHADERDIR"SceneRenderer/TechVertexBasic.glsl",
		SHADERDIR"Post Process/Basic.glsl");
	if (!postProcessShaders[PostProcessType::BASIC]->LinkProgram())
	{
		NCLERROR("Could not link shader: Post Process Shader/BASIC");
	}

	postProcessShaders[PostProcessType::SOBEL] = new Shader(
		SHADERDIR"SceneRenderer/TechVertexBasic.glsl",
		SHADERDIR"Post Process/SOBEL.glsl");
	if (!postProcessShaders[PostProcessType::SOBEL]->LinkProgram())
	{
		NCLERROR("Could not link shader: Post Process Shader/SOBEL");
	}

	postProcessShaders[PostProcessType::PERFORMANCE_BLUR] = new Shader(
		SHADERDIR"SceneRenderer/TechVertexBasic.glsl",
		SHADERDIR"Post Process/BLUR.glsl");
	if (!postProcessShaders[PostProcessType::PERFORMANCE_BLUR]->LinkProgram())
	{
		NCLERROR("Could not link shader: Post Process Shader/BLUR");
	}

	postProcessShaders[PostProcessType::_WIN] = new Shader(
		SHADERDIR"SceneRenderer/TechVertexBasic.glsl",
		SHADERDIR"Post Process/WIN.glsl");
	if (!postProcessShaders[PostProcessType::_WIN]->LinkProgram())
	{
		NCLERROR("Could not link shader: Post Process Shader/SOBEL");
	}

	for (int i = 0; i < 2; ++i) {
		pingpongBuffers[i] = NULL;
		pingpongFBO[i] = NULL;
	}
	screenQuad2 = Mesh::GenerateQuad();
	GenerateTextrue();
	GenerateScreenFBO2();

	currentPostProcessType = PostProcessType::HDR_BLOOM;
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
	//Render two passes to get gaussian blur with only bright part of the scene
	//afterwards, this will act as a texture of screenFBO2
	//then by sampling normal texture and screenFBO2texture, we get bloom effect

	switch (currentPostProcessType) {
	case PostProcessType::HDR_BLOOM:
		RenderGaussianBlur(1);
		break;
	case PostProcessType::BLUR:
		RenderGaussianBlur();
		break;
	}
	RenderToBackBuffer();
}

void PostProcess::GenerateTextrue()
{
	auto SetTextureDefaults = []() {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	};

	for (int i = 0; i < 2; ++i) {
		glGenTextures(1, &pingpongBuffers[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongBuffers[i]);
		SetTextureDefaults();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F,
			GraphicsPipeline::Instance()->GetScreenTexWidth(),
			GraphicsPipeline::Instance()->GetScreenTexHeight(),
			0, GL_RED, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void PostProcess::GenerateScreenFBO2()
{
	for (int i = 0; i < 2; ++i) {
		glGenFramebuffers(1, &pingpongFBO[i]);
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffers[i], 0);
		GLenum buf2 = GL_COLOR_ATTACHMENT0;
		glDrawBuffers(1, &buf2);
		//Validate our framebuffer
		if ((glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
		{
			NCLERROR("Unable to create Screen Framebuffer! StatusCode: %x");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

//Render Pass 2: Applying GaussianBlur
void PostProcess::RenderGaussianBlur(int a)
{
	//Basic settings for Renderring
	glViewport(0, 0, GraphicsPipeline::Instance()->GetScreenTexWidth(),
		GraphicsPipeline::Instance()->GetScreenTexHeight());
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//Uniform value passing
	glUseProgram(postProcessShaders[PostProcessType::BLUR]->GetProgram());
	glUniform1i(glGetUniformLocation(postProcessShaders[PostProcessType::BLUR]->GetProgram(), "uColorTex"), 0);
	glUniform2f(glGetUniformLocation(postProcessShaders[PostProcessType::BLUR]->GetProgram(), "uSinglepixel"),1.f / GraphicsPipeline::Instance()->GetScreenTexWidth(),1.f / GraphicsPipeline::Instance()->GetScreenTexHeight());
	glActiveTexture(GL_TEXTURE0);
	GLuint amount = 10;
	GLboolean horizontal = true, first_iteration = true;
	for (uint i = 0; i < amount; ++i) {
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
		glUniform1i(glGetUniformLocation(postProcessShaders[PostProcessType::BLUR]->GetProgram(), "horizontal"), horizontal);
		if ((bool)first_iteration == true) {
			if (a == 1) {
				glBindTexture(GL_TEXTURE_2D, GraphicsPipeline::Instance()->GetScreenTexColor2());
			}
			else {
				glBindTexture(GL_TEXTURE_2D, GraphicsPipeline::Instance()->GetScreenTexColor1());
			}
			first_iteration = false;
		}
		else {
			glBindTexture(GL_TEXTURE_2D, pingpongBuffers[!horizontal]);
		}
		screenQuad2->Draw();
		horizontal = !horizontal;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcess::RenderToBackBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, GraphicsPipeline::Instance()->GetWidth(), GraphicsPipeline::Instance()->GetHeight());

	float superSamples = (float)(GraphicsPipeline::Instance()->GetNumSuperSamples());
	glUseProgram(GetCurrentPostProcessShader()->GetProgram());
	glUniform1f(glGetUniformLocation(GetCurrentPostProcessShader()->GetProgram(), "uGammaCorrection"),
		GraphicsPipeline::Instance()->GetGammaCorrection());
	glActiveTexture(GL_TEXTURE0);

	if (GetCurrentPostProcessShader() != postProcessShaders[PostProcessType::HDR_BLOOM] && currentPostProcessType != PostProcessType::BLUR) {
		glUniform1i(glGetUniformLocation(GetCurrentPostProcessShader()->GetProgram(), "uColorTex"), 0);
		//glBindTexture(GL_TEXTURE_2D, pingpongBuffers[0]);
		glBindTexture(GL_TEXTURE_2D, GraphicsPipeline::Instance()->GetScreenTexColor1());
		glUniform1f(glGetUniformLocation(GetCurrentPostProcessShader()->GetProgram(), "uNumSuperSamples"), superSamples);
		glUniform2f(glGetUniformLocation(GetCurrentPostProcessShader()->GetProgram(), "uSinglepixel"),1.f / GraphicsPipeline::Instance()->GetScreenTexWidth(),1.f / GraphicsPipeline::Instance()->GetScreenTexHeight());
	}
	else if (currentPostProcessType == PostProcessType::BLUR) {
		glUseProgram(postProcessShaders[PostProcessType::BASIC]->GetProgram());
		glUniform1f(glGetUniformLocation(GetCurrentPostProcessShader()->GetProgram(), "uGammaCorrection"),
			GraphicsPipeline::Instance()->GetGammaCorrection());
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(GetCurrentPostProcessShader()->GetProgram(), "uColorTex"), 0);
		glBindTexture(GL_TEXTURE_2D, pingpongBuffers[0]);
		glUniform1f(glGetUniformLocation(GetCurrentPostProcessShader()->GetProgram(), "uNumSuperSamples"), superSamples);
		glUniform2f(glGetUniformLocation(GetCurrentPostProcessShader()->GetProgram(), "uSinglepixel"), 1.f / GraphicsPipeline::Instance()->GetScreenTexWidth(), 1.f / GraphicsPipeline::Instance()->GetScreenTexHeight());
	}
	else {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GraphicsPipeline::Instance()->GetScreenTexColor1());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, pingpongBuffers[0]);
		glUniform1i(glGetUniformLocation(GetCurrentPostProcessShader()->GetProgram(), "scene"), 0);
		glUniform1i(glGetUniformLocation(GetCurrentPostProcessShader()->GetProgram(), "bloomBlur"), 1);
	}
	GraphicsPipeline::Instance()->GetScreenQuad()->Draw();
}

