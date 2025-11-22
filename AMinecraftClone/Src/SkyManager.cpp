#include "SkyManager.h"
#include "Game.h"

void SkyManager::RenderSky() {
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	

	Game::e_SkyColorShader.use();
	Game::e_SkyColorShader.setMat4("view", Game::player.getViewOnlyMatrix());
	Game::e_SkyColorShader.setMat4("proj", Game::Proj);
	Game::e_SkyColorShader.setFloat("time", (SDL_GetTicks() / 1000.0f) * 0.01f);

	glBindVertexArray(Game::tempVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void SkyManager::RenderClouds() {
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);


	Game::e_CloudShader.use();
	Game::e_CloudShader.setMat4("view", Game::View);
	Game::e_CloudShader.setMat4("projection", Game::Proj);
	glm::dvec3 cloudPos = glm::dvec3(0.0f, 128.0f, 0.0f);
	glm::vec3 relativePos = cloudPos - Game::player.GetPosition();
	glm::mat4 cloudsMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, relativePos.y, 0.0f));
	cloudsMatrix = glm::scale(cloudsMatrix, glm::vec3(1024.0f, 1.0f, 1024.0f));
	Game::e_CloudShader.setMat4("model", cloudsMatrix);
	Game::e_CloudShader.setFloat("Time", SDL_GetTicks() / 1000.0f);
	Game::e_CloudShader.setVec3("playerWorldPos", Game::player.GetPosition());
	Game::e_LoadedTextures[4]->bind();
	

	glBindVertexArray(Game::tempVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}