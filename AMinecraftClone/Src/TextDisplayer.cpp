#include "TextDisplayer.h"
#include "Game.h"

#include "glm/gtc/matrix_transform.hpp"

glm::vec2 reg_lowercase_size = glm::vec2(1.0f, 1.0f);
glm::vec2 tall_lowercase_size = glm::vec2(1.0f, 1.4f);
float tall_lowercase_height = 0.2f;
float reg_lowercase_advance = 1.2f;

glm::vec2 reg_uppercase_size = glm::vec2(1.0f, 1.4f);
float reg_uppercase_height = 0.2f;

std::unordered_map<char, Letter> letters = {
	{'a',{ {glm::vec2(0.0625, 0.390625), glm::vec2(0.1015625, 0.390625),
				glm::vec2(0.1015625, 0.4296875), glm::vec2(0.0625, 0.4296875), },

				reg_lowercase_size,

				reg_lowercase_advance,

				0.0f }},
	{'c', { {glm::vec2(0.1875, 0.390625), glm::vec2(0.2265625, 0.390625),
				glm::vec2(0.2265625, 0.4296875), glm::vec2(0.1875, 0.4296875), },

				reg_lowercase_size,

				reg_lowercase_advance,

				0.0f }},
	{'d', { {glm::vec2(0.25, 0.375), glm::vec2(0.2890625, 0.375),
				glm::vec2(0.2890625, 0.4296875), glm::vec2(0.25, 0.4296875), },

				tall_lowercase_size,

				reg_lowercase_advance,

				tall_lowercase_height }},
	{'e', { {glm::vec2(0.3125, 0.390625), glm::vec2(0.3515625, 0.390625),
				glm::vec2(0.3515625, 0.4296875), glm::vec2(0.3125, 0.4296875), },

				reg_lowercase_size,

				reg_lowercase_advance,

				0.0f }},
	{'f', { {glm::vec2(0.375, 0.375), glm::vec2(0.40625, 0.375),
				glm::vec2(0.40625, 0.4296875), glm::vec2(0.375, 0.4296875), },

				{0.8f, 1.4f},

				1.1f,

				tall_lowercase_height }},
	{'v', { {glm::vec2(0.375, 0.453125), glm::vec2(0.4140625, 0.453125),
				glm::vec2(0.4140625, 0.4921875), glm::vec2(0.375, 0.4921875), },

				reg_lowercase_size,

				reg_lowercase_advance,

				0.0f }},
	{'i', { {glm::vec2(0.5625, 0.375), glm::vec2(0.5703125, 0.375),
				glm::vec2(0.5703125, 0.4296875), glm::vec2(0.5625, 0.4296875), },

				{0.2f, 1.4f},

				0.8f,

				tall_lowercase_height }},
	{'n', { {glm::vec2(0.875, 0.390625), glm::vec2(0.9140625, 0.390625),
				glm::vec2(0.9140625, 0.4296875), glm::vec2(0.875, 0.4296875), },

				reg_lowercase_size,

				reg_lowercase_advance,

				0.0f }},
	{'r', { {glm::vec2(0.125, 0.453125), glm::vec2(0.1640625, 0.453125),
				glm::vec2(0.1640625, 0.4921875), glm::vec2(0.125, 0.4921875), },

				reg_lowercase_size,

				reg_lowercase_advance,

				0.0f }},
	{'t', { {glm::vec2(0.25, 0.4375), glm::vec2(0.2734375, 0.4375),
				glm::vec2(0.2734375, 0.4921875), glm::vec2(0.25, 0.4921875), },

				{0.6f, 1.4f},

				reg_lowercase_advance,

				tall_lowercase_height }},



	{'M', { {glm::vec2(0.8125, 0.25), glm::vec2(0.8515625, 0.25),
				glm::vec2(0.8515625, 0.3046875), glm::vec2(0.8125, 0.3046875), },

				reg_uppercase_size,

				0.8f,

				reg_uppercase_height }},
	{'I', { {glm::vec2(0.5625, 0.25), glm::vec2(0.5859375, 0.25),
				glm::vec2(0.5859375, 0.3046875), glm::vec2(0.5625, 0.3046875), },

				glm::vec2(0.6f, 1.4f),

				1.0f,

				reg_uppercase_height }}
};

void TextDisplayer::setText(const std::string& newText) {
	std::string txt = newText;
	CharUVs.clear();
	for (auto& c : txt) {
		switch (c) {
		case 'a':
			CharUVs.push_back(letters['a']);
			break;
		case 'd':
			CharUVs.push_back(letters['d']);
			break;
		case 'v':
			CharUVs.push_back(letters['v']);
			break;
		case 'i':
			CharUVs.push_back(letters['i']);
			break;
		case 'n':
			CharUVs.push_back(letters['n']);
			break;
		case 'e':
			CharUVs.push_back(letters['e']);
			break;
		case 'c':
			CharUVs.push_back(letters['c']);
			break;
		case 'r':
			CharUVs.push_back(letters['r']);
			break;
		case 'f':
			CharUVs.push_back(letters['f']);
			break;
		case 't':
			CharUVs.push_back(letters['t']);
			break;
		case ' ':
			CharUVs.push_back({ {glm::vec2(0.0, 0.0), glm::vec2(0.0, 0.0),
				glm::vec2(0.0, 0.0), glm::vec2(0.0, 0.0), },

				{1.0f, 1.0f},

				1.1f,

				0.0f });
			break;
		case 'M': 
			CharUVs.push_back(letters['M']);
			break;
		case 'I':
			CharUVs.push_back(letters['I']);
			break;
		default:
			//defaults to the character 'a'
			CharUVs.push_back(letters['a']);
			break;
		}
	}
}

void TextDisplayer::Render() {
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	Game::fontTex.bind();
	Game::e_ImageWidgetShader.use();

	float advance = 0.0f;
	for (auto& c : CharUVs) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position.x + advance, position.y - c.height * scale.y, 0.0f));
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0f));
		model = glm::scale(model, glm::vec3(scale.x * c.size.x, scale.y * c.size.y, 1.0));
		Game::e_ImageWidgetShader.setMat4("model", model);

		Game::e_ImageWidgetShader.setVec2("UV0", c.uvs.uv0);
		Game::e_ImageWidgetShader.setVec2("UV1", c.uvs.uv1);
		Game::e_ImageWidgetShader.setVec2("UV2", c.uvs.uv2);
		Game::e_ImageWidgetShader.setVec2("UV3", c.uvs.uv3);

		glBindVertexArray(Game::tempVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		advance += c.advance * scale.x;
	}
}