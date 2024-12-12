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
#include "Camera.h"
#include "Model.h"
#include "Mesh.h"

#pragma endregion INCLUDES

#pragma region DEFINES
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
constexpr auto SCRN_W = 1980;
constexpr auto SCRN_H = 1080;
#pragma endregion DEFINES

#pragma region GLOBAL_VARS
Object* obj;
Shader* shader;
Camera camera(glm::vec3(0.0f, 2.0f, 3.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;
int lastX = SCRN_W / 2, lastY = SCRN_H / 2;


Model* stingrayModel = nullptr;
float stingrayRotation = 0.0f;

#pragma endregion GLOBAL_VARS

#pragma region INPUT_G_VARIABLES
float rotation = 0.0f;
glm::vec3 translation(0.0f);
float scale = 1.0f;

// Mouse tracking variables

bool leftMousePressed = false;

#pragma endregion INPUT_G_VARIABLES


#pragma region INPUT_CONTROLS

void keyboard(unsigned char key, int x, int y) {
	const float moveSpeed = 0.1f;

	if (key == 'w') camera.ProcessKeyboard(FORWARD, deltaTime);
	if (key == 's') camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (key == 'a') camera.ProcessKeyboard(LEFT, deltaTime);
	if (key == 'd') camera.ProcessKeyboard(RIGHT, deltaTime);

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
	static bool firstMouse = true;

	if (firstMouse) {
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	float xOffset = x - lastX;
	float yOffset = lastY - y;
	lastX = x;
	lastY = y;

	camera.ProcessMouseMovement(xOffset, yOffset);
	glutPostRedisplay();
	
}
#pragma endregion INPUT_CONTROLS;


#pragma region OGL_HELPER_FUNCTIONS

float getCurrentTime() {
	static auto start = std::chrono::high_resolution_clock::now();
	auto now = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsed = now - start;
	return elapsed.count();
}


void display() {

	glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float currentFrame = getCurrentTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;


	if (obj != nullptr && shader != nullptr) {
		shader->use();

		// Create transformation matrices

		
		//glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();

		//Defining Perspective Projection 
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(75.0f), static_cast<float>(SCRN_W) / static_cast<float>(SCRN_H), 0.1f, 100.0f);

		//Keyboard controlled movements



		// Pass uniforms to shader
		//shader->setUniform("texture_one", 0);
		//shader->setUniform("texture_two", 1);



		shader->setUniform("view", view);
		shader->setUniform("projection", projection);
		
		//shader->setUniform("color", glm::vec4(0.0f, greenValue, 0.3f, 1.0f));
		glm::mat4 model = glm::mat4(1.0f);
		/*model = glm::translate(model, cubePositions[i]);*/
		float angle = 20.0f + rotation;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		model = glm::translate(model, translation);
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 1.0f));

		//model = glm::scale(model, glm::vec3(scale));
		//shader->setUniform("model", model);
		//obj->draw();




		if (obj != nullptr) {
			glm::mat4 terrainModel = glm::mat4(1.0f);
			terrainModel = glm::translate(terrainModel, glm::vec3(0.0f, -5.0f, 0.0f));
			shader->setUniform("model", terrainModel);
			obj->draw();
		}



		if (stingrayModel != nullptr) {
			glm::mat4 stingrayModelMatrix = glm::mat4(1.0f);

			// Position the stingray above the terrain
			stingrayModelMatrix = glm::translate(stingrayModelMatrix,
				glm::vec3(0.0f, -3.0f, 0.0f));

			// Add some animation
			stingrayRotation += deltaTime * 20.0f; // Rotate 20 degrees per second
			stingrayModelMatrix = glm::rotate(stingrayModelMatrix,
				glm::radians(stingrayRotation), glm::vec3(0.0f, 1.0f, 0.0f));

			// Scale the model if needed
			stingrayModelMatrix = glm::scale(stingrayModelMatrix,
				glm::vec3(0.5f)); // Adjust scale as needed

			shader->setUniform("model", stingrayModelMatrix);
			stingrayModel->Draw(*shader);
		}
		
	}

	glutSwapBuffers();
}

void init() {

	try {
		shader = new Shader("../lab_three/vertex_shader.glsl", "../lab_three/fragment_shader.glsl");
		shader->use();
		int gridSize = 50; // Size of the terrain grid
		float terrainSize = 20.0f; // Physical size of terrain

		std::vector<glm::vec3> terrainVertices = Object::generateTerrain(gridSize, terrainSize);
		std::vector<GLuint> terrainIndices = Object::generateTerrainIndices(gridSize);
		std::vector<glm::vec2> terrainTexCoords = Object::generateTerrainTexCoords(gridSize);

		// Generate colors for terrain (sandy colors)
		std::vector<glm::vec4> terrainColors;
		for (size_t i = 0; i < terrainVertices.size(); i++) {
			terrainColors.push_back(glm::vec4(0.76f, 0.70f, 0.50f, 1.0f)); // Sandy color
		}
		obj = new Object(
			terrainVertices,
			terrainColors,
			terrainIndices,
			"../textures/sand.jpg", 
			"../textures/sand_detailed.png",
			terrainTexCoords
		);
		stingrayModel = new Model("../models/stingray/stingray.obj");
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
	glutCreateWindow("demo");


	

	// Initialize GLEW
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	
	glEnable(GL_DEPTH_TEST);


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

