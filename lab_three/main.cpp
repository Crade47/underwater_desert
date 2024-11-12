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

#pragma region DEFINES
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
constexpr auto SCRN_W = 1980;
constexpr auto SCRN_H = 1080;
#pragma endregion DEFINES

#pragma region GLOBAL_VARS
Object* obj;
Shader* shader;

glm::vec3 cubePositions[] = {
glm::vec3(0.0f,  0.0f,  0.0f),
glm::vec3(2.0f,  5.0f, -15.0f),
glm::vec3(-1.5f, -2.2f, -2.5f),
glm::vec3(-3.8f, -2.0f, -12.3f),
glm::vec3(2.4f, -0.4f, -3.5f),
glm::vec3(-1.7f,  3.0f, -7.5f),
glm::vec3(1.3f, -2.0f, -2.5f),
glm::vec3(1.5f,  2.0f, -2.5f),
glm::vec3(1.5f,  0.2f, -1.5f),
glm::vec3(-1.3f,  1.0f, -1.5f)
};
#pragma endregion GLOBAL_VARS

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


	if (key == 'w') translation.y += moveSpeed;
	if (key == 's') translation.y -= moveSpeed;
	if (key == 'a') translation.x -= moveSpeed;
	if (key == 'd') translation.x += moveSpeed;

	if (key == 'q') rotation += 5.0f;
	if (key == 'e') rotation -= 5.0f;

	if (key == '+') scale += 1.1f;
	if (key == '-') scale -= 1.1f;

	if (key == 27) {
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

float getCurrentTime() {
	static auto start = std::chrono::high_resolution_clock::now();
	auto now = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsed = now - start;
	return elapsed.count();
}

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

		
		glm::mat4 view = glm::mat4(1.0f);

		//Defining view
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		//Defining Perspective Projection 
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(75.0f), static_cast<float>(SCRN_W) / static_cast<float>(SCRN_H), 0.1f, 100.0f);

		//Keyboard controlled movements


		// Pass uniforms to shader
		shader->setUniform("texture_one", 0);
		shader->setUniform("texture_two", 1);

		shader->setUniform("view", view);
		shader->setUniform("projection", projection);
		
		//shader->setUniform("color", glm::vec4(0.0f, greenValue, 0.3f, 1.0f));

		for (unsigned int i = 0; i < 10; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i + rotation;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			model = glm::translate(model, translation);
			model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 1.0f));

			model = glm::scale(model, glm::vec3(scale));
			shader->setUniform("model", model);
			obj->draw();
		}
		
	}

	glutSwapBuffers();
}

void init() {

	try {
		shader = new Shader("../lab_three/vertex_shader.glsl", "../lab_three/fragment_shader.glsl");
		shader->use();


		std::vector<glm::vec3> vertices = {
			// Front face
			glm::vec3(-0.5f, -0.5f,  0.5f),  // 0
			glm::vec3(0.5f, -0.5f,  0.5f),  // 1
			glm::vec3(0.5f,  0.5f,  0.5f),  // 2
			glm::vec3(-0.5f,  0.5f,  0.5f),  // 3

			// Back face
			glm::vec3(-0.5f, -0.5f, -0.5f),  // 4
			glm::vec3(0.5f, -0.5f, -0.5f),  // 5
			glm::vec3(0.5f,  0.5f, -0.5f),  // 6
			glm::vec3(-0.5f,  0.5f, -0.5f),  // 7

			// Top face
			glm::vec3(-0.5f,  0.5f, -0.5f),  // 8
			glm::vec3(0.5f,  0.5f, -0.5f),  // 9
			glm::vec3(0.5f,  0.5f,  0.5f),  // 10
			glm::vec3(-0.5f,  0.5f,  0.5f),  // 11

			// Bottom face
			glm::vec3(-0.5f, -0.5f, -0.5f),  // 12
			glm::vec3(0.5f, -0.5f, -0.5f),  // 13
			glm::vec3(0.5f, -0.5f,  0.5f),  // 14
			glm::vec3(-0.5f, -0.5f,  0.5f),  // 15

			// Right face
			glm::vec3(0.5f, -0.5f, -0.5f),  // 16
			glm::vec3(0.5f,  0.5f, -0.5f),  // 17
			glm::vec3(0.5f,  0.5f,  0.5f),  // 18
			glm::vec3(0.5f, -0.5f,  0.5f),  // 19

			// Left face
			glm::vec3(-0.5f, -0.5f, -0.5f),  // 20
			glm::vec3(-0.5f,  0.5f, -0.5f),  // 21
			glm::vec3(-0.5f,  0.5f,  0.5f),  // 22
			glm::vec3(-0.5f, -0.5f,  0.5f)   // 23
		};

		std::vector<glm::vec4> colors = generateRainbowColors(vertices.size());
		std::vector<GLuint> indices = {
			// Front face
			0,  1,  2,     0,  2,  3,
			// Back face
			4,  5,  6,     4,  6,  7,
			// Top face
			8,  9,  10,    8,  10, 11,
			// Bottom face
			12, 13, 14,    12, 14, 15,
			// Right face
			16, 17, 18,    16, 18, 19,
			// Left face
			20, 21, 22,    20, 22, 23
		};
		std::vector<glm::vec2> texCoords = {
			// Front face
			glm::vec2(0.0f, 0.0f),  // 0
			glm::vec2(1.0f, 0.0f),  // 1
			glm::vec2(1.0f, 1.0f),  // 2
			glm::vec2(0.0f, 1.0f),  // 3

			// Back face
			glm::vec2(1.0f, 0.0f),  // 4
			glm::vec2(0.0f, 0.0f),  // 5
			glm::vec2(0.0f, 1.0f),  // 6
			glm::vec2(1.0f, 1.0f),  // 7

			// Top face
			glm::vec2(0.0f, 0.0f),  // 8
			glm::vec2(1.0f, 0.0f),  // 9
			glm::vec2(1.0f, 1.0f),  // 10
			glm::vec2(0.0f, 1.0f),  // 11

			// Bottom face
			glm::vec2(0.0f, 0.0f),  // 12
			glm::vec2(1.0f, 0.0f),  // 13
			glm::vec2(1.0f, 1.0f),  // 14
			glm::vec2(0.0f, 1.0f),  // 15

			// Right face
			glm::vec2(0.0f, 0.0f),  // 16
			glm::vec2(1.0f, 0.0f),  // 17
			glm::vec2(1.0f, 1.0f),  // 18
			glm::vec2(0.0f, 1.0f),  // 19

			// Left face
			glm::vec2(0.0f, 0.0f),  // 20
			glm::vec2(1.0f, 0.0f),  // 21
			glm::vec2(1.0f, 1.0f),  // 22
			glm::vec2(0.0f, 1.0f)   // 23
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
	glutInitWindowSize(SCRN_W, SCRN_H);
	glutCreateWindow("PEEEEEEETAHHHHH");


	

	// Initialize GLEW
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CW);
	//glCullFace(GL_BACK);

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

