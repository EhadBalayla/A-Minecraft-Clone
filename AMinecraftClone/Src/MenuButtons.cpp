#include "MenuButtons.h"
#include "Game.h"

UIUVQuad buttonOffUV = { glm::vec2(0.0, 0.1796875), glm::vec2(0.78125, 0.1796875), glm::vec2(0.78125, 0.2578125), glm::vec2(0.0, 0.2578125) };
UIUVQuad buttonNormalUV = { glm::vec2(0.0, 0.2578125), glm::vec2(0.78125, 0.2578125), glm::vec2(0.78125, 0.3359375), glm::vec2(0.0, 0.3359375) };
UIUVQuad buttonHoveredUV = { glm::vec2(0.0, 0.3359375), glm::vec2(0.78125, 0.3359375), glm::vec2(0.78125, 0.4140625), glm::vec2(0.0, 0.4140625) };

float FindTextSize(const std::string& text) {
	float size = 0.0f;
	const float Scale = 3.0f;
	for (auto& c : text) {
		size += Scale * Game::e_FontRegistery[c].width;
	}
	return size;
}

void MenuButtons::Render() {
	//render the button image
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glm::mat4 trans = glm::rotate(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)), glm::vec3(scale, 1.0f)), rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	
	Game::guiAtlas.bind();

	Game::e_ImageWidgetShader.use();
	Game::e_ImageWidgetShader.setMat4("projection", Game::ScreenProjection);
	Game::e_ImageWidgetShader.setMat4("model", trans);

	if (IsHovered) {
		Game::e_ImageWidgetShader.setVec2("UV0", buttonHoveredUV.uv0);
		Game::e_ImageWidgetShader.setVec2("UV1", buttonHoveredUV.uv1);
		Game::e_ImageWidgetShader.setVec2("UV2", buttonHoveredUV.uv2);
		Game::e_ImageWidgetShader.setVec2("UV3", buttonHoveredUV.uv3);
	}
	else {
		Game::e_ImageWidgetShader.setVec2("UV0", buttonNormalUV.uv0);
		Game::e_ImageWidgetShader.setVec2("UV1", buttonNormalUV.uv1);
		Game::e_ImageWidgetShader.setVec2("UV2", buttonNormalUV.uv2);
		Game::e_ImageWidgetShader.setVec2("UV3", buttonNormalUV.uv3);
	}


	glBindVertexArray(Game::tempVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//render the text
	text.position = glm::vec2(position.x - FindTextSize(text.GetText()) / 2.0f, position.y);
	if (!IsHovered) text.setTint(glm::vec4(1.0f));
	else text.setTint(glm::vec4(1.0f, 1.0f, 0.5f, 1.0f));
	text.Render();
}
void MenuButtons::Update() {
	int MouseX = 0;
	int MouseY = 0;
	auto mouseState = SDL_GetMouseState(&MouseX, &MouseY);

	if ((MouseX > position.x - (scale.x / 2.0f) && MouseX < position.x + (scale.x / 2.0f)) &&
		(MouseY > position.y - (scale.y / 2.0f) && MouseY < position.y + (scale.y / 2.0f))) {
		if (!IsHovered) { //if wasn't hovered already then make hovered
			IsHovered = true;
		}

		if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			if (!FirstClick) {
				FirstClick = true;
				Callback();
			}
		}
		else {
			if (FirstClick)
				FirstClick = false;
		}
	}
	else {
		if (IsHovered) {
			IsHovered = false;
		}
	}
}

void MenuButtons::SetText(const std::string& newText) {
	text.setText(newText);
}