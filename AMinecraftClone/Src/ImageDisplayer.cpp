#include "ImageDisplayer.h"
#include "Game.h"

void ImageDisplayer::Render() {
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::mat4 trans = glm::rotate(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)), glm::vec3(scale, 1.0f)), rotation, glm::vec3(0.0f, 0.0f, 1.0f));

	tex->bind();

	Game::e_ImageWidgetShader.use();
	Game::e_ImageWidgetShader.setMat4("projection", Game::ScreenProjection);
	Game::e_ImageWidgetShader.setMat4("model", trans);

	Game::e_ImageWidgetShader.setVec2("UV0", uv.uv0);
	Game::e_ImageWidgetShader.setVec2("UV1", uv.uv1);
	Game::e_ImageWidgetShader.setVec2("UV2", uv.uv2);
	Game::e_ImageWidgetShader.setVec2("UV3", uv.uv3);

	glBindVertexArray(Game::tempVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}