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

	//GUI create helper function
	CEGUI::Window* createWidget(const std::string type, const Vector4& destRectPerc, const Vector4& destRectPix, const std::string name = "");

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

	inline void SetCurrentWeapon(int w) { currentWeapon = w; }
	inline void SetHasWeapon(bool b) { hasWeapon = b; }
	inline void SetPlayerColour(Vector3 c) { playerColour = c; }

	string player1name;
	string player2name;
	string player3name;
	string player4name;
protected:
	static CEGUI::OpenGL3Renderer* m_renderer;
	CEGUI::GUIContext* m_context = NULL;
	CEGUI::Window* m_root = NULL;

	Vector2 realMousePos;
	float mouseSensitivity;
	bool isTyping;

	Mesh* scorebar;
	Shader* scorebarShader;
	bool drawScorebar = false;

	//weapon icon
	Mesh* weaponIcon;
	Shader* weaponShader;
	GLuint weaponTextures[4];
	int currentWeapon;
	bool hasWeapon;
	Vector3 playerColour;
	void DrawWeaponIcon();

	float p1, p2, p3, p4;
};