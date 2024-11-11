//#include "OpenGLApp.h"
//#include <iostream>
//
//void OpenGLApp::init()
//{
//	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//
//	std::vector<glm::vec3> vertices = {
//	glm::vec3(-1.0f, -1.0f, 0.0f),
//	glm::vec3(1.0f, -1.0f, 0.0f),
//	glm::vec3(0.0f, 1.0f, 0.0f)
//	};
//	// Create a color array that identfies the colors of each vertex (format R, G, B, A)
//	std::vector<glm::vec4> colors = {
//		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  // Red
//		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),  // Green
//		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)   // Blue
//	};
//	shader = std::make_unique<Shader>("../lab_three/vertex_shader.glsl", "../lab_three/fragment_shader.glsl");
//	triangle = std::make_unique<Object>(vertices, colors);
//
//}
//
//void OpenGLApp::render() {
//	glClear(GL_COLOR_BUFFER_BIT);
//	shader->use();
//	triangle->draw();
//	glutSwapBuffers();
//}
//
//void OpenGLApp::displayCallback()
//{
//	getInstance()->render();
//}
//
//OpenGLApp* OpenGLApp::getInstance()
//{
//	if (instance == nullptr) {
//		instance = new OpenGLApp();
//	}
//	return instance;
//}
//
//OpenGLApp* OpenGLApp::instance = nullptr;
//
//

#include "OpenGLApp.h"
#include <iostream>

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length,
    const GLchar* message, const void* userParam) {
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
}

void OpenGLApp::init()
{
    // Enable debug output
    glEnable(GL_DEBUG_OUTPUT);


    // Set clear color and viewport
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glViewport(0, 0, 800, 600);

    std::vector<glm::vec3> vertices = {
        glm::vec3(-0.5f, -0.5f, 0.0f),  // Changed vertices to be smaller
        glm::vec3(0.5f, -0.5f, 0.0f),
        glm::vec3(0.0f, 0.5f, 0.0f)
    };

    std::vector<glm::vec4> colors = {
        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
        glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)
    };

    // Create shader first
    shader = std::make_unique<Shader>("../lab_three/vertex_shader.glsl", "../lab_three/fragment_shader.glsl");
    if (!shader->isValid()) {
        std::cerr << "Failed to create shader program!" << std::endl;
        return;
    }

    // Then create triangle
    triangle = std::make_unique<Object>(vertices, colors);

    // Check for errors
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error during init: " << err << std::endl;
    }
}

void OpenGLApp::displayCallback()
{
}

OpenGLApp* OpenGLApp::getInstance()
{
    return nullptr;
}

void OpenGLApp::render() {
    glClear(GL_COLOR_BUFFER_BIT);

    shader->use();
    triangle->draw();

    // Check for errors
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error during render: " << err << std::endl;
    }

    glutSwapBuffers();
    glutPostRedisplay();  // Request a redraw for continuous rendering
}