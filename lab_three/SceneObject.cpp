#include "SceneObject.h"
#include "Level.h"
SceneObject::SceneObject(Model* model, std::vector<glm::vec3> positions, glm::vec3 scale)
	: model(model), positions(positions), scale(scale){}


void SceneObject::render(Shader& shader)
{
    
    
    for (auto& pos : positions) {
        std::cout << "Generating scene object" << std::endl;
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, pos);
        modelMatrix = glm::scale(modelMatrix, scale);
        shader.setUniform("model", modelMatrix);
        // Add a debug check
        if (model == nullptr) {
            std::cerr << "Error: Null model pointer!" << std::endl;
            continue;
        }
        model->Draw(shader);
    }
}
