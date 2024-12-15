#pragma once
#include <vector>
#include "Camera.h"
#include "Shader.h"
#include "SceneObject.h"
class Level
{
private:
    std::vector<SceneObject*> sceneObjects;
    Shader* shader;
    Camera* camera;
    int scrn_w;
    int scrn_h;
public:
    Level(Shader* shader, Camera* camera, const int scrn_w, const int scrn_h);

    ~Level();

    void addObject(SceneObject* obj);

    void update(float deltaTime);

    void render();
};

