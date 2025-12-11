#include "ItemDisplayer.h"
#include "Game.h"

void ItemDisplayer::RenderWidget(Shader& shader) {
	glDisable(GL_DEPTH_TEST);


	glm::mat4 transform = glm::translate(glm::rotate(glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f)), rotation, glm::vec3(0.0f, 0.0f, 1.0f)), glm::vec3(position, 0.0f));

	Game::e_InventoryBlockShader.use();
	Game::e_InventoryBlockShader.setMat4("proj", Game::m_UIManager.ScreenProjection);
	Game::e_InventoryBlockShader.setMat4("model", transform);

	glBindVertexArray(Game::tempVAO);
	glDrawArrays(GL_TRIANGLES, 0, 18);
}