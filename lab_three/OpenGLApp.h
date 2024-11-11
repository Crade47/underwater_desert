#pragma once
#include <memory>
#include "Shader.h"
#include "Object.h"
#include <glm/ext.hpp>
#include <GL/freeglut.h>
class OpenGLApp
{
public:
	void init();
	static void displayCallback();
	static OpenGLApp* getInstance();
	void render();
private:
	static OpenGLApp* instance;
	std::unique_ptr<Shader> shader;
	std::unique_ptr<Object> triangle;
	OpenGLApp() {}
};

