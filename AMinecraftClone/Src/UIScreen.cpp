#include "UIScreen.h"

void UIScreen::RenderScreen(Shader& shader) {
	for (auto& n : m_Widgets) {
		n.get()->RenderWidget(shader);
	}
}

void UIScreen::UpdateScreen() {
	for (auto& n : m_Widgets) {
		n.get()->UpdateWidget();
	}
}

void UIScreen::AddWidget(std::unique_ptr<Widget> newWidget) {
	m_Widgets.push_back(std::move(newWidget));
}

Widget* UIScreen::GetWidget(int index) {
	return m_Widgets[index].get();
}