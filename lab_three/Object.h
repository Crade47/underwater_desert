#pragma once
#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <vector>
#include "stb_image.h"
#include "iostream"
class Object
{
public:
	GLuint VAO, EBO, texture;
	GLuint VBO[3];
	Object(
		const std::vector<glm::vec3>& vertices, 
		const std::vector<glm::vec4>& colors, 
		const std::vector<GLuint>& indices,
		const std::string& texFilePath,
		const std::vector<glm::vec2>&texCoor
	);
	void draw() const;
	~Object();
private:
	std::vector<GLuint> indices_arr;
};

