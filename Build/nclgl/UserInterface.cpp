#include <nclgl/UserInterface.h>

CEGUI::OpenGL3Renderer* GUI::m_renderer = NULL;

void GUI::Init(const std::string& resourceDirectory)
{
	if (m_renderer == NULL) {
		//Initialize CEGUI library
		m_renderer = &CEGUI::OpenGL3Renderer::bootstrapSystem();

		//Set up CEGUI resource Directories
		CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());
		rp->setResourceGroupDirectory("imagesets", resourceDirectory + "/imagesets/");
		rp->setResourceGroupDirectory("schemes", resourceDirectory + "/schemes/");
		rp->setResourceGroupDirectory("fonts", resourceDirectory + "/fonts/");
		rp->setResourceGroupDirectory("layouts", resourceDirectory + "/layouts/");
		rp->setResourceGroupDirectory("looknfeels", resourceDirectory + "/looknfeel/");
		rp->setResourceGroupDirectory("lua_scripts", resourceDirectory + "/lua_scripts/");

		CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
		CEGUI::Scheme::setDefaultResourceGroup("schemes");
		CEGUI::Font::setDefaultResourceGroup("fonts");
		CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
		CEGUI::WindowManager::setDefaultResourceGroup("layouts");
		CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
	}

	//Generate CEGUI context
	m_context = &CEGUI::System::getSingleton().createGUIContext(m_renderer->getDefaultRenderTarget());

	//Create Root window
	m_root = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "root");
	m_context->setRootWindow(m_root);
}

void GUI::Destory()
{
	CEGUI::System::getSingleton().destroyGUIContext(*m_context);
}

void GUI::Draw()
{
	glUseProgram(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_renderer->beginRendering();
	m_context->draw();
	m_renderer->endRendering();
	glDisable(GL_SCISSOR_TEST);
	glEnable(GL_DEPTH_TEST);
}

void GUI::SetMouseCursor(const std::string & imageFile)
{
	m_context->getMouseCursor().setDefaultImage(imageFile);
}

void GUI::ShowMouseCursor()
{
	m_context->getMouseCursor().show();
}

void GUI::HideMouseCursor()
{
	m_context->getMouseCursor().hide();
}


void GUI::HandleTextInput(KeyboardKeys pressedKey)
{
	CEGUI::utf32 codePoint = 61;
	switch (pressedKey)
	{
	case KEYBOARD_A:
		m_context->injectChar(61);
		break;
	case KEYBOARD_B:
		m_context->injectChar(62);
		break;
	case KEYBOARD_C:
		m_context->injectChar(63);
		break;
	case KEYBOARD_D:
		m_context->injectChar(64);
		break;
	case KEYBOARD_E:
		m_context->injectChar(65);
		break;
	case KEYBOARD_F:
		m_context->injectChar(66);
		break;
	case KEYBOARD_G:
		m_context->injectChar(67);
		break;
	case KEYBOARD_H:
		m_context->injectChar(68);
		break;
	case KEYBOARD_I:
		m_context->injectChar(69);
		break;
	case KEYBOARD_J:
		m_context->injectChar(70);
		break;
	case KEYBOARD_K:
		m_context->injectChar(71);
		break;
	case KEYBOARD_L:
		m_context->injectChar(72);
		break;
	case KEYBOARD_M:
		m_context->injectChar(73);
		break;
	case KEYBOARD_N:
		m_context->injectChar(74);
		break;
	case KEYBOARD_O:
		m_context->injectChar(75);
		break;
	case KEYBOARD_P:
		m_context->injectChar(76);
		break;
	case KEYBOARD_Q:
		m_context->injectChar(77);
		break;
	case KEYBOARD_R:
		m_context->injectChar(78);
		break;
	case KEYBOARD_S:
		m_context->injectChar(79);
		break;
	case KEYBOARD_T:
		m_context->injectChar(80);
		break;
	case KEYBOARD_U:
		m_context->injectChar(81);
		break;
	case KEYBOARD_V:
		m_context->injectChar(82);
		break;
	case KEYBOARD_W:
		m_context->injectChar(83);
		break;
	case KEYBOARD_X:
		m_context->injectChar(84);
		break;
	case KEYBOARD_Y:
		m_context->injectChar(85);
		break;
	case KEYBOARD_Z:
		m_context->injectChar(86);
		break;
	default:
		break;
	}
}

CEGUI::MouseButton TransferMouseButton(MouseButtons button)
{
	switch (button)
	{
	case MOUSE_LEFT:
		return CEGUI::MouseButton::LeftButton;
		break;
	case MOUSE_RIGHT:
		return CEGUI::MouseButton::RightButton;
		break;
	default:
		return CEGUI::MouseButton::NoButton;
		break;
	}

	return CEGUI::MouseButton();
}

void GUI::HandleMousePosition(float x, float y)
{
	m_context->injectMousePosition(x, y);
}

void GUI::onMouseButtonPressed(MouseButtons button)
{
	CEGUI::MouseButton transferedButton;
	switch (button)
	{
	case MOUSE_LEFT:
		transferedButton = TransferMouseButton(button);
		m_context->injectMouseButtonClick(transferedButton);
		break;
	case MOUSE_RIGHT:
		transferedButton = TransferMouseButton(button);
		m_context->injectMouseButtonClick(transferedButton);
		break;
	default:
		break;
	}
}

void GUI::onMouseButtonHold(bool isHold)
{
	if (isHold == true) {
		m_context->injectMouseButtonDown(CEGUI::MouseButton::LeftButton);
	}
	else {
		m_context->injectMouseButtonUp(CEGUI::MouseButton::LeftButton);
	}
}

void GUI::LoadScheme(const std::string & schemeFile)
{
	CEGUI::SchemeManager::getSingleton().createFromFile(schemeFile);
}

void GUI::SetFont(const std::string & fontFile)
{
	CEGUI::FontManager::getSingleton().createFromFile(fontFile + ".font");
	m_context->setDefaultFont(fontFile);
}

CEGUI::Window * GUI::createWidget(const std::string type, const Vector4& destRectPerc, const Vector4& destRectPix, const std::string name)
{
	CEGUI::Window* newWindow = CEGUI::WindowManager::getSingleton().createWindow(type, name);
	m_root->addChild(newWindow);
	newWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(destRectPerc.x, destRectPix.x), CEGUI::UDim(destRectPerc.y, destRectPix.y)));
	newWindow->setSize(CEGUI::USize(CEGUI::UDim(destRectPerc.z, destRectPix.z), CEGUI::UDim(destRectPerc.w, destRectPix.w)));
	return newWindow;
}
