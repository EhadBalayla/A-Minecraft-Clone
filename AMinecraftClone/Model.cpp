#include "Model.h"
#include <glad/glad.h>
#include <vector>

void Model::DrawModel() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, verticiesCount);
    glBindVertexArray(0);
}

void Model::DeleteModel() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

Model* InitModels::InitializeCube() {

    std::vector<modelVertex> cubeVertices = {
        // Front face
        {{-0.5f, -0.5f,  0.5f}, {0, 0, 1}, {0, 0}},
        {{ 0.5f, -0.5f,  0.5f}, {0, 0, 1}, {1, 0}},
        {{ 0.5f,  0.5f,  0.5f}, {0, 0, 1}, {1, 1}},
        {{ 0.5f,  0.5f,  0.5f}, {0, 0, 1}, {1, 1}},
        {{-0.5f,  0.5f,  0.5f}, {0, 0, 1}, {0, 1}},
        {{-0.5f, -0.5f,  0.5f}, {0, 0, 1}, {0, 0}},

        // Back face
        {{ 0.5f, -0.5f, -0.5f}, {0, 0, -1}, {0, 0}},
        {{-0.5f, -0.5f, -0.5f}, {0, 0, -1}, {1, 0}},
        {{-0.5f,  0.5f, -0.5f}, {0, 0, -1}, {1, 1}},
        {{-0.5f,  0.5f, -0.5f}, {0, 0, -1}, {1, 1}},
        {{ 0.5f,  0.5f, -0.5f}, {0, 0, -1}, {0, 1}},
        {{ 0.5f, -0.5f, -0.5f}, {0, 0, -1}, {0, 0}},

        // Left face
        {{-0.5f, -0.5f, -0.5f}, {-1, 0, 0}, {0, 0}},
        {{-0.5f, -0.5f,  0.5f}, {-1, 0, 0}, {1, 0}},
        {{-0.5f,  0.5f,  0.5f}, {-1, 0, 0}, {1, 1}},
        {{-0.5f,  0.5f,  0.5f}, {-1, 0, 0}, {1, 1}},
        {{-0.5f,  0.5f, -0.5f}, {-1, 0, 0}, {0, 1}},
        {{-0.5f, -0.5f, -0.5f}, {-1, 0, 0}, {0, 0}},

        // Right face
        {{ 0.5f, -0.5f,  0.5f}, {1, 0, 0}, {0, 0}},
        {{ 0.5f, -0.5f, -0.5f}, {1, 0, 0}, {1, 0}},
        {{ 0.5f,  0.5f, -0.5f}, {1, 0, 0}, {1, 1}},
        {{ 0.5f,  0.5f, -0.5f}, {1, 0, 0}, {1, 1}},
        {{ 0.5f,  0.5f,  0.5f}, {1, 0, 0}, {0, 1}},
        {{ 0.5f, -0.5f,  0.5f}, {1, 0, 0}, {0, 0}},

        // Top face
        {{-0.5f,  0.5f,  0.5f}, {0, 1, 0}, {0, 0}},
        {{ 0.5f,  0.5f,  0.5f}, {0, 1, 0}, {1, 0}},
        {{ 0.5f,  0.5f, -0.5f}, {0, 1, 0}, {1, 1}},
        {{ 0.5f,  0.5f, -0.5f}, {0, 1, 0}, {1, 1}},
        {{-0.5f,  0.5f, -0.5f}, {0, 1, 0}, {0, 1}},
        {{-0.5f,  0.5f,  0.5f}, {0, 1, 0}, {0, 0}},

        // Bottom face
        {{-0.5f, -0.5f, -0.5f}, {0, -1, 0}, {0, 0}},
        {{ 0.5f, -0.5f, -0.5f}, {0, -1, 0}, {1, 0}},
        {{ 0.5f, -0.5f,  0.5f}, {0, -1, 0}, {1, 1}},
        {{ 0.5f, -0.5f,  0.5f}, {0, -1, 0}, {1, 1}},
        {{-0.5f, -0.5f,  0.5f}, {0, -1, 0}, {0, 1}},
        {{-0.5f, -0.5f, -0.5f}, {0, -1, 0}, {0, 0}},
    };

    Model* cubeModel = new Model();

    cubeModel->verticiesCount = cubeVertices.size();

    glGenVertexArrays(1, &cubeModel->VAO);
    glGenBuffers(1, &cubeModel->VBO);

    glBindVertexArray(cubeModel->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeModel->VBO);
    glBufferData(GL_TRIANGLES, cubeVertices.size() * sizeof(modelVertex), cubeVertices.data(), GL_STATIC_DRAW);


    //verticies attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(modelVertex), (void*)0);
    glEnableVertexAttribArray(0);
    //normals attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(modelVertex), (void*)offsetof(modelVertex, pos));
    glEnableVertexAttribArray(1);
    //UV map attributes
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(modelVertex), (void*)offsetof(modelVertex, normal));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return cubeModel;
}