#include "Level.h"

Level::Level(Shader* shader, Camera* camera, const int scrn_w, const int scrn_h) 
    : shader(shader), camera(camera), scrn_w(scrn_w), scrn_h(scrn_h) {

}

Level::~Level()
{
    for (auto obj : sceneObjects) {
        delete obj;
    }
}

void Level::addObject(SceneObject* obj)
{
    sceneObjects.push_back(obj);
}

void Level::update(float deltaTime)
{
}

void Level::render()
{

    shader->use();
    //glm::mat4 view = camera->GetViewMatrix();
    //glm::mat4 projection = glm::perspective(glm::radians(75.0f), static_cast<float>(scrn_w) / scrn_h, 0.1f, 100.0f);

    //shader->setUniform("view", view);
    //shader->setUniform("projection", projection);
    shader->setUniform("viewPos", camera->Position);


    shader->setUniform("material.shininess", 32.0f);


    for (SceneObject* obj : sceneObjects) {
        obj->render(*shader);
    }
}
