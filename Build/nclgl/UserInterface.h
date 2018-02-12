/***************************************************************
Author: Jeffery Zhou
Data: 2018-02-08
Basic Class for setup CEGUI
*/

#pragma once
#include <nclgl\Vector4.h>
#include <CEGUI\CEGUI.h>
#include <CEGUI\RendererModules\OpenGL\GL3Renderer.h>
#include <nclgl\Mouse.h>
#include <nclgl\Keyboard.h>
#include <nclgl\Vector2.h>

class GUI
{
public:
	void Init(const std::string& resourceDirectory);
	void Destory();

	void Draw();

	void SetMouseCursor(const std::string& imageFile);
	void ShowMouseCursor();
	void HideMouseCursor();

	void HandleInput(int _case);
	CEGUI::Key::Scan TransferKeys(int Key);

	//HandleMousePosition
	void HandleMousePosition(float x, float y);

	//HandleMouseInput
	void onMouseButtonPressed(MouseButtons button);
	void onMouseButtonReleased(MouseButtons button);

	void LoadScheme(const std::string& schemeFile);
	void SetFont(const std::string& fontFile);

	CEGUI::Window* createWidget(const std::string type, const Vector4& destRectPerc, const Vector4& destRectPix, const std::string name = "");

	//Getter
	static CEGUI::OpenGL3Renderer* GetRenderer() { return m_renderer; }
	const CEGUI::GUIContext* GetContext() { return m_context; }
protected:

private:
	static CEGUI::OpenGL3Renderer* m_renderer;
	CEGUI::GUIContext* m_context = NULL;
	CEGUI::Window* m_root = NULL;

	Vector2 realMousePos;
	float mouseSensitivity;
};