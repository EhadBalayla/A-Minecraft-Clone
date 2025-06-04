#pragma once
#include <iostream>
#include <glm/glm.hpp>

struct modelVertex { 
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
};

class Model
{
public:
	unsigned int VBO;
	unsigned int VAO;
	uint32_t verticiesCount;

	void DrawModel();
	void DeleteModel();
};

namespace InitModels {
	Model* InitializeCube();
}

