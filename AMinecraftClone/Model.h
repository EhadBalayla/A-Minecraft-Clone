#pragma once
#include <iostream>

class Model
{
public:
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	uint32_t indexCount;

	void DrawModel();
	void DeleteModel();
};

namespace InitModels {
	Model* InitializeCube();
}

