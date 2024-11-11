#pragma region INCLUDES
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <vector>
#include "Shader.h"
#include "Object.h"
#include <memory>
#include "OpenGLApp.h"
#include <chrono>
#include <cmath>
#include "stb_image.h"
#pragma endregion INCLUDES


#define BUFFER_OFFSET(i) ((char *)NULL + (i))
Object* obj;
Shader* shader;

#pragma region INPUT_G_VARIABLES
float rotation = 0.0f;
glm::vec3 translation(0.0f);
float scale = 1.0f;

// Mouse tracking variables
int lastX = 400, lastY = 300;
bool firstMouse = true;
bool leftMousePressed = false;

#pragma endregion INPUT_G_VARIABLES


#pragma region INPUT_CONTROLS

void keyboard(unsigned char key, int x, int y) {
	const float moveSpeed = 0.1f;
	switch (key) {
	case 'w': translation.y += moveSpeed; break;
	case 's': translation.y -= moveSpeed; break;
	case 'a': translation.x -= moveSpeed; break;
	case 'd': translation.x += moveSpeed; break;
	case 'q': rotation += 5.0f; break;
	case 'e': rotation -= 5.0f; break;
	case '+': scale *= 1.1f; break;
	case '-': scale /= 1.1f; break;
	case 27:  // ESC key
		delete obj;
		delete shader;
		exit(0);
	}
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		leftMousePressed = (state == GLUT_DOWN);
		if (state == GLUT_DOWN) {
			lastX = x;
			lastY = y;
		}
	}
}

void mouseMotion(int x, int y) {
	if (leftMousePressed) {
		float xoffset = x - lastX;
		float yoffset = lastY - y;
		lastX = x;
		lastY = y;

		const float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		rotation += xoffset;
		translation.y += yoffset * 0.01f;

		glutPostRedisplay();
	}
}
#pragma endregion INPUT_CONTROLS;


#pragma region OGL_HELPER_FUNCTIONS


std::vector<glm::vec4> generateRainbowColors(int numColors) {
	std::vector<glm::vec4> colors(numColors);
	float hueStep = 360.0f / static_cast<float>(numColors);

	for (int i = 0; i < numColors; i++) {
		float hue = static_cast<float>(i) * hueStep;
		float saturation = 1.0f;
		float value = 1.0f;

		// Convert HSV to RGB
		float c = value * saturation;
		float x = c * (1 - std::abs(std::fmod(hue / 60.0f, 2) - 1));
		float m = value - c;
		float r, g, b;

		if (hue >= 0 && hue < 60) {
			r = c, g = x, b = 0;
		}
		else if (hue >= 60 && hue < 120) {
			r = x, g = c, b = 0;
		}
		else if (hue >= 120 && hue < 180) {
			r = 0, g = c, b = x;
		}
		else if (hue >= 180 && hue < 240) {
			r = 0, g = x, b = c;
		}
		else if (hue >= 240 && hue < 300) {
			r = x, g = 0, b = c;
		}
		else {
			r = c, g = 0, b = x;
		}

		colors[i] = glm::vec4(r + m, g + m, b + m, 1.0f);
	}

	return colors;
}


void display() {

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if (obj != nullptr && shader != nullptr) {
		shader->use();

		// Create transformation matrices

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, translation);
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(scale));

		// Pass uniforms to shader
		shader->setUniform("model", model);
		shader->setUniform("texture_one", 0);
		shader->setUniform("texture_two", 1);
		//shader->setUniform("color", glm::vec4(0.0f, greenValue, 0.3f, 1.0f));

		obj->draw();
	}

	glutSwapBuffers();
}

void init() {

	try {
		shader = new Shader("../lab_three/vertex_shader.glsl", "../lab_three/fragment_shader.glsl");
		shader->use();


		std::vector<glm::vec3> vertices = {
			// Front face
			//glm::vec3(-0.5f, -0.5f,  0.5f),
			//glm::vec3(0.5f, -0.5f,  0.5f),
			//glm::vec3(0.5f,  0.5f,  0.5f),
			//glm::vec3(-0.5f, -0.5f,  0.5f),
			//glm::vec3(0.5f,  0.5f,  0.5f),
			//glm::vec3(-0.5f,  0.5f,  0.5f),
			//// Back face
			//glm::vec3(-0.5f, -0.5f, -0.5f),
			//glm::vec3(-0.5f,  0.5f, -0.5f),
			//glm::vec3(0.5f,  0.5f, -0.5f),
			//glm::vec3(-0.5f, -0.5f, -0.5f),
			//glm::vec3(0.5f,  0.5f, -0.5f),
			//glm::vec3(0.5f, -0.5f, -0.5f),
			// Add more faces as needed...
			glm::vec3(0.5f,  0.5f, 0.0f),  // top right
			glm::vec3(0.5f, -0.5f, 0.0f),  // bottom right
			glm::vec3(-0.5f, -0.5f, 0.0f),  // bottom left
			glm::vec3(-0.5f,  0.5f, 0.0f)
		};
		std::vector<glm::vec4> colors = generateRainbowColors(vertices.size());
		//std::vector<glm::vec4> colors(vertices.size(), glm::vec4(0.2f, 0.3f, 0.2f, 0.5f));
		std::vector<GLuint> indices = {
			//0, 1, 2, 2, 3, 0,    // Front face
			//0, 4, 7, 0, 7, 1,    // Left face
			//1, 7, 6, 1, 6, 2,    // Bottom face
			//2, 6, 5, 2, 5, 3,    // Right face
			//3, 5, 4, 3, 4, 0,    // Top face
			//4, 5, 6, 4, 6, 7     // Back face
			0, 1, 2,   // First triangle
			0, 2, 3
		};

		std::vector<glm::vec2> texCoords = {
			// Front face (6 vertices, 2 triangles)
			//glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f),
			//glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f),

			//// Back face (6 vertices, 2 triangles)
			//glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f),
			//glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f),

			//// Left face
			//glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f),
			//glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f),

			//// Bottom face
			//glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f),
			//glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f),

			//// Right face
			//glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f),
			//glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f),

			//// Top face
			//glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f),
			//glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f)
			glm::vec2(1.0f, 1.0f),  // top right
			glm::vec2(1.0f, 0.0f),  // bottom right
			glm::vec2(0.0f, 0.0f),  // bottom left
			glm::vec2(0.0f, 1.0f)
		};

		obj = new Object(
			vertices, 
			colors, 
			indices, 
			"../textures/container.jpg", 
			"../textures/betterface.png",
			texCoords
		);

	}

	catch (const std::exception& e) {
		fprintf(stderr, "Setup error: %s\n", e.what());
	
	}
}

#pragma endregion OGL_HELPER_FUNCTIONS


int main(int argc, char** argv) {


	// Set up the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	/*glutInitWindowSize(800, 600);*/
	glutInitWindowSize(1280, 720);
	glutCreateWindow("PEEEEEEETAHHHHH");


	

	// Initialize GLEW
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	init();


	// 5. Set up display callback
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
		

	// 6. Start the main loop
	glutMainLoop();
	return 0;
	

}

