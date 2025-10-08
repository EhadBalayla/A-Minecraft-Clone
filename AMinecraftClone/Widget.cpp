#include "Widget.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"

Widget::Widget() {

}
Widget::Widget(UIUVQuad uvs) {
	this->uvs = uvs;
}

Widget::~Widget() {

}

void Widget::RenderWidget(Shader& shader) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0f));
	model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0));
	shader.setMat4("model", model);

	shader.setVec2("UV0", uvs.uv0);
	shader.setVec2("UV1", uvs.uv1);
	shader.setVec2("UV2", uvs.uv2);
	shader.setVec2("UV3", uvs.uv3);

	m_Texture->bind();

	Game::m_UIManager.DrawQuad();
}

void Widget::UpdateWidget() {

}

void Widget::AssignTexture(Texture* texture) {
	m_Texture = texture;
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

glm::vec2 Widget::GetPosition() {
	return position;
}
float Widget::GetRotation() {
	return rotation;
}
glm::vec2 Widget::GetScale() {
	return scale;
}