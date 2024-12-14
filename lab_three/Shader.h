#pragma once
#include "GL/glew.h"
#include <string>
#include "glm/glm.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
class Shader
{
public:
	GLuint shaderProgramID;

	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	void use() const;
	void setUniform(const std::string& name, float value) const;
	void setUniform(const std::string& name, int value) const;
	void setUniform(const std::string& name, const glm::vec3& value) const;
	void setUniform(const std::string& name, const glm::vec4& value) const;
	void setUniform(const std::string& name, const glm::mat4& matrix) const;
	void setUniformTexture(const std::string& name, int textureUnit) const;
	~Shader();
	bool isValid() const { return shaderProgramID != 0; }
private:
	//void setUniform(const std::string& name, const glm::mat4& matrix) const;
	GLuint compileShader(GLenum type, const char* source);
	void checkCompileErrors(GLuint shader, const std::string type) const;
	std::string LoadShaderFromFile(const std::string& filepath);
	std::string shaderSource;
	GLuint vertex;
	GLuint fragment;
};

