
/*****************************************************************************
:;-"""-::::::::::::::::::::::::::::::::::::::::::::::::.::..:... ..  .
(      .)::::::::::::::::::::::::::::::::::::::::::::.::..:... ..  .
        )-:::::::::::::::::::::::::::::::::::::::::::::::.::..:... .. .
      -'   )-"-:::::::::::::::::::::::::::::::::::::::.::..:... ..  .
 ___________)______________          _____
|                         \          \ U \__      _______
|        Yesheng Su        \__________\   \/_______\___\_____________
|        14/02/2018        /          < /_/   ..................... ^`-._
|_________________________/            `-----------,----,--------------'
                      )                          _/____/_
-.                .    ):::::::::::::::::::::::::::::.::..:... ..  .
  )--.__..--"-.__,'---':::::::::::::::::::::::::::::::::.::..:... ..  .
-':::::::::::::::::::::::::::::::::::::::::::::.::..:... ..  .
:::::::::::::::::::::::::::::::::::::::::::::::::.::..:... ..  .
															
*****************************************************************************/
//adpated by Jianfei

#include "GraphicsPipeline.h"
#include "ScreenPicker.h"
#include "BoundingBox.h"
#include <nclgl\NCLDebug.h>
#include <algorithm>
#include <ncltech\TextureManager.h>
//used by minimap
#include <PC\Game.h>
#include <PC\Map.h>
#include <PC\PaintPool.h>

GraphicsPipeline::GraphicsPipeline()
	: OGLRenderer(Window::GetWindow())
	, camera(new Camera())
	, isVsyncEnabled(false)
	, screenTexWidth(0)
	, screenTexHeight(0)
	, screenFBO(NULL)
	, screenTexDepth(NULL)
	, fullscreenQuad(NULL)
	, shadowFBO(NULL)
	, shadowTex(NULL)
	, pathFBO(NULL)
	, pathTex(NULL)
	, scoreFBO(NULL)
	,time(0.0f)
{
	for (int i = 0; i < 2; ++i) {
		screenTexColor[i] = NULL;
	}
	LoadShaders();
	LoadMaterial();
	NCLDebug::_LoadShaders();

	fullscreenQuad = Mesh::GenerateQuad();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_CLAMP);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glDepthFunc(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	sceneBoundingRadius = 30.f; ///Approx based on scene contents

	camera->SetPosition(Vector3(0.0f, 10.0f, 15.0f));
	camera->SetYaw(0.f);
	camera->SetPitch(-20.f);

	InitializeDefaults();
	Resize(width, height);
}

GraphicsPipeline::~GraphicsPipeline()
{
	SAFE_DELETE(camera);

	SAFE_DELETE(fullscreenQuad);

	for (int i = 0; i < Shader_Number; i++)
	{
		SAFE_DELETE(shaders[i]);
		SAFE_DELETE(materials[i]);
	}

	delete[] shaders;
	delete[] materials;

	NCLDebug::_ReleaseShaders();

	if (screenFBO)
	{
		glDeleteTextures(1, &screenTexColor[0]);
		glDeleteTextures(1, &screenTexColor[1]);
		glDeleteTextures(1, &screenTexDepth);
		glDeleteFramebuffers(1, &screenFBO);
		screenFBO = NULL;
	}

	if (shadowFBO)
	{
		glDeleteTextures(1, &shadowTex);
		glDeleteFramebuffers(1, &shadowFBO);
		shadowFBO = NULL;
	}

	if (pathFBO)
	{
		glDeleteTextures(1, &pathTex);
		glDeleteFramebuffers(1, &pathFBO);
		pathFBO = NULL;
	}

	if (scoreFBO)
	{
		glDeleteFramebuffers(1, &scoreFBO);
		scoreFBO = NULL;
	}

	playerRenderNodes.clear();
	pathRenderNodes.clear();
}

void GraphicsPipeline::InitializeDefaults()
{
	allNodes.clear();
	ScreenPicker::Instance()->ClearAllObjects();

	//--- Do we want the camera to reset pos/orientation everytime we reset a scene????
	//camera->SetPosition(Vector3(-3.0f, 10.0f, 15.0f));
	//camera->SetYaw(-10.f);
	//camera->SetPitch(-30.f);

	backgroundColor = Vector3(0.8f, 0.8f, 0.8f);
	ambientColor = Vector3(0.2f, 0.2f, 0.2f);
	lightDirection = Vector3(0.5f, -1.0f, -0.8f); lightDirection.Normalise();
	specularFactor = 96.0f;

	numSuperSamples = 4;
	gammaCorrection = 1.0f / 2.2f;
}


void GraphicsPipeline::AddRenderNode(RenderNode* node)
{
	if (std::find(allNodes.begin(), allNodes.end(), node) == allNodes.end())
		allNodes.push_back(node);
}


void GraphicsPipeline::RemoveRenderNode(RenderNode* node)
{
	allNodes.erase(std::remove(allNodes.begin(), allNodes.end(), node), allNodes.end());
}

void GraphicsPipeline::RemovePlayerRenderNode(RenderNode* node)
{
	playerRenderNodes.erase(std::remove(playerRenderNodes.begin(), playerRenderNodes.end(), node), playerRenderNodes.end());
}

void GraphicsPipeline::LoadShaders()
{
	shaders = new Shader*[Shader_Number];

	shaders[SHADERTYPE::Present_To_Window] = new Shader(
		SHADERDIR"SceneRenderer/TechVertexBasic.glsl",
		SHADERDIR"SceneRenderer/TechFragSuperSample.glsl");
	if (!shaders[SHADERTYPE::Present_To_Window]->LinkProgram())
	{
		NCLERROR("Could not link shader: Present to window / SuperSampling");
	}

	shaders[SHADERTYPE::Shadow] = new Shader(
		SHADERDIR"SceneRenderer/TechVertexShadow.glsl",
		SHADERDIR"Common/EmptyFragment.glsl",
		SHADERDIR"SceneRenderer/TechGeomShadow.glsl");
	if (!shaders[SHADERTYPE::Shadow]->LinkProgram())
	{
		NCLERROR("Could not link shader: Shadow Shader");
	}

	shaders[SHADERTYPE::Forward_Lighting] = new Shader(
		SHADERDIR"SceneRenderer/TechVertexFull.glsl",
		SHADERDIR"SceneRenderer/TechFragForwardRender.glsl");
	if (!shaders[SHADERTYPE::Forward_Lighting]->LinkProgram())
	{
		NCLERROR("Could not link shader: Forward Renderer");
	}

	shaders[SHADERTYPE::Texture_UI] = new Shader(
		SHADERDIR"Game/TextureUIVertex.glsl",
		SHADERDIR"Game/TextureUIFragment.glsl");
	if (!shaders[SHADERTYPE::Texture_UI]->LinkProgram())
	{
		NCLERROR("Could not link shader: Simple UI Renderer");
	}

	shaders[SHADERTYPE::Draw_Path] = new Shader(
		SHADERDIR"Common/EmptyVertex.glsl",
		SHADERDIR"Common/ColorFragment.glsl");
	if (!shaders[SHADERTYPE::Draw_Path]->LinkProgram())
	{
		NCLERROR("Could not link shader: Draw Path Renderer");
	}

	shaders[SHADERTYPE::Ground] = new Shader(
		SHADERDIR"Game/GroundVertex.glsl",
		SHADERDIR"Game/GroundFragment.glsl");
	if (!shaders[SHADERTYPE::Ground]->LinkProgram())
	{
		NCLERROR("Could not link shader: Ground");
	}

	shaders[SHADERTYPE::SkyBox] = new Shader(
		SHADERDIR"Game/SkyBoxVertex.glsl",
		SHADERDIR"Game/SkyBoxFragment.glsl");
	if (!shaders[SHADERTYPE::SkyBox]->LinkProgram())
	{
		NCLERROR("Could not link shader: SkyBox");
	}

	//phil's minimap
	shaders[SHADERTYPE::MiniMap] = new Shader(
		SHADERDIR"Game/MiniMapVertex.glsl",
		SHADERDIR"Game/MiniMapFragment.glsl"
	);
	if (!shaders[SHADERTYPE::MiniMap]->LinkProgram()) {
		NCLERROR("Could not link shader: MiniMap");
	}

	//Alex's Score
	shaders[SHADERTYPE::Score] = new Shader(
		SHADERDIR"Game/ScoreVertex.glsl",
		SHADERDIR"Game/ScoreFragment.glsl");
	if (!shaders[SHADERTYPE::Score]->LinkProgram())
	{
		NCLERROR("Could not link shader: Score");
	}

}

void GraphicsPipeline::LoadMaterial()
{
	materials = new Material*[Material_Number];
	materials[MATERIALTYPE::Forward_Lighting] = new StandardMaterial();
	materials[MATERIALTYPE::Present_To_Window] = new PresentToWindowMaterial();
	materials[MATERIALTYPE::Shadow] = new ShadowMaterial();
	materials[MATERIALTYPE::Texture_UI] = nullptr;
	materials[MATERIALTYPE::Draw_Path] = new DrawPathMaterial();
	materials[MATERIALTYPE::Ground] = new GroundMaterial();
	materials[MATERIALTYPE::SkyBox] = nullptr;
	materials[MATERIALTYPE::MiniMap] = nullptr;
	materials[MATERIALTYPE::Score] = nullptr;
}

void GraphicsPipeline::UpdateAssets(int width, int height)
{
	GLuint status;

	//Screen Framebuffer
	if (width * numSuperSamples != screenTexWidth || height * numSuperSamples != screenTexHeight)
	{
		screenTexWidth = (uint)(width * numSuperSamples);
		screenTexHeight = (uint)(height * numSuperSamples);
		ScreenPicker::Instance()->UpdateAssets(screenTexWidth, screenTexHeight);


		auto SetTextureDefaults = []() {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		};

		//Color Texture
		if (!screenTexColor[0]) glGenTextures(1, &screenTexColor[0]);
		glBindTexture(GL_TEXTURE_2D, screenTexColor[0]);
		SetTextureDefaults();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, screenTexWidth, screenTexHeight, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

		//Multi-Rendering texture
		if (!screenTexColor[1]) glGenTextures(1, &screenTexColor[1]);
		glBindTexture(GL_TEXTURE_2D, screenTexColor[1]);
		SetTextureDefaults();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, screenTexWidth, screenTexHeight, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

		//Depth+Stencil Texture
		if (!screenTexDepth) glGenTextures(1, &screenTexDepth);
		glBindTexture(GL_TEXTURE_2D, screenTexDepth);
		SetTextureDefaults();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, screenTexWidth, screenTexHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		//Generate our Framebuffer
		if (!screenFBO) glGenFramebuffers(1, &screenFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, screenFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexColor[0], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, screenTexColor[1], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, screenTexDepth, 0);
		GLenum buf[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, buf);
		//Validate our framebuffer
		if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
		{
			NCLERROR("Unable to create Screen Framebuffer! StatusCode: %x", status);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	//Construct our Shadow Maps and Shadow UBO
	if (!shadowTex) glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D_ARRAY, shadowTex);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_DEPTH_COMPONENT32, SHADOWMAP_SIZE, SHADOWMAP_SIZE, SHADOWMAP_NUM);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	if (!shadowFBO) glGenFramebuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowTex, 0);
	glDrawBuffers(0, GL_NONE);
	if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
	{
		NCLERROR("Unable to create Shadow Framebuffer! StatusCode: %x", status);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//m_ShadowUBO._ShadowMapTex = glGetTextureHandleARB(m_ShadowTex);
	//glMakeTextureHandleResidentARB(m_ShadowUBO._ShadowMapTex);

}

void GraphicsPipeline::DebugRender()
{
	// Draw all bounding radius
	if (debugDrawFlags & DEBUGDRAW_FLAGS_BOUNDING)
	{
		for each (RenderNode* var in allNodes)
		{
			Vector3 pos = var->GetWorldTransform().GetPositionVector();
			float radius = var->GetBoundingRadius();
			//Draw Perimeter Axes
			Vector3 lastX = pos + Vector3(0.0f, 1.0f, 0.0f) * radius;
			Vector3 lastY = pos + Vector3(1.0f, 0.0f, 0.0f) * radius;
			Vector3 lastZ = pos + Vector3(1.0f, 0.0f, 0.0f) * radius;
			const int nSubdivisions = 20;
			for (int itr = 1; itr <= nSubdivisions; ++itr)
			{
				float angle = itr / float(nSubdivisions) * PI * 2.f;
				float alpha = cosf(angle) * radius;
				float beta = sinf(angle) * radius;

				Vector3 newX = pos + Vector3(0.0f, alpha, beta);
				Vector3 newY = pos + Vector3(alpha, 0.0f, beta);
				Vector3 newZ = pos + Vector3(alpha, beta, 0.0f);

				NCLDebug::DrawThickLineNDT(lastX, newX, 0.02f, Vector4(1.0f, 0.3f, 1.0f, 1.0f));
				NCLDebug::DrawThickLineNDT(lastY, newY, 0.02f, Vector4(1.0f, 0.3f, 1.0f, 1.0f));
				NCLDebug::DrawThickLineNDT(lastZ, newZ, 0.02f, Vector4(1.0f, 0.3f, 1.0f, 1.0f));

				lastX = newX;
				lastY = newY;
				lastZ = newZ;
			}
		}
	}
}

void GraphicsPipeline::UpdateScene(float dt)
{
	//update all of the camera stuff
	camera->UpdateCamara(dt);


	viewMatrix = camera->BuildViewMatrix();
	projViewMatrix = projMatrix * viewMatrix;

	//update frustum
	frameFrustum.FromMatrix(projViewMatrix);

	//increment time
	time += dt;
	NCLDebug::_SetDebugDrawData(
		projMatrix,
		viewMatrix,
		camera->GetPosition());
}

void GraphicsPipeline::RenderScene()
{
	//Build World Transforms
	// - Most scene objects will probably end up being static, so we really should only be updating
	//   modelMatrices for objects (and their children) who have actually moved since last frame
	for (RenderNode* node : allNodes)
		node->Update(0.0f); //Not sure what the msec is here is for, apologies if this breaks anything in your framework!

	//Build Transparent/Opaque Renderlists
	BuildAndSortRenderLists();

	//NCLDebug - Build render lists
	NCLDebug::_BuildRenderLists();

	//Build shadowmaps
	RenderShadow();

	//Render scene to screen fbo
	RenderObject();

	//render the path to texture
	RenderPath();

	//post process and present
	RenderPostprocessAndPresent();

	//draw the minimap on screen
	if (isMainMenu == false) {
		CountScore();
		DrawMiniMap();
	}

	//NCLDEBUG - Text Elements (aliased)
	if (isMainMenu == false) {
		NCLDebug::_RenderDebugClipSpace();
	}
	NCLDebug::_ClearDebugLists();


	//RenderUI
	RenderUI();


	OGLRenderer::SwapBuffers();
}

void GraphicsPipeline::Resize(int x, int y)
{
	if (x <= 0 || y <= 0)
	{
		x = 1;
		y = 1;
	}

	//Generate/Resize any screen textures (if needed)
	UpdateAssets(x, y);

	//Update 'width', 'height' vars
	OGLRenderer::Resize(x, y);

	//Update our projection matrix
	projMatrix = Matrix4::Perspective(CAMERA_PROJ_NEAR, CAMERA_PROJ_FAR, (float)x / (float)y, CAMERA_PROJ_FOV);

}

void GraphicsPipeline::BuildAndSortRenderLists()
{
	//Divide 'nodesAll' into transparent and opaque versions
	// - As objects don't change their color/transparency that often
	//   this could be improved by just directly inserting into the given list
	//   and maintaining a permenantly sorted list of transparent objects.
	renderlistOpaque.clear();
	renderlistTransparent.clear();

	for (RenderNode* node : allNodes)
		RecursiveAddToRenderLists(node);

	//Sort transparent objects back to front
	std::sort(
		renderlistTransparent.begin(),
		renderlistTransparent.end(),
		[](const RenderNodePair& a, const RenderNodePair& b)
	{
		return a.second > b.second;
	}
	);

	//Sort opaque objects back to front
	std::sort(
		renderlistOpaque.begin(),
		renderlistOpaque.end(),
		[](const RenderNodePair& a, const RenderNodePair& b)
	{
		return a.second > b.second;
	}
	);
}

void GraphicsPipeline::RecursiveAddToRenderLists(RenderNode* node)
{
	if (frameFrustum.InsideFrustum(*node))
	{
		//If the node is renderable, add it to either a opaque or transparent render list
		if (node->IsRenderable())
		{
			Vector3 diff = node->GetWorldTransform().GetPositionVector() - camera->GetPosition();
			float camDistSq = Vector3::Dot(diff, diff); //Same as doing .Length() without the sqrt

			if (node->GetColor().w > 0.999f)
				renderlistOpaque.push_back({ node, camDistSq });
			else
				renderlistTransparent.push_back({ node, camDistSq });
		}
	}

	//Recurse over all children and process them aswell
	for (auto itr = node->GetChildIteratorStart(); itr != node->GetChildIteratorEnd(); itr++)
		RecursiveAddToRenderLists(*itr);
}

void GraphicsPipeline::RenderAllObjects(bool isShadowPass, std::function<void(RenderNode*)> perObjectFunc)
{
	// sort render opaque order
	for (std::vector<RenderNodePair>::reverse_iterator i = renderlistOpaque.rbegin(); i != renderlistOpaque.rend(); ++i)
	{
		perObjectFunc((*i).first);
		if ((*i).first->IsCulling()) { glEnable(GL_CULL_FACE); }
		else { glDisable(GL_CULL_FACE); }
		(*i).first->DrawOpenGL(isShadowPass);
	}

	if (isShadowPass)
	{
		for (std::vector<RenderNodePair>::iterator i = renderlistTransparent.begin(); i != renderlistTransparent.end(); ++i)
		{
			perObjectFunc((*i).first);
			(*i).first->DrawOpenGL(isShadowPass);
		}
	}
	else
	{
		for (std::vector<RenderNodePair>::iterator i = renderlistTransparent.begin(); i != renderlistTransparent.end(); ++i)
		{
			perObjectFunc((*i).first);
			glCullFace(GL_FRONT);
			(*i).first->DrawOpenGL(isShadowPass);

			glCullFace(GL_BACK);
			(*i).first->DrawOpenGL(isShadowPass);
		}
	}
}

void GraphicsPipeline::BuildShadowTransforms()
{
	const float proj_range = SHADOW_PROJ_FAR - SHADOW_PROJ_NEAR;

	//Variable size - shadowmap always rotated to be square with camera
	//  Vector3 viewDir = Matrix3::Transpose(Matrix3(viewMatrix)) * Vector3(0, 0, 1);
	//  Matrix4 lightview = Matrix4::BuildViewMatrix(Vector3(0.0f, 0.0f, 0.0f), -lightDirection, viewDir);	

	//Fixed size shadow area (just moves with camera) 
	shadowViewMtx = Matrix4::BuildViewMatrix(Vector3(0.0f, 0.0f, 0.0f), -lightDirection, Vector3(0, 1, 0));

	Matrix4 invCamProjView = Matrix4::Inverse(projMatrix * viewMatrix);

	auto compute_depth = [&](float x)
	{
		float proj_start = -(proj_range * x + SHADOW_PROJ_NEAR);
		return (proj_start*projMatrix[10] + projMatrix[14]) / (proj_start*projMatrix[11]);
	};

	const float divisor = (SHADOWMAP_NUM*SHADOWMAP_NUM) - 1.f;
	for (int i = 0; i < SHADOWMAP_NUM; ++i)
	{
		//Linear scalars going from 0.0f (near) to 1.0f (far)
		float lin_near = (powf(2.0f, (float)(i)) - 1.f) / divisor;
		float lin_far = (powf(2.0f, (float)(i + 1)) - 1.f) / divisor;

		//True non-linear depth ranging from -1.0f (near) to 1.0f (far)
		float norm_near = compute_depth(lin_near);
		float norm_far = compute_depth(lin_far);

		//Build Bounding Box around frustum section (Axis Aligned)
		BoundingBox bb;
		bb.ExpandToFit(invCamProjView * Vector3(-1.0f, -1.0f, norm_near));
		bb.ExpandToFit(invCamProjView * Vector3(-1.0f, 1.0f, norm_near));
		bb.ExpandToFit(invCamProjView * Vector3(1.0f, -1.0f, norm_near));
		bb.ExpandToFit(invCamProjView * Vector3(1.0f, 1.0f, norm_near));
		bb.ExpandToFit(invCamProjView * Vector3(-1.0f, -1.0f, norm_far));
		bb.ExpandToFit(invCamProjView * Vector3(-1.0f, 1.0f, norm_far));
		bb.ExpandToFit(invCamProjView * Vector3(1.0f, -1.0f, norm_far));
		bb.ExpandToFit(invCamProjView * Vector3(1.0f, 1.0f, norm_far));

		//Rotate bounding box so it's orientated in the lights direction
		// - Rotates bounding box and creates a new AABB that encompasses it
		bb = bb.Transform(shadowViewMtx);

		//Extend the Z depths to catch shadow casters outside view frustum
		bb._min.z = min(bb._min.z, -sceneBoundingRadius);
		bb._max.z = max(bb._max.z, sceneBoundingRadius);

		//Build Light Projection		
		shadowProj[i] = Matrix4::Orthographic(bb._max.z, bb._min.z, bb._min.x, bb._max.x, bb._max.y, bb._min.y);
		shadowProjView[i] = shadowProj[i] * shadowViewMtx;
	}
}

void GraphicsPipeline::RenderShadow()
{
	BuildShadowTransforms();
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowTex, 0);
	glViewport(0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE);
	glClear(GL_DEPTH_BUFFER_BIT);

	for (std::vector<RenderNodePair>::reverse_iterator i = renderlistOpaque.rbegin(); i != renderlistOpaque.rend(); ++i) {
		if ((*i).first->IsCulling()) { glEnable(GL_CULL_FACE); }
		else { glDisable(GL_CULL_FACE); }
		(*i).first->DrawOpenGL(true, materials[MATERIALTYPE::Shadow]);
	}
	for (std::vector<RenderNodePair>::iterator i = renderlistTransparent.begin(); i != renderlistTransparent.end(); ++i) {
		Material* mat = (*i).first->GetMaterial();
		(*i).first->DrawOpenGL(true, materials[MATERIALTYPE::Shadow]);
	}
}

void GraphicsPipeline::RenderObject()
{
	glBindFramebuffer(GL_FRAMEBUFFER, screenFBO);
	glViewport(0, 0, screenTexWidth, screenTexHeight);
	glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glUseProgram(shaders[SHADERTYPE::SkyBox]->GetProgram());
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shaders[SHADERTYPE::SkyBox]->GetProgram(), "cubeTex"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureManager::Instance()->GetTexture(TEXTURETYPE::Sky_Box));
	glUniformMatrix4fv(glGetUniformLocation(shaders[SHADERTYPE::SkyBox]->GetProgram(), "viewMatrix"), 1, false, (float*)&camera->BuildViewMatrix());
	glUniformMatrix4fv(glGetUniformLocation(shaders[SHADERTYPE::SkyBox]->GetProgram(), "projMatrix"), 1, false, (float*)&projMatrix);
	glUniform1f(glGetUniformLocation(shaders[SHADERTYPE::SkyBox]->GetProgram(), "brightness"), 1.0f);

	glDisable(GL_DEPTH_TEST);
	fullscreenQuad->Draw(); //Draw sky box
	glEnable(GL_DEPTH_TEST);

	RenderAllObjects(false, [&](RenderNode* node) {});

	// Render Screen Picking ID's
	// - This needs to be somewhere before we lose our depth buffer
	//   BUT at the moment that means our screen picking is super sampled and rendered at 
	//   a much higher resolution. Which is silly.
	ScreenPicker::Instance()->RenderPickingScene(projViewMatrix, Matrix4::Inverse(projViewMatrix), screenTexDepth, screenTexWidth, screenTexHeight);

	glBindFramebuffer(GL_FRAMEBUFFER, screenFBO);
	glViewport(0, 0, screenTexWidth, screenTexHeight);

	//NCLDEBUG - World Debug Data (anti-aliased)
	if (isMainMenu == false) {
		NCLDebug::_RenderDebugDepthTested();
		NCLDebug::_RenderDebugNonDepthTested();
	}

	//debug code
	/*glUseProgram(shaders[SHADERTYPE::Texture_UI]->GetProgram());
	glDisable(GL_DEPTH_TEST);
	Matrix4 mat = Matrix4::Translation(Vector3(0.7f, 0.7f, 0.0f))* Matrix4::Scale(Vector3(0.3f, 0.3f, 1.0f));
	glUniformMatrix4fv(glGetUniformLocation(shaders[SHADERTYPE::Texture_UI]->GetProgram(), "modelMatrix"), 1, GL_FALSE, (float*)&mat);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shaders[SHADERTYPE::Texture_UI]->GetProgram(), "diffuseTex"), 0);
	
	if (isMainMenu == false) {
		glBindTexture(GL_TEXTURE_2D, pathTex);
		glUniform1f(glGetUniformLocation(shaders[SHADERTYPE::Texture_UI]->GetProgram(), "brightness"), 1.0f);
	}
	
	fullscreenQuad->Draw();
	glEnable(GL_DEPTH_TEST);*/


}

void GraphicsPipeline::RenderUI()
{
	GUIsystem::Instance()->Draw();
}

void GraphicsPipeline::RenderPath()
{
	pathRenderNodes.clear();
	for (int i = 0; i < playerRenderNodes.size(); ++i)
	{
		RecursiveAddToPathRenderLists(playerRenderNodes[i]);
	}

	Matrix4 projMatrix2 = Matrix4::Orthographic(-40, 40, -groundSize.x, groundSize.x, -groundSize.y, groundSize.y);
	Matrix4	viewMatrix2 = Matrix4::Rotation(90, Vector3(1, 0, 0)) *Matrix4::Translation(Vector3(0.0f,-20.0f,0.0f));
	glViewport(0, 0, (GLsizei)(groundSize.x*PIXELPERSIZE), (GLsizei)(groundSize.y*PIXELPERSIZE));
	Matrix4 temp = projViewMatrix;
	projViewMatrix = projMatrix2 * viewMatrix2;

	// draw the object and do not clean the color
	glBindFramebuffer(GL_FRAMEBUFFER, pathFBO);
	static_cast<DrawPathMaterial*>(materials[MATERIALTYPE::Draw_Path])->SetProjViewMtx(projMatrix2 * viewMatrix2);
	for (int i = 0; i < pathRenderNodes.size(); i++)
	{
		pathRenderNodes[i]->DrawOpenGL(true, materials[MATERIALTYPE::Draw_Path]);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	projViewMatrix = temp;
}

void GraphicsPipeline::RenderPostprocessAndPresent()
{
	PostProcess::Instance()->RenderPostProcess();
	glUseProgram(0);
}

void GraphicsPipeline::InitPath(Vector2 _groundSize)
{
	if (pathTex == NULL)
		glDeleteTextures(1, &pathTex);

	this->groundSize = _groundSize;
	//Color Texture
	if (!pathTex) glGenTextures(1, &pathTex);
	glBindTexture(GL_TEXTURE_2D, pathTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8, (GLsizei)(groundSize.x*PIXELPERSIZE), (GLsizei)(groundSize.y*PIXELPERSIZE), 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

	//Generate our Framebuffer
	if (!pathFBO) glGenFramebuffers(1, &pathFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, pathFBO);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pathTex, 0);
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	GLenum buf = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &buf);
	//Validate our framebuffer
	GLuint status;
	if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
	{
		NCLERROR("Unable to create Screen Framebuffer! StatusCode: %x", status);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Score Init - I put this here because score only needs to be initialized if we initialize the path
	// and because it requires the same size
		if (!scoreBuffer) glGenBuffers(1, &scoreBuffer);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, scoreBuffer);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint) * 4, NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, scoreBuffer);
	glBindFramebuffer(GL_ATOMIC_COUNTER_BUFFER, 0);

	//Color Texture
	if (!scoreTex) glGenTextures(1, &scoreTex);
	glBindTexture(GL_TEXTURE_2D, scoreTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8, (GLsizei)(groundSize.x*PIXELPERSIZE), (GLsizei)(groundSize.y*PIXELPERSIZE), 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

	if (!scoreFBO) glGenFramebuffers(1, &scoreFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, scoreFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, scoreTex, 0);
	GLenum buf2 = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &buf2);

	if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
	{
		NCLERROR("Unable to create Screen Framebuffer! StatusCode: %x", status);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void GraphicsPipeline::RecursiveAddToPathRenderLists(RenderNode* node)
{
	PlayerRenderNode* p = dynamic_cast<PlayerRenderNode*>(node);
	if (p != nullptr)
	{
		if (!p->GetIsInAir())
		{
			pathRenderNodes.push_back(node);
		}
	}

	//Recurse over all children and process them aswell
	for (auto itr = node->GetChildIteratorStart(); itr != node->GetChildIteratorEnd(); itr++)
		RecursiveAddToPathRenderLists(*itr);
}

// Score - Alex - 27/02/2018
void GraphicsPipeline::CountScore()
{
	ResetScoreBuffer();

	glBindFramebuffer(GL_FRAMEBUFFER, scoreFBO);
	glUseProgram(shaders[SHADERTYPE::Score]->GetProgram());
	
	glBindTexture(GL_TEXTURE_2D, scoreTex);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shaders[SHADERTYPE::Score]->GetProgram(), "uPathTex"), 0);
	glBindTexture(GL_TEXTURE_2D, pathTex);

	fullscreenQuad->Draw();

	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, scoreBuffer);
	glGetBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint) * 4, scores);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);

	glUseProgram(0);
}

void GraphicsPipeline::ResetScoreBuffer()
{
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, scoreBuffer);
	GLuint a[4] = { 0,0,0,0 };
	glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint) * 4, a);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
}

// Minimap - philip 20/02/2018
void GraphicsPipeline::DrawMiniMap() {
	if (pathTex == NULL) {
		glDeleteTextures(1, &pathTex);
	}
	if (!pathTex) {
		glGenTextures(1, &pathTex);
		glUseProgram(shaders[SHADERTYPE::MiniMap]->GetProgram());
		glBindTexture(GL_TEXTURE_2D, pathTex);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//add textures
	glUseProgram(shaders[SHADERTYPE::MiniMap]->GetProgram());
	glBindTexture(GL_TEXTURE_2D, pathTex);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextureManager::Instance()->GetTexture(TEXTURETYPE::Paint_Pool));
	glUniform1i(glGetUniformLocation(shaders[SHADERTYPE::MiniMap]->GetProgram(), "poolTex"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, TextureManager::Instance()->GetTexture(TEXTURETYPE::Gun_Pickup));
	glUniform1i(glGetUniformLocation(shaders[SHADERTYPE::MiniMap]->GetProgram(), "gunTex"), 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, TextureManager::Instance()->GetTexture(TEXTURETYPE::Jump_Pickup));
	glUniform1i(glGetUniformLocation(shaders[SHADERTYPE::MiniMap]->GetProgram(), "jumpTex"), 3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, TextureManager::Instance()->GetTexture(TEXTURETYPE::Speed_Pickup));
	glUniform1i(glGetUniformLocation(shaders[SHADERTYPE::MiniMap]->GetProgram(), "speedTex"), 4);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, TextureManager::Instance()->GetTexture(TEXTURETYPE::Capture_Point));
	glUniform1i(glGetUniformLocation(shaders[SHADERTYPE::MiniMap]->GetProgram(), "captureTex"), 5);


	//these numbers are hardcoded at the moment but will be variables in the end
	float aspect = (float)width / height;
	float sx = 0.2;
	float sy = sx * aspect;

	glUniformMatrix4fv(glGetUniformLocation(shaders[SHADERTYPE::MiniMap]->GetProgram(), "modelMatrix"), 1, GL_FALSE,
		(float*)&(Matrix4::Translation(Vector3(-1.0f + sx, -1.0f + sy, 0.0f)) * Matrix4::Scale(Vector3(sx, sy, 1.0f))));
	uint playerCount = Game::Instance()->GetPlayerNumber();
	glUniform1ui(glGetUniformLocation(shaders[SHADERTYPE::MiniMap]->GetProgram(), "playerCount"), playerCount);
	
	//player uniforms///////////////
	//four Vector2s
	float players[8];
	//four Vector3s
	int colours[4];
	uint count = 0;
	for (int i = 0; i < 4; i++) {
		Avatar* a = Game::Instance()->GetPlayer(i);
		if (a) {
			//let the shader know which is the current player
			if (i == Game::Instance()->getUserID()) {
				glUniform1ui(glGetUniformLocation(shaders[SHADERTYPE::MiniMap]->GetProgram(), "self"), count);
			}
			//adding the player's positions
			Vector2 v = VectorToMapCoord(a->GetPosition());
			players[count * 2] = v.x;
			players[(count * 2) + 1] = v.y;
			//colours
			colours[count] = a->GetColour();
			//increment count inside here incase a player has left the game
			count++;
		}
	}
	glUniform2fv(glGetUniformLocation(shaders[SHADERTYPE::MiniMap]->GetProgram(), "players"), 4, players);
	glUniform1iv(glGetUniformLocation(shaders[SHADERTYPE::MiniMap]->GetProgram(), "colours"), 4, colours);


	//map feature uniforms/////////////////
	//get the current map
	Map* map = (Map*)SceneManager::Instance()->GetCurrentScene();
	//int array for pickup type
	uint pickupTypes[20];
	float pickupPositions[40];
	int pickupColours[20];
	//reset count
	count = 0;
	for (int i = 0; i < map->GetNPickup(); i++) {
		Pickup* p = map->GetPickups()[i];
		if (p->GetActive()) {
			pickupTypes[count] = p->GetPickupType();
			if (pickupTypes[count] == PickupType::PAINTPOOL) {
				pickupColours[count] = ((PaintPool*)map->GetPickups()[i])->GetColour();
			}
			Vector2 v = VectorToMapCoord(p->Physics()->GetPosition());
			pickupPositions[count * 2] = v.x;
			pickupPositions[(count * 2) + 1] = v.y;
			count++;
		}
	}
	//capturable object
	for (int i = 0; i < map->GetNCapture(); i++) {
		//four is one more than the highest number
		pickupTypes[count] = 4;
		pickupColours[count] = map->GetCaptureAreas()[i]->GetColour();
		Vector2 v = VectorToMapCoord(map->GetCaptureAreas()[i]->Physics()->GetPosition());
		pickupPositions[count * 2] = v.x;
		pickupPositions[(count * 2) + 1] = v.y;
		count++;
	}

	glUniform1ui(glGetUniformLocation(shaders[SHADERTYPE::MiniMap]->GetProgram(), "pickupCount"), count);
	glUniform1uiv(glGetUniformLocation(shaders[SHADERTYPE::MiniMap]->GetProgram(), "pickupTypes"), 20, pickupTypes);
	glUniform2fv(glGetUniformLocation(shaders[SHADERTYPE::MiniMap]->GetProgram(), "pickupPositions"), 20, pickupPositions);
	glUniform1iv(glGetUniformLocation(shaders[SHADERTYPE::MiniMap]->GetProgram(), "pickupColours"), 20, pickupColours);

	//pass the view angle through in radians
	glUniform1f(glGetUniformLocation(shaders[SHADERTYPE::MiniMap]->GetProgram(), "angle"), -(camera->GetYaw() + 180.0f)*PI/180.0f);
	//opacity of minimap, this will be a variable eventually
	glUniform1f(glGetUniformLocation(shaders[SHADERTYPE::MiniMap]->GetProgram(), "opacity"), 1.0);
	glUniform1f(glGetUniformLocation(shaders[SHADERTYPE::MiniMap]->GetProgram(), "zoom"), 0.7);
	//time
	glUniform1f(glGetUniformLocation(shaders[SHADERTYPE::MiniMap]->GetProgram(), "time"), time);

	//finally draw the minimap to the screen
	fullscreenQuad->Draw();
	glUseProgram(0);
}


Vector2 GraphicsPipeline::VectorToMapCoord(Vector3 pos) {
	Vector2 r;
	//get the x and y demensions of map
	float xDimension = ((Map*)(SceneManager::Instance()->GetCurrentScene()))->GetXDimension();
	float yDimension = ((Map*)(SceneManager::Instance()->GetCurrentScene()))->GetYDimension();

	r.x = (xDimension - pos.x) / (xDimension * 2);
	r.y = (yDimension + pos.z) / (yDimension * 2);

	return r;
}