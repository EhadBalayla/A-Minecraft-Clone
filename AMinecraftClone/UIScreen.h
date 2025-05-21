#pragma once
#include "Widget.h"
#include <memory>
#include <vector>

class UIScreen
{
public:

	void RenderScreen(Shader& shader);
	void UpdateScreen();

	Widget* GetWidget(int index);

protected:
	std::vector<std::unique_ptr<Widget>> m_Widgets;
	void AddWidget(std::unique_ptr<Widget> newWidget);
};

