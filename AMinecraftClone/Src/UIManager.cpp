#include "UIManager.h"

#include <Algorithm>


void UIManager::Init() {
}

void UIManager::Update() {
	for (auto& n : m_Screens) {
		n.get()->UpdateScreen();
	}
}

void UIManager::Render() {
	for (auto& n : m_Screens) {
		n.get()->RenderScreen();
	}
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

void UIManager::ClearScreens() {
	m_Screens.clear();
}