#include "Widget.h"
#include <glm/gtc/matrix_transform.hpp>

Widget::Widget(UIUVQuad uvs) {
	verticies[0] = { glm::vec2(-0.5, -0.5), uvs.uv0 }; //bottom left
	verticies[1] = { glm::vec2(0.5, -0.5), uvs.uv1 }; //bottom right
	verticies[2] = { glm::vec2(0.5, 0.5), uvs.uv2 }; // top right
	verticies[3] = { glm::vec2(-0.5, 0.5), uvs.uv3 };// top left


	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	MakeData();
}

Widget::~Widget() {
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
}

void Widget::RenderWidget(Shader& shader) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0f));
	model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0));
	shader.setMat4("model", model);

	m_Texture->bind();

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Widget::UpdateWidget() {

}

void Widget::AssignTexture(Texture* texture) {
	m_Texture = texture;
}

void Widget::MakeData() {
	
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(UIVertex), verticies, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indicies, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // position
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(UIVertex), (void*)0);

	glEnableVertexAttribArray(1); // uv
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(UIVertex), (void*)offsetof(UIVertex, uv));

}

void Widget::SetPosition(glm::vec2 newPosition) {
	position = newPosition;
}

void Widget::SetRotation(float newRot) {
	rotation = newRot;
}

void Widget::SetScale(glm::vec2 newScale) {
	scale = newScale;
}
