#include <enet\enet.h>
#include <ncltech\PhysicsEngine.h>
#include <ncltech\SceneManager.h>
#include <nclgl\Window.h>
#include <nclgl\NCLDebug.h>
#include <nclgl\PerfTimer.h>
#include "UserInterface.h"
#include "AudioSystem.h"
#include "SimpleGamePlay.h"
#include "MainMenu.h"
#include "Arena.h"
#include "TestMap.h"
#include "GameInput.h"
#include "Game.h"
#include "PostProcess.h"


bool draw_debug = true;
bool draw_performance = false;

const Vector4 status_colour = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
const Vector4 status_colour_header = Vector4(0.8f, 0.9f, 1.0f, 1.0f);

bool show_perf_metrics = false;
PerfTimer timer_total, timer_physics, timer_update, timer_render, timer_audio;
uint shadowCycleKey = 4;

//Prevent multiple clicking from happening
int fpsCounter = 6;



// Program Deconstructor
//  - Releases all global components and memory
//  - Optionally prints out an error message and
//    stalls the runtime if requested.
void Quit(bool error = false, const std::string &reason = "") {
	//Release Singletons
	SceneManager::Release();
	PhysicsEngine::Release();
	PostProcess::Release();
	GUIsystem::Release();
	GraphicsPipeline::Release();
	enet_deinitialize();
	AudioSystem::Release();
	Window::Destroy();
	
	//Show console reason before exit
	if (error) {
		std::cout << reason << std::endl;
		system("PAUSE");
		exit(-1);
	}
}

//initialise all audio files
void InitialiseAudioFiles() {
	AudioSystem::Instance()->Create3DSound(MENU_MUSIC, "../AudioFiles/singing.wav", 0.5f, 30.0f);
	AudioSystem::Instance()->Create2DStream(GAME_MUSIC, "../AudioFiles/wave.mp3");
}

// Program Initialise
//  - Generates all program wide components and enqueues all scenes
//    for the SceneManager to display
void Initialize()
{
	//Initialise the Window
	if (!Window::Initialise("Game Technologies", 1280, 800, false))
		Quit(true, "Window failed to initialise!");


	//Initialize Renderer
	GraphicsPipeline::Instance();

	GUIsystem::Instance();

	PostProcess::Instance();
	//Initialise the PhysicsEngine
	PhysicsEngine::Instance();

	SceneManager::Instance()->EnqueueScene(new MainMenu("MainMenu - The worst menu ever!"));
	SceneManager::Instance()->EnqueueScene(new SimpleGamePlay("SimpleGamePlay - The Best Game Ever"));
	SceneManager::Instance()->EnqueueScene(new SimpleGamePlay("SimpleGamePlay - The Best Game Ever"));
	SceneManager::Instance()->EnqueueScene(new SimpleGamePlay("SimpleGamePlay - The Best Game Ever"));
	SceneManager::Instance()->EnqueueScene(new SimpleGamePlay("SimpleGamePlay - The Best Game Ever"));

	AudioSystem::Instance();
	InitialiseAudioFiles();
}

// Print Debug Info
//  - Prints a list of status entries to the top left
//    hand corner of the screen each frame.
void PrintStatusEntries()
{
	//Print Engine Options
	NCLDebug::AddStatusEntry(status_colour_header, "NCLTech Settings");
	NCLDebug::AddStatusEntry(status_colour, "     Physics Engine: %s (Press P to toggle)", PhysicsEngine::Instance()->IsPaused() ? "Paused  " : "Enabled ");
	NCLDebug::AddStatusEntry(status_colour, "     Monitor V-Sync: %s (Press V to toggle)", GraphicsPipeline::Instance()->GetVsyncEnabled() ? "Enabled " : "Disabled");
	NCLDebug::AddStatusEntry(status_colour, "");

	//Print Current Scene Name
	NCLDebug::AddStatusEntry(status_colour_header, "[%d/%d]: %s",
		SceneManager::Instance()->GetCurrentSceneIndex() + 1,
		SceneManager::Instance()->SceneCount(),
		SceneManager::Instance()->GetCurrentScene()->GetSceneName().c_str()
		);
	NCLDebug::AddStatusEntry(status_colour, "     \x01 T/Y to cycle or R to reload scene");

	//Print Performance Timers
	NCLDebug::AddStatusEntry(status_colour, "     FPS: %5.2f  (Press G for %s info)", 1000.f / timer_total.GetAvg(), show_perf_metrics ? "less" : "more");
	if (show_perf_metrics)
	{
		timer_total.PrintOutputToStatusEntry(status_colour, "          Total Time     :");
		timer_update.PrintOutputToStatusEntry(status_colour, "          Scene Update   :");
		timer_physics.PrintOutputToStatusEntry(status_colour, "          Physics Update :");
		PhysicsEngine::Instance()->PrintPerformanceTimers(status_colour);
		timer_render.PrintOutputToStatusEntry(status_colour, "          Render Scene   :");
		timer_audio.PrintOutputToStatusEntry(status_colour, "         Audio Update   :");
	}

	const Vector4 status_color_debug = Vector4(1.0f, 0.6f, 1.0f, 1.0f);

	//Physics Debug Drawing options
	if (Game::Instance()->IsRunning())
	{
		NCLDebug::AddStatusEntry(status_colour_header, "Score");
		NCLDebug::AddStatusEntry(status_colour, "     Score = " + to_string(Game::Instance()->GetScore(0)));
		NCLDebug::AddStatusEntry(status_colour, "     Score = " + to_string(Game::Instance()->GetScore(1)));
		NCLDebug::AddStatusEntry(status_colour, "     Score = " + to_string(Game::Instance()->GetScore(2)));
		NCLDebug::AddStatusEntry(status_colour, "     Score = " + to_string(Game::Instance()->GetScore(3)));

	}

	uint drawFlags = PhysicsEngine::Instance()->GetDebugDrawFlags();
	NCLDebug::AddStatusEntry(status_color_debug, "--- Debug Draw  [G] ---");
	if (draw_debug)
	{
		NCLDebug::AddStatusEntry(status_color_debug, "Constraints       : %s [Z] - Tut 3+", (drawFlags & DEBUGDRAW_FLAGS_CONSTRAINT) ? "Enabled " : "Disabled");
		NCLDebug::AddStatusEntry(status_color_debug, "Collision Normals : %s [X] - Tut 4", (drawFlags & DEBUGDRAW_FLAGS_COLLISIONNORMALS) ? "Enabled " : "Disabled");
		NCLDebug::AddStatusEntry(status_color_debug, "Collision Volumes : %s [C] - Tut 4+", (drawFlags & DEBUGDRAW_FLAGS_COLLISIONVOLUMES) ? "Enabled " : "Disabled");
		NCLDebug::AddStatusEntry(status_color_debug, "Manifolds         : %s [V] - Tut 5+", (drawFlags & DEBUGDRAW_FLAGS_MANIFOLD) ? "Enabled " : "Disabled");
		NCLDebug::AddStatusEntry(status_color_debug, "OCtree            : %s [B]", (drawFlags & DEBUGDRAW_FLAGS_FIXED_WORLD) ? "Enabled " : "Disabled");
		//NCLDebug::AddStatusEntry(status_color_debug, "Bounding          : %s [N]", (drawFlags & DEBUGDRAW_FLAGS_BOUNDING) ? "Enabled " : "Disabled");

		NCLDebug::AddStatusEntry(status_color_debug, "");
	}
	NCLDebug::AddStatusEntry(status_colour, "");

}


static bool ScoreCallbackFunction(PhysicsNode * self, PhysicsNode* collidingObject) {
	return true;
}

// Process Input
//  - Handles all program wide keyboard inputs for
//    things such toggling the physics engine and 
//    cycling through scenes.
void HandleKeyboardInputs()
{
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_P))
		PhysicsEngine::Instance()->SetPaused(!PhysicsEngine::Instance()->IsPaused());

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_V))
		GraphicsPipeline::Instance()->SetVsyncEnabled(!GraphicsPipeline::Instance()->GetVsyncEnabled());

	uint sceneIdx = SceneManager::Instance()->GetCurrentSceneIndex();
	uint sceneMax = SceneManager::Instance()->SceneCount();

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_R))
	{
		Game::Instance()->ResetGame();
		SceneManager::Instance()->JumpToScene(0);
	}
		

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_G))
		show_perf_metrics = !show_perf_metrics;

	//audio test functionality
	//TODO remove this when finished testing
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_8)) {	
		AudioSystem::Instance()->PlayASound(GAME_MUSIC, true, { 4.0f, 0.0f, 0.0f });
	}

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_1)) {
		AudioSystem::Instance()->PlayASound(MENU_MUSIC, false, { 0.0f, 0.0f, -15.0f });
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_2)) {
		AudioSystem::Instance()->PlayASound(MENU_MUSIC, false, { 15.0f, 0.0f, 0.0f });
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_3)) {
		AudioSystem::Instance()->PlayASound(MENU_MUSIC, false, { -15.0f, 0.0f, 0.0f });
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_4)) {
		AudioSystem::Instance()->PlayASound(MENU_MUSIC, false, { 0.0f, 0.0f, 15.0f });
	}

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_7)) {
		AudioSystem::Instance()->StopAllSounds();
	}

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_6)) {
		AudioSystem::Instance()->UnmuteAllSounds();
	}



	uint drawFlags = PhysicsEngine::Instance()->GetDebugDrawFlags();
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_Z))
		drawFlags ^= DEBUGDRAW_FLAGS_CONSTRAINT;

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_X))
		drawFlags ^= DEBUGDRAW_FLAGS_COLLISIONNORMALS;

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_C))
		drawFlags ^= DEBUGDRAW_FLAGS_COLLISIONVOLUMES;

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_V))
		drawFlags ^= DEBUGDRAW_FLAGS_MANIFOLD;

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_B))
		drawFlags ^= DEBUGDRAW_FLAGS_FIXED_WORLD;

	//if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_N))
		//drawFlags ^= DEBUGDRAW_FLAGS_BOUNDING;

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_J))
	{
		GameObject* spawnSphere = CommonUtils::BuildSphereObject("spawned_sphere",
			GraphicsPipeline::Instance()->GetCamera()->GetPosition() + GraphicsPipeline::Instance()->GetCamera()->GetViewDirection().Normalise()*2.0f,
			0.5f,									//Radius
			true,									//Has Physics Object
			1.0f / 4.0f,							//Inverse Mass
			true,									//Has Collision Shape
			true,									//Dragable by the user
			DEFAULT_PHYSICS,
			CommonUtils::GenColor(0.1f, 0.8f));		//Color


		spawnSphere->Physics()->SetLinearVelocity(GraphicsPipeline::Instance()->GetCamera()->GetViewDirection().Normalise()*50.0f);

		spawnSphere->Physics()->SetOnCollisionCallback(&ScoreCallbackFunction);


		SceneManager::Instance()->GetCurrentScene()->AddGameObject(spawnSphere);
	}

	//toggle the camera
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_L)) {
		SceneManager::Instance()->GetCurrentScene()->ToggleCamera();
	}

	Input::Instance()->SetInput(FORWARD, Window::GetKeyboard()->KeyDown(KEYBOARD_W) || Window::GetKeyboard()->KeyDown(KEYBOARD_UP));
	Input::Instance()->SetInput(BACKWARD, Window::GetKeyboard()->KeyDown(KEYBOARD_S) || Window::GetKeyboard()->KeyDown(KEYBOARD_DOWN));
	Input::Instance()->SetInput(LEFT, Window::GetKeyboard()->KeyDown(KEYBOARD_A) || Window::GetKeyboard()->KeyDown(KEYBOARD_LEFT));
	Input::Instance()->SetInput(RIGHT, Window::GetKeyboard()->KeyDown(KEYBOARD_D) || Window::GetKeyboard()->KeyDown(KEYBOARD_RIGHT));
	Input::Instance()->SetInput(JUMP, Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE));
	Input::Instance()->SetInput(PAUSE, Window::GetKeyboard()->KeyDown(KEYBOARD_P));
	Input::Instance()->SetInput(SHOOT, Window::GetMouse()->ButtonDown(MOUSE_LEFT) && !Window::GetMouse()->ButtonHeld(MOUSE_LEFT));
	//possibly temporary
	Input::Instance()->SetInput(CAMERA_UP, Window::GetKeyboard()->KeyDown(KEYBOARD_SHIFT));
	Input::Instance()->SetInput(CAMERA_DOWN, Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE));

	//mouse input
	Input::Instance()->SetLookX(Window::GetMouse()->GetRelativePosition().x);
	Input::Instance()->SetLookY(Window::GetMouse()->GetRelativePosition().y);

	PhysicsEngine::Instance()->SetDebugDrawFlags(drawFlags);
//	GraphicsPipeline::Instance()->SetDebugDrawFlags(drawFlags);
}

void HandleGUIMouseCursor()
{
	Vector2 absPos;
	Window::GetWindow().GetMouseScreenPos(&absPos);
	GUIsystem::Instance()->HandleMousePosition(absPos.x, absPos.y);
}

void HandleGUIMouseButton()
{
	//Score bar function, temporaily here
	if (GUIsystem::Instance()->GetDrawScoreBar()==true) {
		float a1 = (Game::Instance()->GetScore(0));
		float a2 = (Game::Instance()->GetScore(1));
		float a3 = (Game::Instance()->GetScore(2));
		float a4 = (Game::Instance()->GetScore(3));
		GUIsystem::Instance()->UpdateScorebar(a1, a2, a3, a4);
		//update the weapon bar part of the interface
		Avatar* p = Game::Instance()->GetPlayer(Game::Instance()->getUserID());
		GUIsystem::Instance()->SetHasWeapon(p->GetWeaponActive());
		GUIsystem::Instance()->SetCurrentWeapon(p->GetWeapon());
		//only needs setting once
		GUIsystem::Instance()->SetPlayerColour(p->GetColourRGBA().ToVector3());
	}

	fpsCounter++;
	if (fpsCounter > 5) {
		if (Window::GetMouse()->ButtonDown(MOUSE_LEFT))
		{
			GUIsystem::Instance()->onMouseButtonPressed(MOUSE_LEFT);
		}
		else if (Window::GetMouse()->ButtonDown(MOUSE_RIGHT))
		{
			GUIsystem::Instance()->onMouseButtonPressed(MOUSE_RIGHT);
		}
		fpsCounter = 0;
	}
	GUIsystem::Instance()->onMouseButtonHold(Window::GetMouse()->ButtonHeld(MOUSE_LEFT));
}

//Handle GUI text input
void HandleGUITextInput()
{
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_RETURN)) {
		GUIsystem::Instance()->HandleTextInput(KEYBOARD_RETURN);
		GUIsystem::Instance()->SetIsTyping(false);
		return;
	}
	else if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_SPACE)) {
		GUIsystem::Instance()->HandleTextInput(KEYBOARD_SPACE);
		return;
	}
	else if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_BACK)) {
		GUIsystem::Instance()->HandleTextInput(KEYBOARD_BACK);
		return;
	}
	for (int i = KeyboardKeys::KEYBOARD_0; i <= KeyboardKeys::KEYBOARD_PERIOD; i++) {
		if (Window::GetKeyboard()->KeyTriggered(static_cast<KeyboardKeys>(i))) {
			GUIsystem::Instance()->HandleTextInput(static_cast<KeyboardKeys>(i));
			break; 
		}
	}
}

void TestPostProcess()
{
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_F1)) {
		PostProcess::Instance()->SetPostProcessType(PostProcessType::HDR_BLOOM);
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_F2)) {
		PostProcess::Instance()->SetPostProcessType(PostProcessType::INVERSION);
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_F3)) {
		PostProcess::Instance()->SetPostProcessType(PostProcessType::GRAYSCALE);
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_F4)) {
		PostProcess::Instance()->SetPostProcessType(PostProcessType::SHARPEN);
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_F5)) {
		PostProcess::Instance()->SetPostProcessType(PostProcessType::BLUR);
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_F6)) {
		PostProcess::Instance()->SetPostProcessType(PostProcessType::EDGE_DETECTION);
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_F7)) {
		PostProcess::Instance()->SetPostProcessType(PostProcessType::BASIC);
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_F8)) {
		PostProcess::Instance()->SetPostProcessType(PostProcessType::PERFORMANCE_BLUR);
	}
}

// Program Entry Point
int main()
{
	//Initialize our Window, Physics, Scenes etc
	Initialize();
	Game::Instance();
	GraphicsPipeline::Instance()->SetVsyncEnabled(true);

	Window::GetWindow().GetTimer()->GetTimedMS();

	//lock mouse so moving around the screen is nicer
	Window::GetWindow().LockMouseToWindow(false);
	Window::GetWindow().ShowOSPointer(false);
	//Create main game-loop
	while (Window::GetWindow().UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE) 
		&& SceneManager::Instance()->GetExitButtonClicked() == false) 
	{
		float dt = Window::GetWindow().GetTimer()->GetTimedMS() * 0.001f;	//How many milliseconds since last update?
																			//Update Performance Timers (Show results every second)
		
		GraphicsPipeline::Instance()->GetCamera()->SetPosition(Vector3(0, 1, 0));

		timer_total.UpdateRealElapsedTime(dt);
		timer_physics.UpdateRealElapsedTime(dt);
		timer_update.UpdateRealElapsedTime(dt);
		timer_render.UpdateRealElapsedTime(dt);
		timer_audio.UpdateRealElapsedTime(dt);

		//Handle GUI mouseCursor
		HandleGUIMouseCursor();

		//Handle GUI mouseButton
		HandleGUIMouseButton();

		//Print Status Entries
		PrintStatusEntries();

		//Test Post Process
		TestPostProcess();

		//Handle Keyboard Inputs
		if (GUIsystem::Instance()->GetIsTyping() == false) {
			HandleKeyboardInputs();
		}
		else {
			//Handle User Typing input
			HandleGUITextInput();
		}

		timer_total.BeginTimingSection();

		//Update Scene
		timer_update.BeginTimingSection();
		SceneManager::Instance()->GetCurrentScene()->OnUpdateScene(dt);
		timer_update.EndTimingSection();

		Game::Instance()->Update(dt);

		//Update Physics	
		timer_physics.BeginTimingSection();
		PhysicsEngine::Instance()->Update(dt);
		timer_physics.EndTimingSection();
		PhysicsEngine::Instance()->DebugRender();

		//Render Scene
		timer_render.BeginTimingSection();
		GraphicsPipeline::Instance()->UpdateScene(dt);
		//		GraphicsPipeline::Instance()->DebugRender();
		GraphicsPipeline::Instance()->RenderScene();
		{
			//Forces synchronisation if vsync is disabled
			// - This is solely to allow accurate estimation of render time
			// - We should NEVER normally lock our render or game loop!		
			//	glClientWaitSync(glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, NULL), GL_SYNC_FLUSH_COMMANDS_BIT, 1000000);
		}
		timer_render.EndTimingSection();

		PhysicsEngine::Instance()->CleanUpPhase();

		timer_audio.BeginTimingSection();
		AudioSystem::Instance()->Update(GraphicsPipeline::Instance()->GetCamera()->GetPosition(), GraphicsPipeline::Instance()->GetCamera()->GetViewDirection(), GraphicsPipeline::Instance()->GetCamera()->GetUpDirection(), dt);
		timer_audio.EndTimingSection();
		

		//Finish Timing
		timer_total.EndTimingSection();
		//}


	}

	//Cleanup
	Quit();
	return 0;
}
