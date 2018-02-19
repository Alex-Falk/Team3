/*********************************************************************************
Author: Jeffery Zhou
Data: 2018-02-08
Basic Class for setup CEGUI
Give the basic communication access to mouse and keyboard for further interaction
***********************************************************************************/

//TODO:
//Create Keyboard Text inputting function

#pragma once
#include <nclgl\Vector4.h>
#include <CEGUI\CEGUI.h>
#include <CEGUI\RendererModules\OpenGL\GL3Renderer.h>
#include <nclgl\Mouse.h>
#include <nclgl\Keyboard.h>
#include <nclgl\Vector2.h>
#include <nclgl\TSingleton.h>
#include <nclgl\common.h>

struct inputBox
{
	std::string type;
	CEGUI::Editbox* editbox;
};

struct userInput
{
	std::string type;
	std::string content;
};

class GUIsystem: public TSingleton<GUIsystem>
{
public:
	friend class TSingleton<GUIsystem>;

	GUIsystem();
	~GUIsystem();

	std::vector<inputBox> inputBox;
	std::vector<userInput> userTyping;

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

protected:
	static CEGUI::OpenGL3Renderer* m_renderer;
	CEGUI::GUIContext* m_context = NULL;
	CEGUI::Window* m_root = NULL;

	Vector2 realMousePos;
	float mouseSensitivity;
	bool isTyping;
};