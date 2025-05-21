#include "UIManager.h"


void UIManager::Init() {
	WidgetShader.loadShader("UIVertex.file", "UIFragment.file");
}

void UIManager::Update() {
	for (auto& n : m_Screens) { //basically updates all screens
		n.get()->UpdateScreen();
	}
}

void UIManager::Render() {
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	WidgetShader.use();
	WidgetShader.setMat4("projection", ScreenProjection);
	for (auto& n : m_Screens) { //basically renders all screens
		n.get()->RenderScreen(WidgetShader);
	}
	glEnable(GL_CULL_FACE);
}

void UIManager::Terminate() {

}


void UIManager::AddScreen(std::unique_ptr<UIScreen> newScreen) {
	m_Screens.push_back(std::move(newScreen));
}

void UIManager::RemoveScreen(UIScreen* screen) {
	m_Screens.erase(std::remove_if(m_Screens.begin(), m_Screens.end(),[screen](const std::unique_ptr<UIScreen>& ptr) {return ptr.get() == screen;}),m_Screens.end());
}

UIScreen* UIManager::GetScreen(int index) {
	return m_Screens[index].get();
}