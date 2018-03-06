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

	GUIsystem();
	~GUIsystem();

	std::vector<inputBox> editboxes;
	//Tell which textBox is typing now?
	std::string currentType;

	void Init(const std::string& resourceDirectory);
	void Destory();
	void Reset();
	void Draw();

	void SetMouseCursor(const std::string& imageFile);
	void ShowMouseCursor();
	void HideMouseCursor();

	//HandleTextInput
	void HandleTextInput(KeyboardKeys pressedKey);

	//HandleMousePosition
	void HandleMousePosition(float x, float y);

	//HandleMouseInput
	void onMouseButtonPressed(MouseButtons button);
	void onMouseButtonHold(bool isHold);

	//Load style for the GUI
	void LoadScheme(const std::string& schemeFile);

	void SetFont(const std::string& fontFile);
	void SetFont2(const std::string& fontFile);

	//GUI create helper function
	CEGUI::Window* createWidget(const std::string type, const Vector4& destRectPerc, const Vector4& destRectPix, const std::string name = "");
	
	CEGUI::Window* createWidgetForLoadingScreen(const std::string type, const Vector4& destRectPerc, const Vector4& destRectPix, const std::string name = "");

	static CEGUI::OpenGL3Renderer* GetRenderer() { return m_renderer; }
	const CEGUI::GUIContext* GetContext() { return m_context; }

	bool GetIsTyping() { return isTyping; }
	void SetIsTyping(bool a) { isTyping = a; }

	void SetDrawScoreBar(bool a) { drawScorebar = a; }

	bool GetDrawScoreBar() { return drawScorebar; }

	void UpdateScorebar(float a1, float a2, float a3, float a4) {
		p1 = a1;
		p2 = a2;
		p3 = a3;
		p4 = a4;
	}

	inline void SetDrawMiniMap(bool a) { drawMiniMap = a; }
	inline void SetCurrentWeapon(int w) { currentWeapon = w; }
	inline void SetHasWeapon(bool b) { hasWeapon = b; }
	inline bool GetDrawMiniMap() { return drawMiniMap; }
	inline void SetPlayerColour(Vector3 c) { playerColour = c; }

	//Getter Setter of isLoading boolean
	inline LoadingScreenType GetCurrentLoadingScreen() { return currentLoadingScreen; }
	inline void SetLoadingScreen(LoadingScreenType currentType) { currentLoadingScreen = currentType; }

	//Functions for loading Screen
	void DrawTransitionLoadingScreen();
	void DrawStartLoadingScreen();
	void SetUpLoadingScreen();
	void UpdateFakeProgressBar();

	//Loading Screen textrue translation
	float translation = 0.01;
	
	//Temp text context holder
	std::string userName;
	std::string clientName;
	bool sendInfo = false;

	//playerNames
	std::string playerNames[4] = {"", "", "", ""};
	Vector3 playersPosition[4] = {
		Vector3(0,0,0),
		Vector3(0,0,0),
		Vector3(0,0,0),
		Vector3(0,0,0)
	};
	float time;
	bool drawPlayerName = false;

	//Result
	inline void SetResult(RESULT result) { this->result = result; }
	inline RESULT GetResult() { return result; }
	inline void SetDrawResult(bool drawResult) { this->drawResult = drawResult; }
	inline bool GetDrawResult() { return this->drawResult; }
	void SetUpResultText();
	void DrawWinLostText();
protected:
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

	Mesh* scorebar;
	Shader* scorebarShader;
	bool drawScorebar = false;

	bool drawMiniMap = false;

	//weapon icon
	Mesh* weaponIcon;
	Shader* weaponShader;
	GLuint weaponTextures[4];
	int currentWeapon;
	bool hasWeapon;
	Vector3 playerColour;
	void DrawWeaponIcon();

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
};