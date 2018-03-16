#include "Game.h"
#include <PC/UserInterface.h>


CEGUI::OpenGL3Renderer* GUIsystem::m_renderer = NULL;

GUIsystem::GUIsystem()
{
	Init(CEGUIDIR);

	//Scorebar
	scorebar = Mesh::GenerateQuad();
	scorebarShader = new Shader(SHADERDIR"UI/ScorebarVertex.glsl",
		SHADERDIR"UI/ScorebarFragment.glsl");
	if (!scorebarShader->LinkProgram()) {
		NCLERROR("Load scoreBar shader failed");
	}
	scorebarTexture = SOIL_load_OGL_texture(TEXTUREDIR"scorebartexture.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT);
	scorebarDUDV = SOIL_load_OGL_texture(TEXTUREDIR"waterDUDV.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT);
	glBindTexture(GL_TEXTURE_2D, scorebarTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, scorebarDUDV);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	//LifeBar
	lifebar = Mesh::GenerateQuad();
	lifebarShader = new Shader(SHADERDIR"UI/lifebarVert.glsl",
		SHADERDIR"UI/lifebarFrag.glsl");
	if (!scorebarShader->LinkProgram()) {
		NCLERROR("Load lifebar shader failed");
	}

	//phil 28/02/2018
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//weapon icon
	weaponIcon = Mesh::GenerateQuad();
	weaponShader = new Shader(SHADERDIR"Game/MiniMapVertex.glsl",
		SHADERDIR"UI/WeaponTextureFrag.glsl");
	if (!weaponShader->LinkProgram()) {
		NCLERROR("Linking weapon shader failed :(");
	}

	//load icons
	weaponTextures[0] = SOIL_load_OGL_texture(TEXTUREDIR"SprayIcon.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT);
	weaponTextures[1] = SOIL_load_OGL_texture(TEXTUREDIR"PistolIcon.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT);
	weaponTextures[2] = SOIL_load_OGL_texture(TEXTUREDIR"AutoPaintIcon.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT);
	weaponTextures[3] = SOIL_load_OGL_texture(TEXTUREDIR"RocketIcon.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT);
	for (int i = 0; i < 4; i++) {
		glBindTexture(GL_TEXTURE_2D, weaponTextures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	currentWeapon = 0;
	hasWeapon = false;

	message = rand() % 5;
}

GUIsystem::~GUIsystem()
{
	Destory();

}

void GUIsystem::UpdateLifebar(float life)
{
	playerLife = life;
}

void GUIsystem::Init(const std::string& resourceDirectory)
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

	//Load Scheme - Which actually means UI style - notice that multiple Scheme could be load at once
	LoadScheme("WindowsLook.scheme");
	LoadScheme("TaharezLook.scheme");
	LoadScheme("AlfiskoSkin.scheme");
	LoadScheme("OgreTray.scheme");

	//Set Font sytle
	SetFont("DejaVuSans-10");

	//SetMouseCursor
	SetMouseCursor("AlfiskoSkin/MouseArrow");
	ShowMouseCursor();

}

void GUIsystem::Destory()
{
	CEGUI::System::getSingleton().destroyGUIContext(*m_context);
}

void GUIsystem::Reset()
{
	Destory();
	Init(CEGUIDIR);
}

void GUIsystem::Draw()
{
	DrawNormalUI();
	DrawWeaponIcon();
	DrawLifebar();
	DrawScorebar();
	DrawPlayerName();
	DrawWinLostText();
}

void GUIsystem::DrawNormalUI()
{
	//Render normal UI
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

void GUIsystem::DrawPlayerName()
{
	if (drawPlayerName) {
		//Draw player names
		for (int i = 0; i < 4; i++) {
			if (Game::Instance()->GetPlayer(i)) {
				//draw
				playersPosition[i].y += 0.7f;
				NCLDebug::DrawTextWs(playersPosition[i], 25.0f, TEXTALIGN_CENTRE, Vector4(0, 0, 0, 1), playerNames[i]);
			}
		}
	}
}

void GUIsystem::DrawScorebar()
{
	//Render score bar
	if (drawScorebar == true) {
		float superSamples = (float)(GraphicsPipeline::Instance()->GetNumSuperSamples());
		glUseProgram(scorebarShader->GetProgram());
		Matrix4 modelMatrix = Matrix4::Translation(Vector3(0.0f, 0.88f, 0.0f)) * Matrix4::Scale(Vector3(0.40f, 0.03f, 0.0f));
		Matrix4 textureMatrix;
		glUniformMatrix4fv(glGetUniformLocation(scorebarShader->GetProgram(), "textureMatrix"), 1, GL_FALSE, (float*)&textureMatrix);
		glUniformMatrix4fv(glGetUniformLocation(scorebarShader->GetProgram(), "uModelMtx"), 1, false, *&modelMatrix.values);
		glUniform1f(glGetUniformLocation(scorebarShader->GetProgram(), "uGammaCorrection"),
			GraphicsPipeline::Instance()->GetGammaCorrection());
		glUniform1f(glGetUniformLocation(scorebarShader->GetProgram(), "moveFactor"), GraphicsPipeline::Instance()->GetTotalTime()*0.1f);
		glUniform1f(glGetUniformLocation(scorebarShader->GetProgram(), "uNumSuperSamples"), superSamples);
		glUniform1f(glGetUniformLocation(scorebarShader->GetProgram(), "isLifebar"), 0.0);
		glUniform1f(glGetUniformLocation(scorebarShader->GetProgram(), "health"), playerLife);
		glUniform2f(glGetUniformLocation(scorebarShader->GetProgram(), "uSinglepixel"), 1.f / GraphicsPipeline::Instance()->GetScreenTexWidth(), 1.f / GraphicsPipeline::Instance()->GetScreenTexHeight());
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(scorebarShader->GetProgram(), "DiffuseTex"), 0);
		glBindTexture(GL_TEXTURE_2D, scorebarTexture);
		glActiveTexture(GL_TEXTURE1);
		glUniform1i(glGetUniformLocation(scorebarShader->GetProgram(), "dudvTex"), 1);
		glBindTexture(GL_TEXTURE_2D, scorebarDUDV);
		glUniform1f(glGetUniformLocation(scorebarShader->GetProgram(), "player1"), p1);
		glUniform1f(glGetUniformLocation(scorebarShader->GetProgram(), "player2"), p2);
		glUniform1f(glGetUniformLocation(scorebarShader->GetProgram(), "player3"), p3);
		glUniform1f(glGetUniformLocation(scorebarShader->GetProgram(), "player4"), p4);
		//glBindTexture(GL_TEXTURE_2D, 0);
		//glActiveTexture(0);
		scorebar->Draw();
		glUseProgram(0);
	}
}

void GUIsystem::DrawLifebar()
{
	if (drawLifebar == true) {
		float superSamples = (float)(GraphicsPipeline::Instance()->GetNumSuperSamples());
		glUseProgram(scorebarShader->GetProgram());
		Matrix4 modelMatrix = Matrix4::Translation(Vector3(0.0f, -0.88f, 0.0f)) * Matrix4::Scale(Vector3(0.40f, 0.03f, 0.0f));
		Matrix4 textureMatrix;
		glUniformMatrix4fv(glGetUniformLocation(scorebarShader->GetProgram(), "textureMatrix"), 1, GL_FALSE, (float*)&textureMatrix);
		glUniformMatrix4fv(glGetUniformLocation(scorebarShader->GetProgram(), "uModelMtx"), 1, false, *&modelMatrix.values);
		glUniform1f(glGetUniformLocation(scorebarShader->GetProgram(), "uGammaCorrection"),
			GraphicsPipeline::Instance()->GetGammaCorrection());
		glUniform1f(glGetUniformLocation(scorebarShader->GetProgram(), "moveFactor"), GraphicsPipeline::Instance()->GetTotalTime()*0.1f);
		glUniform1f(glGetUniformLocation(scorebarShader->GetProgram(), "uNumSuperSamples"), superSamples);
		glUniform1f(glGetUniformLocation(scorebarShader->GetProgram(), "isLifebar"), 1.0);
		glUniform1f(glGetUniformLocation(scorebarShader->GetProgram(), "health"), playerLife);
		glUniform2f(glGetUniformLocation(scorebarShader->GetProgram(), "uSinglepixel"), 1.f / GraphicsPipeline::Instance()->GetScreenTexWidth(), 1.f / GraphicsPipeline::Instance()->GetScreenTexHeight());
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(scorebarShader->GetProgram(), "DiffuseTex"), 0);
		glBindTexture(GL_TEXTURE_2D, scorebarTexture);
		glActiveTexture(GL_TEXTURE1);
		glUniform1i(glGetUniformLocation(scorebarShader->GetProgram(), "dudvTex"), 1);
		glBindTexture(GL_TEXTURE_2D, scorebarDUDV);
		glUniform1f(glGetUniformLocation(scorebarShader->GetProgram(), "player1"), p1);
		glUniform1f(glGetUniformLocation(scorebarShader->GetProgram(), "player2"), p2);
		glUniform1f(glGetUniformLocation(scorebarShader->GetProgram(), "player3"), p3);
		glUniform1f(glGetUniformLocation(scorebarShader->GetProgram(), "player4"), p4);
		//glBindTexture(GL_TEXTURE_2D, 0);
		//glActiveTexture(0);
		scorebar->Draw();
		glUseProgram(0);
	}
}

void GUIsystem::SetMouseCursor(const std::string & imageFile)
{
	m_context->getMouseCursor().setDefaultImage(imageFile);
}

void GUIsystem::ShowMouseCursor()
{
	m_context->getMouseCursor().show();
}

void GUIsystem::HideMouseCursor()
{
	m_context->getMouseCursor().hide();
}

//Add arrow keys

void GUIsystem::HandleTextInput(KeyboardKeys pressedKey)
{
	CEGUI::utf32 codePoint = 46;
	if (isCapsLocked == false) {
		switch (pressedKey)
		{
		case KEYBOARD_A:
			m_context->injectChar(0x61);
			break;
		case KEYBOARD_B:
			m_context->injectChar(0x62);
			break;
		case KEYBOARD_C:
			m_context->injectChar(0x63);
			break;
		case KEYBOARD_D:
			m_context->injectChar(0x64);
			break;
		case KEYBOARD_E:
			m_context->injectChar(0x65);
			break;
		case KEYBOARD_F:
			m_context->injectChar(0x66);
			break;
		case KEYBOARD_G:
			m_context->injectChar(0x67);
			break;
		case KEYBOARD_H:
			m_context->injectChar(0x68);
			break;
		case KEYBOARD_I:
			m_context->injectChar(0x69);
			break;
		case KEYBOARD_J:
			m_context->injectChar(0x6a);
			break;
		case KEYBOARD_K:
			m_context->injectChar(0x6b);
			break;
		case KEYBOARD_L:
			m_context->injectChar(0x6c);
			break;
		case KEYBOARD_M:
			m_context->injectChar(0x6d);
			break;
		case KEYBOARD_N:
			m_context->injectChar(0x6e);
			break;
		case KEYBOARD_O:
			m_context->injectChar(0x6f);
			break;
		case KEYBOARD_P:
			m_context->injectChar(0x70);
			break;
		case KEYBOARD_Q:
			m_context->injectChar(0x71);
			break;
		case KEYBOARD_R:
			m_context->injectChar(0x72);
			break;
		case KEYBOARD_S:
			m_context->injectChar(0x73);
			break;
		case KEYBOARD_T:
			m_context->injectChar(0x74);
			break;
		case KEYBOARD_U:
			m_context->injectChar(0x75);
			break;
		case KEYBOARD_V:
			m_context->injectChar(0x76);
			break;
		case KEYBOARD_W:
			m_context->injectChar(0x77);
			break;
		case KEYBOARD_X:
			m_context->injectChar(0x78);
			break;
		case KEYBOARD_Y:
			m_context->injectChar(0x79);
			break;
		case KEYBOARD_Z:
			m_context->injectChar(0x7a);
			break;
		case KEYBOARD_PERIOD:
			m_context->injectChar(0x2e);
			break;
		case KEYBOARD_0:
			m_context->injectChar(0x30);
			break;
		case KEYBOARD_1:
			m_context->injectChar(0x31);
			break;
		case KEYBOARD_2:
			m_context->injectChar(0x32);
			break;
		case KEYBOARD_3:
			m_context->injectChar(0x33);
			break;
		case KEYBOARD_4:
			m_context->injectChar(0x34);
			break;
		case KEYBOARD_5:
			m_context->injectChar(0x35);
			break;
		case KEYBOARD_6:
			m_context->injectChar(0x36);
			break;
		case KEYBOARD_7:
			m_context->injectChar(0x37);
			break;
		case KEYBOARD_8:
			m_context->injectChar(0x38);
			break;
		case KEYBOARD_9:
			m_context->injectChar(0x39);
			break;
		case KEYBOARD_LEFT:
			m_context->injectKeyDown(CEGUI::Key::ArrowLeft);
			m_context->injectKeyUp(CEGUI::Key::ArrowLeft);
			break;
		case KEYBOARD_RIGHT:
			m_context->injectKeyDown(CEGUI::Key::ArrowRight);
			m_context->injectKeyUp(CEGUI::Key::ArrowRight);
			break;
		case KEYBOARD_BACK:
			m_context->injectKeyDown(CEGUI::Key::Backspace);
			m_context->injectKeyUp(CEGUI::Key::Backspace);
			break;
		case KEYBOARD_SEPARATOR:
			m_context->injectChar(0x3a);
			break;
		case KEYBOARD_COMMA:
			m_context->injectChar(0x3a);
			break;
		case KEYBOARD_SPACE:
			m_context->injectChar(0x20);
			break;
		case KEYBOARD_RETURN:
			isTyping = false;
			sendInfo = true;
			break;
		default:
			break;
		}
	}
	else {
		switch (pressedKey)
		{
		case KEYBOARD_A:
			m_context->injectChar(0x41);
			break;
		case KEYBOARD_B:
			m_context->injectChar(0x42);
			break;
		case KEYBOARD_C:
			m_context->injectChar(0x43);
			break;
		case KEYBOARD_D:
			m_context->injectChar(0x44);
			break;
		case KEYBOARD_E:
			m_context->injectChar(0x45);
			break;
		case KEYBOARD_F:
			m_context->injectChar(0x46);
			break;
		case KEYBOARD_G:
			m_context->injectChar(0x47);
			break;
		case KEYBOARD_H:
			m_context->injectChar(0x48);
			break;
		case KEYBOARD_I:
			m_context->injectChar(0x49);
			break;
		case KEYBOARD_J:
			m_context->injectChar(0x4a);
			break;
		case KEYBOARD_K:
			m_context->injectChar(0x4b);
			break;
		case KEYBOARD_L:
			m_context->injectChar(0x4c);
			break;
		case KEYBOARD_M:
			m_context->injectChar(0x4d);
			break;
		case KEYBOARD_N:
			m_context->injectChar(0x4e);
			break;
		case KEYBOARD_O:
			m_context->injectChar(0x4f);
			break;
		case KEYBOARD_P:
			m_context->injectChar(0x50);
			break;
		case KEYBOARD_Q:
			m_context->injectChar(0x51);
			break;
		case KEYBOARD_R:
			m_context->injectChar(0x52);
			break;
		case KEYBOARD_S:
			m_context->injectChar(0x53);
			break;
		case KEYBOARD_T:
			m_context->injectChar(0x54);
			break;
		case KEYBOARD_U:
			m_context->injectChar(0x55);
			break;
		case KEYBOARD_V:
			m_context->injectChar(0x56);
			break;
		case KEYBOARD_W:
			m_context->injectChar(0x57);
			break;
		case KEYBOARD_X:
			m_context->injectChar(0x58);
			break;
		case KEYBOARD_Y:
			m_context->injectChar(0x59);
			break;
		case KEYBOARD_Z:
			m_context->injectChar(0x5a);
			break;
		case KEYBOARD_PERIOD:
			m_context->injectChar(0x2e);
			break;
		case KEYBOARD_0:
			m_context->injectChar(0x30);
			break;
		case KEYBOARD_1:
			m_context->injectChar(0x31);
			break;
		case KEYBOARD_2:
			m_context->injectChar(0x32);
			break;
		case KEYBOARD_3:
			m_context->injectChar(0x33);
			break;
		case KEYBOARD_4:
			m_context->injectChar(0x34);
			break;
		case KEYBOARD_5:
			m_context->injectChar(0x35);
			break;
		case KEYBOARD_6:
			m_context->injectChar(0x36);
			break;
		case KEYBOARD_7:
			m_context->injectChar(0x37);
			break;
		case KEYBOARD_8:
			m_context->injectChar(0x38);
			break;
		case KEYBOARD_9:
			m_context->injectChar(0x39);
			break;
		case KEYBOARD_LEFT:
			m_context->injectKeyDown(CEGUI::Key::ArrowLeft);
			m_context->injectKeyUp(CEGUI::Key::ArrowLeft);
			break;
		case KEYBOARD_RIGHT:
			m_context->injectKeyDown(CEGUI::Key::ArrowRight);
			m_context->injectKeyUp(CEGUI::Key::ArrowRight);
			break;
		case KEYBOARD_COMMA:
			m_context->injectChar(0x3a);
			break;
		case KEYBOARD_BACK:
			m_context->injectKeyDown(CEGUI::Key::Backspace);
			m_context->injectKeyUp(CEGUI::Key::Backspace);
			break;
		case KEYBOARD_SPACE:
			m_context->injectChar(0x20);
			break;
		case KEYBOARD_RETURN:
			isTyping = false;
			sendInfo = true;
			break;
		default:
			break;
		}
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

void GUIsystem::HandleMousePosition(float x, float y)
{
	m_context->injectMousePosition(x, y);
}

void GUIsystem::onMouseButtonPressed(MouseButtons button)
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

void GUIsystem::onMouseButtonHold(bool isHold)
{
	if (isHold == true) {
		m_context->injectMouseButtonDown(CEGUI::MouseButton::LeftButton);
	}
	else {
		m_context->injectMouseButtonUp(CEGUI::MouseButton::LeftButton);
	}
}

void GUIsystem::LoadScheme(const std::string & schemeFile)
{
	CEGUI::SchemeManager::getSingleton().createFromFile(schemeFile);
}

void GUIsystem::SetFont(const std::string & fontFile)
{
	CEGUI::FontManager::getSingleton().createFromFile(fontFile + ".font");
	m_context->setDefaultFont(fontFile);
}

void GUIsystem::SetFont2(const std::string& fontFile)
{
	CEGUI::FontManager::getSingleton().createFromFile(fontFile + ".font");
	m_context_public->setDefaultFont(fontFile);
}

CEGUI::Window * GUIsystem::createWidget(const std::string type, const Vector4& destRectPerc, const Vector4& destRectPix, const std::string name)
{
	CEGUI::Window* newWindow = CEGUI::WindowManager::getSingleton().createWindow(type, name);
	m_root->addChild(newWindow);
	newWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(destRectPerc.x, destRectPix.x), CEGUI::UDim(destRectPerc.y, destRectPix.y)));
	newWindow->setSize(CEGUI::USize(CEGUI::UDim(destRectPerc.z, destRectPix.z), CEGUI::UDim(destRectPerc.w, destRectPix.w)));
	return newWindow;
}

CEGUI::Window * GUIsystem::createWidgetForLoadingScreen(const std::string type, const Vector4 & destRectPerc, const Vector4 & destRectPix, const std::string name)
{
	CEGUI::Window* newWindow = CEGUI::WindowManager::getSingleton().createWindow(type, name);
	m_loadingRoot->addChild(newWindow);
	newWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(destRectPerc.x, destRectPix.x), CEGUI::UDim(destRectPerc.y, destRectPix.y)));
	newWindow->setSize(CEGUI::USize(CEGUI::UDim(destRectPerc.z, destRectPix.z), CEGUI::UDim(destRectPerc.w, destRectPix.w)));
	return newWindow;
}

void GUIsystem::DrawTransitionLoadingScreen()
{
	loadingMessage->enable();
	loadingMessage->setVisible(true);

	loadingProgress->disable();
	loadingProgress->setVisible(true);
	glUseProgram(loadingScreenShader->GetProgram());
	float superSamples = (float)(GraphicsPipeline::Instance()->GetNumSuperSamples());
	//Matrix4 modelMatrix = Matrix4::Translation(Vector3(0, 0, 0)) * Matrix4::Scale(Vector3(1, 1, 1));
	Matrix4 modelMatrix = Matrix4::Translation(Vector3(0, 0, 0)) * Matrix4::Scale(Vector3(1, 1, 0));
	Matrix4 textureMatrix;
	//textureMatrix.ToIdentity();
	//Matrix4 textureMatrix = Matrix4::Scale(Vector3(0.8,0.8,0))* Matrix4::Translation(Vector3(translation, 0, 0));
	//Matrix4 textureMatrix = Matrix4::Scale(Vector3(0.9, 0.9, 0)) * Matrix4::Translation(Vector3(texRotation,0,0));
	glUniformMatrix4fv(glGetUniformLocation(loadingScreenShader->GetProgram(), "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);
	glUniformMatrix4fv(glGetUniformLocation(loadingScreenShader->GetProgram(), "textureMatrix"), 1, GL_FALSE, (float*)&textureMatrix);
	glUniform1f(glGetUniformLocation(loadingScreenShader->GetProgram(), "uGammaCorrection"),
		GraphicsPipeline::Instance()->GetGammaCorrection());
	glUniform1f(glGetUniformLocation(loadingScreenShader->GetProgram(), "uNumSuperSamples"), superSamples);
	glUniform2f(glGetUniformLocation(loadingScreenShader->GetProgram(), "uSinglepixel"), 1.f / GraphicsPipeline::Instance()->GetScreenTexWidth(), 1.f / GraphicsPipeline::Instance()->GetScreenTexHeight());
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(loadingScreenShader->GetProgram(), "DiffuseTex"), 0);
	glBindTexture(GL_TEXTURE_2D, loadingScreenTexture[START]);
	loadingScreen->Draw();

	glUseProgram(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_renderer->beginRendering();
	m_context_public->draw();
	m_renderer->endRendering();
	glDisable(GL_SCISSOR_TEST);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(0);
	//UpdateFakeProgressBar();
}

void GUIsystem::DrawStartLoadingScreen()
{
	glUseProgram(loadingScreenShader->GetProgram());
	float superSamples = (float)(GraphicsPipeline::Instance()->GetNumSuperSamples());
	//Matrix4 modelMatrix = Matrix4::Translation(Vector3(0, 0, 0)) * Matrix4::Scale(Vector3(1, 1, 1));
	Matrix4 modelMatrix = Matrix4::Translation(Vector3(0, 0, 0)) * Matrix4::Scale(Vector3(1, 1, 0));
	Matrix4 textureMatrix;
	//textureMatrix.ToIdentity();
	//Matrix4 textureMatrix = Matrix4::Scale(Vector3(0.8,0.8,0))* Matrix4::Translation(Vector3(translation, 0, 0));
	//Matrix4 textureMatrix = Matrix4::Scale(Vector3(0.9, 0.9, 0)) * Matrix4::Translation(Vector3(texRotation,0,0));
	glUniformMatrix4fv(glGetUniformLocation(loadingScreenShader->GetProgram(), "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);
	glUniformMatrix4fv(glGetUniformLocation(loadingScreenShader->GetProgram(), "textureMatrix"), 1, GL_FALSE, (float*)&textureMatrix);
	glUniform1f(glGetUniformLocation(loadingScreenShader->GetProgram(), "uGammaCorrection"),
		GraphicsPipeline::Instance()->GetGammaCorrection());
	glUniform1f(glGetUniformLocation(loadingScreenShader->GetProgram(), "uNumSuperSamples"), superSamples);
	glUniform2f(glGetUniformLocation(loadingScreenShader->GetProgram(), "uSinglepixel"), 1.f / GraphicsPipeline::Instance()->GetScreenTexWidth(), 1.f / GraphicsPipeline::Instance()->GetScreenTexHeight());
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(loadingScreenShader->GetProgram(), "DiffuseTex"), 0);
	glBindTexture(GL_TEXTURE_2D, loadingScreenTexture[START]);
	loadingScreen->Draw();
	
	glUseProgram(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_renderer->beginRendering();
	m_context_public->draw();
	m_renderer->endRendering();
	glDisable(GL_SCISSOR_TEST);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(0);
	UpdateFakeProgressBar();
}

void GUIsystem::UpdateFakeProgressBar()
{
	progressBarValue += 0.04f;
	if (progressBarValue > 1) {
		progressBarValue = 0;
	}
	loadingProgress->setProgress(progressBarValue);
}

void GUIsystem::SetUpLoadingScreen()
{
	m_context_public = &CEGUI::System::getSingleton().createGUIContext(m_renderer->getDefaultRenderTarget());
	m_loadingRoot = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "loadingRoot");
	m_context_public->setRootWindow(m_loadingRoot);

	loadingScreen = Mesh::GenerateQuad();
	SetFont2("Junicode-13");

	//Loading Texture for background
	loadingScreenTexture[START] = SOIL_load_OGL_texture(TEXTUREDIR"LoadingScreen/loadingscreen.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT);
	loadingScreenTexture[TRANSITION] = SOIL_load_OGL_texture(TEXTUREDIR"LoadingScreen/Paint2.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT);
	//LoadingTexture for animated icon

	for (int i = 0; i < Num_of_loadingTex; i++) {
		glBindTexture(GL_TEXTURE_2D, weaponTextures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//Loading Shader for loading screen
	if (loadingScreenShader == NULL) {
		loadingScreenShader = new Shader(SHADERDIR"UI/LoadingScreenVertex.glsl",
			SHADERDIR"UI/LoadingScreenFrag.glsl");
		if (!loadingScreenShader->LinkProgram()) {
			NCLERROR("Linking loading screen shader failed :(");
		}
	}

	loadingMessage = static_cast<CEGUI::Titlebar*>(
		GUIsystem::Instance()->createWidgetForLoadingScreen("OgreTray/Label",
			Vector4(0.20f, 0.2f, 0.60f, 0.60f),
			Vector4(),
			"loadingMessage"
		));
	loadingMessage->setText("Loading.....");
	//loadingMessage->setFont(titleFont);

	loadingProgress = static_cast<CEGUI::ProgressBar*>(
		GUIsystem::Instance()->createWidgetForLoadingScreen("OgreTray/ProgressBar",
			Vector4(0.30f, 0.7f, 0.40f, 0.05f),
			Vector4(),
			"loadingProgress"
		));

	loadingMessage->disable();
	loadingMessage->setVisible(false);

	loadingProgress->disable();
	loadingProgress->setVisible(false);
	SetUpResultText();
}

void GUIsystem::DrawWeaponIcon(){
	if (hasWeapon) {
		glUseProgram(weaponShader->GetProgram());
		Matrix4 modelMatrix = Matrix4::Translation(Vector3(0.9f, -0.9f, 0.0f)) * Matrix4::Scale(Vector3(0.1f, 0.1f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(weaponShader->GetProgram(), "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);
		glUniform1fv(glGetUniformLocation(weaponShader->GetProgram(), "timer"), 1, (float*)&weaponTimer);
		glUniform3fv(glGetUniformLocation(weaponShader->GetProgram(), "playerColour"), 1, (float*)&playerColour);
		//displays the weapom the user has equipped
		glBindTexture(GL_TEXTURE_2D, weaponTextures[currentWeapon]);
		weaponIcon->Draw();
		glUseProgram(0);
	}
}

void GUIsystem::DrawWinLostText()
{
	loadingMessage->disable();
	loadingMessage->setVisible(false);

	loadingProgress->disable();
	loadingProgress->setVisible(false);
	if (drawResult == true) {
		switch (result) {
		case WIN:
			//off brand bird species
			ResultText->setText(GetWinMessage());
			break;
		case LOST:
			ResultText->setText(GetLoseMessage());
			break;
		case NONE:
			break;
		default:
			break;
		}
		ResultText->enable();
		ResultText->setVisible(true);
	}
	else {
		ResultText->disable();
		ResultText->setVisible(false);
	}

	glUseProgram(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_renderer->beginRendering();
	m_context_public->draw();
	m_renderer->endRendering();
	glDisable(GL_SCISSOR_TEST);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(0);
}

void GUIsystem::SetUpResultText()
{
	ResultText = static_cast<CEGUI::Titlebar*>(
		GUIsystem::Instance()->createWidgetForLoadingScreen("OgreTray/Label",
			Vector4(0.20f, 0.00f, 0.60f, 0.60f),
			Vector4(),
			"ResultText"
		));
	ResultText->disable();
	ResultText->setVisible(false);
}

void GUIsystem::LimitTextLength()
{
	if (isTyping == true) {
		for (int i = 0; i < editboxes.size(); ++i) {
			string temp = editboxes[i].editbox->getText().c_str();
			if (temp.length() > 20) {
				temp.assign(temp, 0, 20);
				editboxes[i].editbox->setText(temp);
			}
		}
	}
}

void GUIsystem::MapProblem(uint errorNumber, uint errorLine) {
	if (errorNumber == 0) {
		cout << "File was corrupted";
	}
	else if (errorNumber == 1) {
		cout << "File was not writen in apropriate format in line " << to_string(errorLine) << " \n";
	}
	else if (errorNumber == 2) {
		cout << "The texture couldn't be found on line: " << to_string(errorLine) << " \n";

	}
}

string GUIsystem::GetWinMessage() {
	switch (message) {
	case 0:
		return "GET YOUR COAT\nYOU'VE WON!!!";
	case 1:
		return "A WINNER IS YOU!";
	case 2:
		return "WINNER WINNER\nPOULTRY DINNER";
	case 3:
		return "YOUR WINNER";
	case 4:
		return "YOU JUST\nPULLED A\nCHARLIE SHEEN";
	case 5:
		return "YOU ARE\nVICTORIOUS!";
	case 6:
		return "ANNIHILATION!";
	default:
		return "YOU ARE VICTORIOUS!!";
	}
}

string GUIsystem::GetLoseMessage() {
	switch (message) {
	case 0:
		return "YOU DIED...";
	case 1:
		return "YOUR PRINCESS\nIS IN\nANOTHER\nCASTLE";
	case 2:
		return "TRY HARD";
	case 3:
		return "NOTHIN\nPERSONNEL\nKID";
	case 4:
		return "CODE 404:\nWINNER NOT\nFOUND";
	case 5:
		return "THOU HAST\nBEEN SHAMED!";
	case 6:
		return "IT'S ALL\nOGRE NOW...";
	default:
		return "YOU ARE LOSE!!";
	}
}