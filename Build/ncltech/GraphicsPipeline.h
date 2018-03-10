
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
#pragma once
#include <nclgl\OGLRenderer.h>
#include <nclgl\TSingleton.h>
#include <nclgl\Camera.h>
#include <nclgl\RenderNode.h>
#include <nclgl\Frustum.h>
#include <nclgl\Mouse.h>
#include <PC\UserInterface.h>
#include <PC\PostProcess.h>
#include <nclgl\PerfTimer.h>

//material
#include <nclgl\Material.h>
#include <nclgl\StandardMaterial.h>
#include <nclgl\ShadowMaterial.h>
#include <nclgl\PostprocessMaterial.h>
#include <nclgl\PlayerRenderNode.h>


//---------------------------
//------ Base Renderer ------
//---------------------------
//------ Use this as a base for incorporating your own graphics
//
//class GraphicsPipeline : public TSingleton<GraphicsPipeline>, OGLRenderer
//{
//	friend class TSingleton<GraphicsPipeline>;
//
//public:
//	//Set all defaults
//	// - Called each time a scene is swapped in/reset, so should reset camera position etc
//	void InitializeDefaults();
//
//	//Add/Remove Render objects to renderable object lists
//	void AddRenderNode(RenderNode* node);
//	void RemoveRenderNode(RenderNode* node);
//
//
//
//
//	//Called by main game loop
//	// - Naming convention from oglrenderer
//	void UpdateScene(float dt);
//	void RenderScene();
//
//
//
//	//Utils
//	inline Camera* GetCamera();
//
//protected:
//	GraphicsPipeline() : OGLRenderer(Window::GetWindow()) {}
//	virtual ~GraphicsPipeline() {}
//
//protected:
//};




//-------------------
// Default Implementation
//-----------------------
// This is a GameTech module, not graphics. This default renderer
// should be good enough for all physics and AI madness you want 
// to mess around with.



//Number of cascading shadow maps 
// - As we don't have the ability to set shadow defines, any changes here also need to be
//   mirrored inside "TechFragForwardRender.glsl" and "TechGeomShadow.glsl" shaders.
#define SHADOWMAP_NUM 4			

//Size of the shadows maps in pixels
// - With a size of 4096x4096 (and 4 shadowmaps) using 32bit floats this
//   currently results in shadows using up 256MB of space. Which is quite alot,
//   but also currently the only potentially memory sensitive thing we do in this 
//   renderer so it's fine.
#define SHADOWMAP_SIZE 4096


#define SHADOW_PROJ_FAR      50.0f			//Can see for 50m - setting this too far really hurts shadow quality as they attempt to cover the entirety of the view frustum
#define SHADOW_PROJ_NEAR     0.1f			//Nearest object @ 10cm
#define SHADOW_PROJ_FOV      45.0f			//45 degree field of view

#define CAMERA_PROJ_FAR      5000.0f		//Can see for 5000m - setting this too far really hurts shadow quality as they attempt to cover the entirety of the view frustum
#define CAMERA_PROJ_NEAR     0.1f			//Nearest object @ 10cm
#define CAMERA_PROJ_FOV      45.0f			//45 degree field of view

#define PATHMAP_SIZE 2000
#define CAPTURE_SIZE 40	 
#define PIXELPERSIZE 100

#define ATOMIC_COUNTER_NUMBER 4

#define DEBUGDRAW_FLAGS_BOUNDING				0x20

typedef std::pair<RenderNode*, float> RenderNodePair;

enum SHADERTYPE
{
	Present_To_Window	= 0,
	Shadow				= 1,
	Forward_Lighting	= 2,
	Texture_UI			= 3,
	Draw_Path			= 4,
	Ground				= 5,
	SkyBox				= 6,
	MiniMap				= 7,
	Score				= 8,
	Shader_Number,
};

typedef SHADERTYPE MATERIALTYPE;
const short Material_Number = Shader_Number;

class GraphicsPipeline : public TSingleton<GraphicsPipeline>, OGLRenderer
{
	friend class TSingleton<GraphicsPipeline>;

public:
	//Set all defaults
	// - Called each time a scene is swapped in/reset, so should reset camera position etc
	void InitializeDefaults();

	//Add/Remove Render objects to renderable object lists
	void AddRenderNode(RenderNode* node);
	void RemoveRenderNode(RenderNode* node);

	inline uint GetDebugDrawFlags() const { return debugDrawFlags; }
	inline void SetDebugDrawFlags(uint flags) { debugDrawFlags = flags; }
	//Debug draw all bounding radius
	void DebugRender();

	//Called by main game loop
	// - Naming convention from oglrenderer
	virtual void UpdateScene(float dt) override;
	virtual void RenderScene() override;

	//Utils
	inline Camera* GetCamera() { return camera; }
	inline bool GetVsyncEnabled() const { return isVsyncEnabled; }
	inline void SetVsyncEnabled(bool enabled) { wglSwapIntervalEXT((isVsyncEnabled = enabled) ? 1 : 0); }

	inline Matrix4& GetProjMtx() { return projMatrix; }
	inline Matrix4& GetViewMtx() { return viewMatrix; }
	inline Matrix4& GetProjViewMtx() { return projViewMatrix; }

	inline float*   GetNormalizedFarPlanes() { return normalizedFarPlanes; }
	inline Matrix4& GetShadowViewMtx() { return shadowViewMtx; }
	inline Matrix4* GetShadowProjMatrices() { return shadowProj; }
	inline Matrix4* GetShadowProjViewMatrices() { return shadowProjView; }

	inline Vector3& GetAmbientColor() { return ambientColor; }
	inline Vector3& GetLightDirection() { return lightDirection; }
	inline float& GetSpecularFactor() { return specularFactor; }
	inline GLuint& GetShadowTex() { return shadowTex; }

	inline Shader** GetAllShaders() { return shaders; }
	inline Material** GetAllMaterials() { return materials; }

	inline void AddPlayerRenderNode(RenderNode* playerRenderNode){ playerRenderNodes.push_back(playerRenderNode); }
	void RemovePlayerRenderNode(RenderNode* node);

	inline void RemoveAllPlayerRenderNode() { playerRenderNodes.clear(); }
	void InitPath(Vector2 groundSize);
	inline GLuint& GetPathTex() { return pathTex; }

	//GUI
	void SetIsMainMenu(bool a) { isMainMenu = a; }
	bool GetIsMainMenu() { return isMainMenu; }
	void ResetPath();

	inline GLuint GetScreenTexWidth() { return screenTexWidth; }
	inline GLuint GetScreenTexHeight() { return screenTexHeight; }
	inline GLuint GetscreenFBO() { return screenFBO; }
	inline uint GetNumSuperSamples() { return numSuperSamples; }
	inline GLuint GetScreenTexColor1() { return screenTexColor[0]; }
	inline GLuint GetScreenTexColor2() { return screenTexColor[1]; }
	float GetGammaCorrection() { return gammaCorrection; }
	inline int GetWidth() { return width; }
	inline int GetHeight() { return height; }
	inline Mesh* GetScreenQuad() { return fullscreenQuad; }
	//Score
	inline float GetScore(uint i) { return (float)scores[i]; }

	void PrintPerformanceTimers(const Vector4& color)
	{
		perfShadow.PrintOutputToStatusEntry(color,		"              Shadows        :");
		perfObjects.PrintOutputToStatusEntry(color,		"              Objects        :");
		perfPostProcess.PrintOutputToStatusEntry(color, "              PostProcess    :");
		perfScoreandMap.PrintOutputToStatusEntry(color, "              Score & Map    :");
	}

protected:
	GraphicsPipeline();
	virtual ~GraphicsPipeline();

	virtual void Resize(int x, int y) override; //Called by window when it is resized

	void LoadShaders();
	void LoadMaterial();
	void UpdateAssets(int width, int height);
	void BuildAndSortRenderLists();
	void RecursiveAddToRenderLists(RenderNode* node);
	void RenderAllObjects(bool isShadowPass, std::function<void(RenderNode*)> perObjectFunc = NULL);
	void BuildShadowTransforms(); //Builds the shadow projView matrices

	void RenderShadow();
	void RenderObject();
	void RenderUI();
	void RenderPath();
	void RenderPostprocessAndPresent();
	//Phil 20/02/2018
	void DrawMiniMap();
	//Alex 27/02/2018
	void CountScore();
	void ResetScoreBuffer();
	
	void RecursiveAddToPathRenderLists(RenderNode* node);

protected:
	Matrix4 projViewMatrix;

	//Render FBO
	GLuint		screenTexWidth, screenTexHeight;
	GLuint		screenFBO;
	GLuint		screenTexColor[2];
	GLuint		screenTexDepth;

	//Shaders
	Shader**	shaders;
	//Material
	Material**	materials;

	//Render Params
	Vector3		ambientColor;
	float		gammaCorrection;	//Monitor Default: 1.0 / 2.2 (Where 2.2 here is the gamma of the monitor which we need to invert before doing lighting calculations)		
	Vector3		lightDirection;
	Vector3		backgroundColor;
	float		specularFactor;
	uint		numSuperSamples;


	//Shadowmaps
	float		sceneBoundingRadius; ///Approx based on scene contents
	GLuint		shadowFBO;
	GLuint		shadowTex;
	Matrix4		shadowProj[SHADOWMAP_NUM];
	Matrix4		shadowViewMtx;
	Matrix4		shadowProjView[SHADOWMAP_NUM];
	float		normalizedFarPlanes[SHADOWMAP_NUM - 1];

	//Common
	uint		debugDrawFlags;
	Frustum		frameFrustum;
	Mesh*		fullscreenQuad;
	Camera*		camera;
	bool		isVsyncEnabled;

	std::vector<RenderNode*> allNodes;

	std::vector<RenderNodePair> renderlistOpaque;
	std::vector<RenderNodePair> renderlistTransparent;	//Also stores cameraDist in the second argument for sorting purposes

	//path
	Vector2		groundSize;
	Vector3		lastPath[4];

	void SetPath(RenderNode* playerRenderNode, uint playerNumber);
	void SetupPathSmoother();
	
	std::vector<RenderNode*>	playerRenderNodes;
	std::vector<RenderNode*>	pathRenderNodes;
	std::vector<RenderNode*>    pathSmoother;
	
	GLuint		pathFBO;
	GLuint		pathTex;
	

	//Score - Alex 27/02/2018
	GLuint		scoreFBO;
	GLuint		scoreTex;
	GLuint		scoreBuffer;
	uint		scores[4];

	float accumTime = 0.0f;
	//For minimap
	float time;
	//translates a world position into a position for the minimap
	Vector2 VectorToMapCoord(Vector3 pos);
	//GUI
	bool isMainMenu = false;

	//--------------------------------------------------------------------------------------------//	Fragkas Nikolaos
	// Performance Timers																				Date: 02/03/2018
	//--------------------------------------------------------------------------------------------//	
	PerfTimer perfShadow;
	PerfTimer perfObjects;
	PerfTimer perfPostProcess;
	PerfTimer perfScoreandMap;
};

