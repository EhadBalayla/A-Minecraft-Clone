#pragma once
#include "UIScreen.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include <vector>

class UIManager
{
public:

	glm::mat4 ScreenProjection = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, -1.0f, 1.0f);

	void Init(); //initializes the widget renderer
	void Update(); //updates all screens
	void Render(); //renders all screens
	void Terminate(); //terminates the widget renderer

	void AddScreen(std::unique_ptr<UIScreen> newScreen);
	void RemoveScreen(UIScreen* screen);
	UIScreen* GetScreen(int index);
	void ClearScreens();
private:
	std::vector<std::unique_ptr<UIScreen>> m_Screens;
};

