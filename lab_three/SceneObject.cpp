#include "SceneObject.h"

SceneObject::SceneObject(Model* model, std::vector<glm::vec3> positions, glm::vec3 scale)
	: model(model), positions(positions), scale(scale){}


void SceneObject::render(Shader& shader)
{
    
    
    for (auto& pos : positions) {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, pos);
        modelMatrix = glm::scale(modelMatrix, scale);
        shader.setUniform("model", modelMatrix);
        model->Draw(shader);
    }
}
