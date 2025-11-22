#include "UIManager.h"


void UIManager::Init() {
	WidgetShader.loadShader("Shaders\\UIShader_Vert.glsl", "Shaders\\UIShader_Frag.glsl");

	float QuadVerts[] = {
		-0.5, -0.5,
		0.5, -0.5,
		0.5, 0.5,
		-0.5, 0.5
	};
	unsigned int indicies[6] = { 0, 1, 2, 2, 3, 0 };

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), QuadVerts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indicies, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // position
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
}

void UIManager::Update() {
	for (auto& n : m_Screens) { //basically updates all screens
		n.get()->UpdateScreen();
	}
}

void UIManager::Render() {
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	WidgetShader.use();
	WidgetShader.setMat4("projection", ScreenProjection);
	for (auto& n : m_Screens) { //basically renders all screens
		n.get()->RenderScreen(WidgetShader);
	}
}

void UIManager::Terminate() {
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
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


void UIManager::DrawQuad() {
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}