#include "TextDisplayer.h"
#include "Game.h"

#include "glm/gtc/matrix_transform.hpp"

void TextDisplayer::setText(const std::string& newText) {
	Text = newText;
}
void TextDisplayer::setTint(const glm::vec4& newTint) {
	Tint = newTint;
}
const std::string& TextDisplayer::GetText() const {
	return Text;
}
void TextDisplayer::Render() {
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	Game::fontTex.bind();
	Game::e_TextShader.use();
	Game::e_TextShader.setMat4("projection", Game::ScreenProjection);

	float CursorX = position.x;
	float CursorY = position.y;

	const float Scale = 3.0f;

	for (auto& c : Text) {
		//setup basic values
		auto it = Game::e_FontRegistery.find(c);
		if (it == Game::e_FontRegistery.end()) continue;

		const Glyph& g = it->second;

		glm::mat4 model = glm::mat4(1.0f);

		Game::e_TextShader.setVec2("UV0", g.uv0);
		Game::e_TextShader.setVec2("UV1", g.uv1);

		//render shadow
		model = glm::translate(model, glm::vec3(CursorX + Scale, CursorY + Scale, 0.0f));
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0f));
		model = glm::scale(model, glm::vec3(8.0 * Scale, 8.0 * Scale, 1.0));

		Game::e_TextShader.setMat4("model", model);
		Game::e_TextShader.setVec4("Tint", glm::vec4(0.0f, 0.0f, 0.0f, 0.5f));

		glBindVertexArray(Game::tempVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//render normal text
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(CursorX, CursorY, 0.0f));
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0f));
		model = glm::scale(model, glm::vec3(8.0 * Scale, 8.0 * Scale, 1.0));

		Game::e_TextShader.setMat4("model", model);
		Game::e_TextShader.setVec4("Tint", Tint);

		glDrawArrays(GL_TRIANGLES, 0, 6);


		//advancing the cursor
		CursorX += g.width * Scale;
	}
}