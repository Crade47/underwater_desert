#pragma once
#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <vector>
#include "stb_image.h"
#include "iostream"
#include <random>
class Object
{
public:
	GLuint VAO, EBO, texture_one, texture_two;
	GLuint VBO[3];
	Object(
		const std::vector<glm::vec3>& vertices, 
		const std::vector<glm::vec4>& colors, 
		const std::vector<GLuint>& indices,
		
		const std::string& texFilePath_one,
		const std::string& texFilePath_two,
		const std::vector<glm::vec2>&texCoor
	);
	static std::vector<glm::vec3> generateTerrain(int gridSize, float size);
	static std::vector<GLuint> generateTerrainIndices(int gridSize);
	static std::vector<glm::vec2> generateTerrainTexCoords(int gridSize);

	void draw() const;
	~Object();
private:
	std::vector<GLuint> indices_arr;
};

