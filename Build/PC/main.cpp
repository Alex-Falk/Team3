#include <enet\enet.h>
#include <ncltech\SceneManager.h>
#include <nclgl\Window.h>
#include <nclgl\NCLDebug.h>
#include <nclgl\PerfTimer.h>
#include "UserInterface.h"
#include "AudioSystem.h"
#include "GameInput.h"
#include "MinionStates.h"

//Scenes
#include "MainMenu.h"
#include "SimpleGamePlay.h"
#include "DataDrivenMap.h"


bool draw_debug = true;
bool draw_performance = false;

const Vector4 status_colour = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
const Vector4 status_colour_header = Vector4(0.8f, 0.9f, 1.0f, 1.0f);

bool show_full_perf_metrics = false;
bool show_debug = false;

PerfTimer timer_total, timer_physics, timer_update, timer_render, timer_audio, timer_gameplay;
uint shadowCycleKey = 4;

//Prevent multiple clicking from happening
int fpsCounter = 0;

void ReleaseAIStates()
{
	MinionStateCaptureZone::Release();
	MinionStateChaseEnemyPlayer::Release();
	MinionStateEscapeRocket::Release();
	MinionStateHealFriendlyPlayer::Release();
	MinionStateWander::Release();
}

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
	ReleaseAIStates();
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
	//TODO get actual audio files
	//TODO place the remaining sounds
	AudioSystem::Instance()->Create2DStream(MENU_MUSIC, SOUNDSDIR"menuMusic.mp3");
	AudioSystem::Instance()->Create2DStream(GAME_MUSIC, SOUNDSDIR"gameMusic.mp3");
	AudioSystem::Instance()->Create3DSound(JUMP_SOUND, SOUNDSDIR"jumpSound.mp3", 5.0f, 80.0f);//need placement
	AudioSystem::Instance()->Create3DSound(ROCKET_FLYING_SOUND, SOUNDSDIR"singing.wav",10.0f, 80.0f);
	AudioSystem::Instance()->Create3DSound(EXPLOSION_SOUND, SOUNDSDIR"explosionSound.mp3", 5.0f, 80.0f);	
	AudioSystem::Instance()->Create3DSound(PROJECTILE_LAUNCH_SOUND, SOUNDSDIR"projectileLaunchSound.mp3", 10.0f, 80.0f);
	AudioSystem::Instance()->Create2DSound(MENU_CHOICE_SOUND, SOUNDSDIR"menuChoiceSound.mp3");//need placement
	AudioSystem::Instance()->Create3DSound(ROLLING_SOUND, SOUNDSDIR"rollingSound.mp3", 5.0f, 80.0f);//need placement
	AudioSystem::Instance()->Create3DSound(PICKUP_COLLECTED_SOUND, SOUNDSDIR"pickupCollectedSound.mp3", 5.0f, 80.0f);
	AudioSystem::Instance()->Create2DSound(TIMER_RUNOUT_SOUND, SOUNDSDIR"timerRunoutSound.mp3");//need placement
	AudioSystem::Instance()->Create2DSound(VICTORY_SOUND, SOUNDSDIR"victorySound.mp3");//need placement
	AudioSystem::Instance()->Create3DSound(CAPTURE_AREA_SOUND, SOUNDSDIR"captureAreaSound.mp3", 5.0f, 80.0f);//need placement
}

// Program Initialise
//  - Generates all program wide components and enqueues all scenes
//    for the SceneManager to display
void Initialize()
{
	//Initialise the Window
	if (!Window::Initialise("Game Technologies", 1440, 900, false))
		Quit(true, "Window failed to initialise!");

	//Initialize Renderer
	GraphicsPipeline::Instance();

	//GUIsystem::Instance();

	PostProcess::Instance();
	
	//Initialise the PhysicsEngine
	PhysicsEngine::Instance();
	AudioSystem::Instance();
	InitialiseAudioFiles();

	SceneManager::Instance()->EnqueueScene(new MainMenu("MainMenu - Dongli's Angels!", "MainMenu"));
	SceneManager::Instance()->EnqueueScene(new DataDrivenMap("SimpleGamePlay - Dongli's Angels", "map1"));
	SceneManager::Instance()->EnqueueScene(new DataDrivenMap("SimpleGamePlay - The Best Game Ever", "map2"));
	SceneManager::Instance()->EnqueueScene(new DataDrivenMap("SimpleGamePlay - The Best Game Ever", "map3"));
	SceneManager::Instance()->EnqueueScene(new DataDrivenMap("SimpleGamePlay - The Best Game Ever", "map2"));
	//SceneManager::Instance()->EnqueueScene(new MapOne("Fourth Stage - The Best Game Ever"));

	

	GUIsystem::Instance()->SetUpLoadingScreen();
}

// Print Debug Info
//  - Prints a list of status entries to the top left
//    hand corner of the screen each frame.
void PrintStatusEntries()
{
	const Vector4 status_color_debug = Vector4(1.0f, 0.6f, 1.0f, 1.0f);
//	NCLDebug::AddStatusEntry(status_color_debug, "--- Debug Draw  [0] ---");
	if (!show_debug)
		return;

	////Print Engine Options
	//NCLDebug::AddStatusEntry(status_colour_header, "NCLTech Settings");
	//NCLDebug::AddStatusEntry(status_colour, "     Physics Engine: %s (Press P to toggle)", PhysicsEngine::Instance()->IsPaused() ? "Paused  " : "Enabled ");
	//NCLDebug::AddStatusEntry(status_colour, "     Monitor V-Sync: %s (Press V to toggle)", GraphicsPipeline::Instance()->GetVsyncEnabled() ? "Enabled " : "Disabled");
	//NCLDebug::AddStatusEntry(status_colour, "");

	////Print Current Scene Name
	//NCLDebug::AddStatusEntry(status_colour_header, "[%d/%d]: %s",
	//	SceneManager::Instance()->GetCurrentSceneIndex() + 1,
	//	SceneManager::Instance()->SceneCount(),
	//	SceneManager::Instance()->GetCurrentScene()->GetSceneName().c_str()
	//	);
	//NCLDebug::AddStatusEntry(status_colour, "     \x01 T/Y to cycle or R to reload scene");

	//Print Performance Timers


	NCLDebug::AddStatusEntry(status_colour, "     FPS: %5.2f  (Press 9 for %s info)", 1000.f / timer_total.GetAvg(), show_full_perf_metrics ? "less" : "more");
	if (!show_full_perf_metrics) {
		timer_total.PrintOutputToStatusEntry(status_colour,		"          Total Time        :");
		timer_physics.PrintOutputToStatusEntry(status_colour,	"            Physics Update  :");
		timer_render.PrintOutputToStatusEntry(status_colour,	"            Renderer Update :");
		timer_update.PrintOutputToStatusEntry(status_colour,	"            Gameplay Update :");
		Game::Instance()->PrintPerformanceTimers(status_colour);//           NetworkUpdate
		timer_audio.PrintOutputToStatusEntry(status_colour,		"            Audio Update    :");
	}
	else 
	{
		timer_total.PrintOutputToStatusEntry(status_colour,		"          Total Time        :");
		timer_physics.PrintOutputToStatusEntry(status_colour,	"            Physics Update  :");
		PhysicsEngine::Instance()->PrintPerformanceTimers(status_colour);
		timer_render.PrintOutputToStatusEntry(status_colour,	"            Render Scene    :");
		GraphicsPipeline::Instance()->PrintPerformanceTimers(status_colour);
		timer_update.PrintOutputToStatusEntry(status_colour,	"            Gameplay update :");
		SceneManager::Instance()->GetCurrentScene()->PrintPerformanceTimers(status_colour);
		Game::Instance()->PrintPerformanceTimers(status_colour);//           NetworkUpdate
		timer_audio.PrintOutputToStatusEntry(status_colour,		"            Audio Update    :");
	}

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
	if (draw_debug)
	{
		NCLDebug::AddStatusEntry(status_color_debug, "Constraints       : %s [Z] ", (drawFlags & DEBUGDRAW_FLAGS_CONSTRAINT) ? "Enabled " : "Disabled");
		NCLDebug::AddStatusEntry(status_color_debug, "Collision Normals : %s [X] ", (drawFlags & DEBUGDRAW_FLAGS_COLLISIONNORMALS) ? "Enabled " : "Disabled");
		NCLDebug::AddStatusEntry(status_color_debug, "Collision Volumes : %s [C] ", (drawFlags & DEBUGDRAW_FLAGS_COLLISIONVOLUMES) ? "Enabled " : "Disabled");
		NCLDebug::AddStatusEntry(status_color_debug, "Manifolds         : %s [V] ", (drawFlags & DEBUGDRAW_FLAGS_MANIFOLD) ? "Enabled " : "Disabled");
		NCLDebug::AddStatusEntry(status_color_debug, "World Partition   : %s [B] ", (drawFlags & DEBUGDRAW_FLAGS_FIXED_WORLD) ? "Enabled " : "Disabled");
		NCLDebug::AddStatusEntry(status_color_debug, "Bounding          : %s [N]", (drawFlags & DEBUGDRAW_FLAGS_BOUNDING) ? "Enabled " : "Disabled");

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
void HandleMouseAndKeyboardInputs(bool handleMouse, bool handleKeyBoard)
{
	if (handleKeyBoard)
	{
		//if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_P))
		//	PhysicsEngine::Instance()->SetPaused(!PhysicsEngine::Instance()->IsPaused());

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_V))
			GraphicsPipeline::Instance()->SetVsyncEnabled(!GraphicsPipeline::Instance()->GetVsyncEnabled());

		uint sceneIdx = SceneManager::Instance()->GetCurrentSceneIndex();
		uint sceneMax = SceneManager::Instance()->SceneCount();

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_ESCAPE))
		{
			if (Game::Instance()->IsRunning())
			{
				((Map*)Game::Instance()->GetMap())->showPauseMenu();
			}
			else
			{
				Game::Instance()->ResetGame();
				SceneManager::Instance()->JumpToScene(0);
			}

		}


		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_9))
			show_full_perf_metrics = !show_full_perf_metrics;

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_0))
			show_debug = !show_debug;

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

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_N))
			drawFlags ^= DEBUGDRAW_FLAGS_BOUNDING;
	
		Input::Instance()->SetInput(FORWARD, Window::GetKeyboard()->KeyDown(KEYBOARD_W) || Window::GetKeyboard()->KeyDown(KEYBOARD_UP));
		Input::Instance()->SetInput(BACKWARD, Window::GetKeyboard()->KeyDown(KEYBOARD_S) || Window::GetKeyboard()->KeyDown(KEYBOARD_DOWN));
		Input::Instance()->SetInput(LEFT, Window::GetKeyboard()->KeyDown(KEYBOARD_A) || Window::GetKeyboard()->KeyDown(KEYBOARD_LEFT));
		Input::Instance()->SetInput(RIGHT, Window::GetKeyboard()->KeyDown(KEYBOARD_D) || Window::GetKeyboard()->KeyDown(KEYBOARD_RIGHT));
		Input::Instance()->SetInput(JUMP, Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE));
		//Input::Instance()->SetInput(PAUSE, Window::GetKeyboard()->KeyDown(KEYBOARD_P));
		Input::Instance()->SetInput(SHOOT, Window::GetMouse()->ButtonDown(MOUSE_LEFT) && !Window::GetMouse()->ButtonHeld(MOUSE_LEFT));
		//possibly temporary
		Input::Instance()->SetInput(CAMERA_UP, Window::GetKeyboard()->KeyDown(KEYBOARD_SHIFT));
		Input::Instance()->SetInput(CAMERA_DOWN, Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE));
		PhysicsEngine::Instance()->SetDebugDrawFlags(drawFlags);
	}

	//mouse input
	if (handleMouse && GraphicsPipeline::Instance()->GetIsMainMenu()==false)
	{
		Input::Instance()->SetLookX(Window::GetMouse()->GetRelativePosition().x);
		Input::Instance()->SetLookY(Window::GetMouse()->GetRelativePosition().y);
	}

	//	GraphicsPipeline::Instance()->SetDebugDrawFlags(drawFlags);
}


//GUI interation function - Jeffery 12/03/2018
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
		Avatar* p = Game::Instance()->GetPlayer(Game::Instance()->GetUserID());
		GUIsystem::Instance()->SetHasWeapon(p->GetWeaponActive());
		GUIsystem::Instance()->SetCurrentWeapon(p->GetWeapon());
		//only needs setting once
		GUIsystem::Instance()->SetPlayerColour(p->GetColourRGBA().ToVector3());
		GUIsystem::Instance()->SetWeaponTimer(p->GetPercentageWeaponTimer());
	}

	fpsCounter++;
	if (fpsCounter > 40) {
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
	else if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_CAPITAL)) {
		GUIsystem::Instance()->SetIsCapsLocked(!GUIsystem::Instance()->GetIsCapsLocked());
		return;
	}
	else if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_LEFT)) {
		GUIsystem::Instance()->HandleTextInput(KEYBOARD_LEFT);
		return;
	}
	else if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_RIGHT)) {
		GUIsystem::Instance()->HandleTextInput(KEYBOARD_RIGHT);
		return;
	}
	else if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_COMMA)) {
		GUIsystem::Instance()->HandleTextInput(KEYBOARD_COMMA);
		return;
	}
	for (int i = KeyboardKeys::KEYBOARD_0; i <= KeyboardKeys::KEYBOARD_PERIOD; i++) {
		if (Window::GetKeyboard()->KeyTriggered(static_cast<KeyboardKeys>(i))) {
			GUIsystem::Instance()->HandleTextInput(static_cast<KeyboardKeys>(i));
			break; 
		}
	}
}

//Post process test - Jeffery 
void TestPostProcess()
{
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_F1)) {
		PostProcess::Instance()->SetPostProcessType(PostProcessType::INVERSION);
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_F2)) {
		PostProcess::Instance()->SetPostProcessType(PostProcessType::GRAYSCALE);
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_F3)) {
		PostProcess::Instance()->SetPostProcessType(PostProcessType::SHARPEN);
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_F4)) {
		PostProcess::Instance()->SetPostProcessType(PostProcessType::EDGE_DETECTION);
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_F5)) {
		PostProcess::Instance()->SetPostProcessType(PostProcessType::PERFORMANCE_BLUR);
	}

	//Test loading Screen
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_F6)) {
		GUIsystem::Instance()->SetLoadingScreen(LoadingScreenType::START);
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_F7)) {
		GUIsystem::Instance()->SetLoadingScreen(LoadingScreenType::NOT_LOADING);
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
	Window::GetWindow().LockMouseToWindow(true);
	Window::GetWindow().ShowOSPointer(false);
	//Create main game-loop
	while (Window::GetWindow().UpdateWindow() && SceneManager::Instance()->GetExitButtonClicked() == false)
	{
		float dt = Window::GetWindow().GetTimer()->GetTimedMS() * 0.001f;	//How many milliseconds since last update?
																			//Update Performance Timers (Show results every second)
		
		GraphicsPipeline::Instance()->GetCamera()->SetPosition(Vector3(0, 1, 0));

		timer_total.UpdateRealElapsedTime(dt);
		timer_physics.UpdateRealElapsedTime(dt);
		timer_update.UpdateRealElapsedTime(dt);
		timer_render.UpdateRealElapsedTime(dt);
		timer_audio.UpdateRealElapsedTime(dt);
		timer_gameplay.UpdateRealElapsedTime(dt);

		//Handle GUI mouseCursor
		HandleGUIMouseCursor();

		//Handle GUI mouseButton
		HandleGUIMouseButton();

		//Print Status Entries
		PrintStatusEntries();

		//Test Post Process
		TestPostProcess();

		//Handle Keyboard Inputs
		if (GUIsystem::Instance()->GetIsPaused() == false && 
			GUIsystem::Instance()->GetCurrentLoadingScreen() == NOT_LOADING &&
			GUIsystem::Instance()->GetResult() == NONE) 
		{
			if (GUIsystem::Instance()->GetIsTyping() == false) {
				HandleMouseAndKeyboardInputs(true, !((Map*)Game::Instance()->GetMap())->isLoading);
			}
			else {
				//Handle User Typing input
				HandleGUITextInput();
			}
		}
		

		timer_total.BeginTimingSection();

		//Update Scene
		timer_update.BeginTimingSection();
		SceneManager::Instance()->GetCurrentScene()->OnUpdateScene(dt);
		timer_update.EndTimingSection();



		timer_gameplay.BeginTimingSection();
		Game::Instance()->Update(dt);
		timer_gameplay.EndTimingSection();

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
