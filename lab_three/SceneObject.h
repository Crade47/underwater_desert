#pragma once
#include "vector"
#include "Model.h"
#include "Camera.h"
class SceneObject
{
public:
    Model* model;
    std::vector<glm::vec3> positions;
    glm::vec3 scale;
    SceneObject(Model* model, std::vector<glm::vec3> positions, glm::vec3 scale);
    void render(Shader& shader);
};

