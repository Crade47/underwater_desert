#pragma once
#include <vector>
#include "Camera.h"
#include "Shader.h"
#include "SceneObject.h"

struct DirectionalLight {
    //light.lightSource = vec3(1.0, 0.0, 0.0);
    glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.3f);
    glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.6f);
    glm::vec3 specular = glm::vec3(0.7f, 0.7f, 0.8f);
    glm::vec3 color = glm::vec3(0.1f, 0.3f, 0.5f);
};

class Level
{
private:
    std::vector<SceneObject*> sceneObjects;
    Shader* shader;
    Camera* camera;
    int scrn_w;
    int scrn_h;
    struct DirectionalLight sunLight;

public:
    Level(Shader* shader, Camera* camera, const int scrn_w, const int scrn_h);

    ~Level();

    void addObject(SceneObject* obj);

    void update(float deltaTime);

    void render();
};

