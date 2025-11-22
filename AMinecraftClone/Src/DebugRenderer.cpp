#include "DebugRenderer.h"
#include "Game.h"

void DebugRenderer::DrawChunkBoundaries() {
	if (Game::ShowChunkBorder) {
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glLineWidth(5.0f);

		Game::e_ChunkBorderShader.use();
		Game::e_ChunkBorderShader.setMat4("view", Game::View);
		Game::e_ChunkBorderShader.setMat4("projection", Game::Proj);
		glm::ivec2 chunkCoords = Game::player.GetCurrentChunkCoords();
		glm::dvec3 relativePos = glm::dvec3(chunkCoords.x * 16, 0.0f, chunkCoords.y * 16) - Game::player.GetPosition();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(relativePos.x, -Game::player.GetPosition().y, relativePos.z));
		Game::e_ChunkBorderShader.setMat4("model", transform);
		
		glBindVertexArray(Game::tempVAO);
		glDrawArrays(GL_LINES, 0, 24);
	}
}