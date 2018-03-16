//////////////////////////////////////////////////////////////////////////////////////
//
//                       .::::.
//                     .::::::::.
//                    :::::::::::
//                 ..:::::::::::'			Author: Jeffery Zhou
//              '::::::::::::'				Data: 2018-02-08
//                .:::::::::
//           '::::::::::::::..
//                ..::::::::::::.
//              ``::::::::::::::::
//               ::::``:::::::::'        .:::.
//              ::::'   ':::::'       .::::::::.
//            .::::'      ::::     .:::::::'::::.
//           .:::'       :::::  .:::::::::' ':::::.
//          .::'        :::::.:::::::::'      ':::::.
//         .::'         ::::::::::::::'         ``::::.
//     ?::             ::::::::::::'              ``::.
//    ```` ':.          ':::::::::'                  ::::..
//                       '.:::::'                    ':'````..
//
//
//Basic Class for setup CEGUI
//Give the basic communication access to mouse and keyboard for further interaction
//////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <nclgl\Vector4.h>
#include <nclgl\Vector3.h>
#include <CEGUI\CEGUI.h>
#include <CEGUI\RendererModules\OpenGL\GL3Renderer.h>
#include <nclgl\Mouse.h>
#include <nclgl\Keyboard.h>
#include <nclgl\Vector2.h>
#include <nclgl\TSingleton.h>
#include <nclgl\common.h>
#include <nclgl\Shader.h>
#include <nclgl\Mesh.h>
#include <nclgl\NCLDebug.h>
#include <cstring>
#include <ncltech\GraphicsPipeline.h>

const int Num_of_loadingTex = 2;

enum RESULT {
	WIN = 0,
	LOST = 1,
	NONE
};

enum LoadingScreenType
{
	START		= 0,
	TRANSITION	= 1,
	NOT_LOADING
};

//Box struct for user text input
struct inputBox
{
	std::string type;
	CEGUI::Editbox* editbox;
};

class GUIsystem: public TSingleton<GUIsystem>
{
public:
	friend class TSingleton<GUIsystem>;

	//constructor
	GUIsystem();
	~GUIsystem();

	//CEGUI basic functions
	void Init(const std::string& resourceDirectory);
	void Destory();
	void Reset();
	void SetMouseCursor(const std::string& imageFile);
	void ShowMouseCursor();
	void HideMouseCursor();
	void SetUpResultText();

	//GUI create helper function
	CEGUI::Window* createWidget(const std::string type, const Vector4& destRectPerc, const Vector4& destRectPix, const std::string name = "");
	CEGUI::Window* createWidgetForLoadingScreen(const std::string type, const Vector4& destRectPerc, const Vector4& destRectPix, const std::string name = "");

	//Load style for the GUI
	void LoadScheme(const std::string& schemeFile);
	void SetFont(const std::string& fontFile);
	void SetFont2(const std::string& fontFile);

	//All draw calls
	void Draw();
	void DrawLifebar();
	void DrawScorebar();
	void DrawNormalUI();
	void DrawPlayerName();
	void DrawWinLostText();

	//Getter 
	inline bool GetIsCapsLocked() { return isCapsLocked; }
	inline bool GetDrawResult() { return this->drawResult; }
	inline RESULT GetResult() { return result; }
	inline bool GetIsPaused() { return isPaused; }
	inline bool GetIsTyping() { return isTyping; }
	inline bool GetDrawScoreBar() { return drawScorebar; }
	inline bool GetDrawMiniMap() { return drawMiniMap; }
	inline LoadingScreenType GetCurrentLoadingScreen() { return currentLoadingScreen; }
	inline static CEGUI::OpenGL3Renderer* GetRenderer() { return m_renderer; }
	inline const CEGUI::GUIContext* GetContext() { return m_context; }
	inline bool GetDrawLifeBar() { return drawLifebar; }

	//Setter
	inline void SetResult(RESULT result) { this->result = result; }
	inline void SetDrawResult(bool drawResult) { this->drawResult = drawResult; }
	inline void SetIsCapsLocked(bool isCapsLocked) { this->isCapsLocked = isCapsLocked; }
	inline void SetIsPaused(bool isPaused) { this->isPaused = isPaused; }
	inline void SetIsTyping(bool a) { isTyping = a; }
	inline void SetDrawScoreBar(bool a) { drawScorebar = a; }
	inline void SetDrawMiniMap(bool a) { drawMiniMap = a; }
	inline void SetPlayerColour(Vector3 c) { playerColour = c; }
	inline void SetWeaponTimer(float t) { weaponTimer = t; }
	inline void SetLoadingBar(float f) { loadingProgress->setProgress(f); }
	inline void SetCurrentWeapon(int w) { currentWeapon = w; }
	inline void SetHasWeapon(bool b) { hasWeapon = b; }
	inline void SetLoadingScreen(LoadingScreenType currentType) { currentLoadingScreen = currentType; }
	inline void SetDrawLifeBar(bool drawLifebar) { this->drawLifebar = drawLifebar; }

	inline void RandomizeMessage() { message = rand() % 7; }
	inline void UpdateScorebar(float a1, float a2, float a3, float a4) { p1 = a1;p2 = a2;p3 = a3;p4 = a4; }
	//HandleTextInput
	void HandleTextInput(KeyboardKeys pressedKey);

	//HandleMousePosition
	void HandleMousePosition(float x, float y);

	//HandleMouseInput
	void onMouseButtonPressed(MouseButtons button);
	void onMouseButtonHold(bool isHold);

	//Functions for loading Screen
	void DrawTransitionLoadingScreen();
	void DrawStartLoadingScreen();
	void SetUpLoadingScreen();
	void UpdateFakeProgressBar();

	//Functions for life bar
	void UpdateLifebar(float life);

	//Map Problem
	void MapProblem(uint errorNumber, uint errorLine);
	
	//limit text input length in main menu
	void LimitTextLength();

	//public variables
	std::string playerNames[4] = {"", "", "", ""};
	Vector3 playersPosition[4] = {Vector3(0,0,0),Vector3(0,0,0),Vector3(0,0,0),Vector3(0,0,0)};
	bool drawPlayerName = false;
	float translation = 0.01f;
	std::vector<inputBox> editboxes;
	std::string currentType;
	std::string userName;
	std::string clientName;
	bool sendInfo = false;
protected:
	//CEGUI variables
	static CEGUI::OpenGL3Renderer* m_renderer;
	CEGUI::GUIContext* m_context = NULL;
	CEGUI::GUIContext* m_context_public = NULL;
	CEGUI::Window* m_root = NULL;
	CEGUI::Window* m_loadingRoot = NULL;
	CEGUI::RenderTarget* target;
	CEGUI::Font* titleFont;

	Vector2 realMousePos;
	float mouseSensitivity;
	bool isTyping;

	//Score bar
	Mesh* scorebar;
	Shader* scorebarShader;
	bool drawScorebar = false;
	GLuint scorebarTexture;
	GLuint scorebarDUDV;
	float playerLife;

	//MiniMap
	bool drawMiniMap = false;

	//Life bar
	Mesh* lifebar;
	Shader* lifebarShader;
	bool drawLifebar = false;

	//weapon icon
	Mesh* weaponIcon;
	Shader* weaponShader;
	GLuint weaponTextures[4];
	int currentWeapon;
	bool hasWeapon;
	Vector3 playerColour;
	void DrawWeaponIcon();
	float weaponTimer;

	float p1, p2, p3, p4;

	//Loading Screen
	Shader* loadingScreenShader = NULL;
	LoadingScreenType currentLoadingScreen = NOT_LOADING;
	Mesh* loadingScreen;
	GLuint loadingScreenTexture[Num_of_loadingTex];
	CEGUI::Titlebar* loadingMessage;
	CEGUI::ProgressBar* loadingProgress;
	float progressBarValue = 0;

	//Result 
	RESULT result;
	bool drawResult = false;
	CEGUI::Titlebar* ResultText;

	bool isCapsLocked = false;

	//PHIL'S PHILTHY CODE 14/03/2018
	string GetWinMessage();
	string GetLoseMessage();
	int message;

	//Pause controller
	bool isPaused = false;
};