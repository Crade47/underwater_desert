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
#include "SceneGenerator.h"
#include "Level.h"

#pragma endregion INCLUDES

#pragma region DEFINES
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
constexpr auto SCRN_W = 1980;
constexpr auto SCRN_H = 1080;
#pragma endregion DEFINES

#pragma region GLOBAL_VARS
Object* obj;
Shader* shader;
Camera* camera = new Camera(glm::vec3(-15.0f, 10.0f, 15.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;
int lastX = SCRN_W / 2, lastY = SCRN_H / 2;



Level* underwaterDesert;


#pragma endregion GLOBAL_VARS

#pragma region INPUT_G_VARIABLES

glm::vec3 translation(0.0f);
float scale = 1.0f;

// Mouse tracking variables

bool leftMousePressed = false;

#pragma endregion INPUT_G_VARIABLES


#pragma region INPUT_CONTROLS

void keyboard(unsigned char key, int x, int y) {


	if (key == 'w') camera->ProcessKeyboard(FORWARD, deltaTime);
	if (key == 's') camera->ProcessKeyboard(BACKWARD, deltaTime);
	if (key == 'a') camera->ProcessKeyboard(LEFT, deltaTime);
	if (key == 'd') camera->ProcessKeyboard(RIGHT, deltaTime);



	if (key == '+') scale += 1.1f;
	if (key == '-') scale -= 1.1f;

	if (key == 27) {
		delete obj;
		delete shader;
		delete underwaterDesert;
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

	camera->ProcessMouseMovement(xOffset, yOffset);
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

	float currentTime = getCurrentTime();
	deltaTime = currentTime - lastFrame;
	lastFrame = currentTime;
	shader->setUniform("time", currentTime);

	if (shader != nullptr) {
		shader->use();



		// Create transformation matrices

		glm::mat4 view = camera->GetViewMatrix();

		//Defining Perspective Projection 
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(75.0f), static_cast<float>(SCRN_W) / static_cast<float>(SCRN_H), 0.1f, 100.0f);



		shader->setUniform("view", view);
		shader->setUniform("projection", projection);


		underwaterDesert->render();

		if (obj != nullptr) {
			glm::mat4 terrainModel = glm::mat4(1.0f);
			terrainModel = glm::translate(terrainModel, glm::vec3(0.0f, -5.0f, 0.0f));
			shader->setUniform("model", terrainModel);
			obj->draw();
		}


	
		glutSwapBuffers();
	}
}

void init() {

	try {
		shader = new Shader("../lab_three/vertex_shader.glsl", "../lab_three/fragment_shader.glsl");
		shader->use();
		int gridSize = 70; // Size of the terrain grid
		float terrainSize = 40.0f; // Physical size of terrain

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

		Model* stingrayModel = new Model("../models/stingray/source/Common_Stingray_02.obj");
		Model* coral_pink_model = new Model("../models/coral-pink/untitled.obj");
		Model* coral_gon_model = new Model("../models/coral-gon/coral-gon.obj");
		Model* coral_pocil_model = new Model("../models/coral-pocil/coral-pocil.obj");
		Model* coral_stone_model = new Model("../models/coral-stone/model.obj");
		Model* life_jellyfish_model = new Model("../models/life-jellyfish/life-jellyfish.obj");
		Model* life_guppy_model = new Model("../models/life-guppy/life-guppy.obj");

		CoralPosParams coral_params;
		coral_params.maxY = -4.0f;
		auto coralPinkPositions = SceneGenerator::generateCoralPositions(13, coral_params);
		coral_params.maxY = -4.8f;
		auto coralGonPositions = SceneGenerator::generateCoralPositions(24, coral_params);
		coral_params.maxY = -4.9f;
		auto coralPocilPositions = SceneGenerator::generateCoralPositions(17, coral_params);
		coral_params.maxY = -4.2f;
		auto coralStonePositions = SceneGenerator::generateCoralPositions(9, coral_params);
		
		SceneObject* coralPinkObjects = new SceneObject(
			coral_pink_model,
			coralPinkPositions,
			glm::vec3(0.5f)
		);
		SceneObject* coralGonObjects = new SceneObject(
			coral_gon_model,
			coralGonPositions,
			glm::vec3(0.5f)
		);
		
		SceneObject* coralPocilObjects = new SceneObject(
			coral_pocil_model,
			coralPocilPositions,
			glm::vec3(0.15f)
		);
		
		SceneObject* coralStoneObjects = new SceneObject(
			coral_stone_model,
			coralStonePositions,
			glm::vec3(0.09f)
		);


		glm::vec3 fishSchoolCenter(6.0f, -1.0f, 10.0f);  // Adjust as needed
		FishSchoolParams school_params;
		school_params.numFish = 60;
		school_params.spread = 10;
		auto fishPositions = SceneGenerator::generateFishSchool(fishSchoolCenter, school_params);
		SceneObject* fishSchool = new SceneObject(
			life_guppy_model,
			fishPositions,
			glm::vec3(0.2f)
		);

		SceneObject* jellyfish = new SceneObject(
			life_jellyfish_model,
			{ glm::vec3(-10.0f, -3.0f, -15.0f), glm::vec3(-8.0f, 0.0f, -10.0f)},
			glm::vec3(0.4f)
		);

		SceneObject* stingray = new SceneObject(
			stingrayModel,
			{ glm::vec3(0.0f, 0.0f, 2.0f) },
			glm::vec3(0.2f)
		);
		underwaterDesert = new Level(shader,camera, SCRN_W, SCRN_H);
		std::cout << "Adding objects" << std::endl;
		underwaterDesert->addObject(coralPinkObjects);
		underwaterDesert->addObject(coralGonObjects);
		underwaterDesert->addObject(coralPocilObjects);
		underwaterDesert->addObject(coralStoneObjects);
		underwaterDesert->addObject(fishSchool);
		underwaterDesert->addObject(jellyfish);		
		underwaterDesert->addObject(stingray);

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
	glutIdleFunc(display);
		

	// 6. Start the main loop
	glutMainLoop();
	return 0;
	

}

