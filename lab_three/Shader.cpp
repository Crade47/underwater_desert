#include "Shader.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{

	shaderProgramID = glCreateProgram();

	std::string vertexShaderC = LoadShaderFromFile(vertexPath);
	std::string fragShaderC = LoadShaderFromFile(fragmentPath);
	//compile shaders
	vertex = compileShader(GL_VERTEX_SHADER, vertexShaderC.c_str());
	fragment = compileShader(GL_FRAGMENT_SHADER, fragShaderC.c_str());
	//attach
	glAttachShader(shaderProgramID, vertex);
	glAttachShader(shaderProgramID, fragment);
	//link
	glLinkProgram(shaderProgramID);
	checkCompileErrors(shaderProgramID, "PROGRAM");


}


void Shader::use() const
{
	glUseProgram(shaderProgramID);
}

Shader::~Shader()
{
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::setUniform(const std::string& name, float value) const
{
	GLint location = glGetUniformLocation(shaderProgramID, name.c_str());
	if (location != -1) {
		glUniform1f(location, value);
	}
	else {
		std::cerr << "location not found for" << name << std::endl;
	}
}
void Shader::setUniform(const std::string& name, int value) const
{
	GLint location = glGetUniformLocation(shaderProgramID, name.c_str());
	if (location != -1) {
		glUniform1i(location, value);
	}
	else {
		std::cerr << "location not found for" << name << std::endl;
	}
}
void Shader::setUniform(const std::string& name, const glm::vec4& value) const
{
	GLint location = glGetUniformLocation(shaderProgramID, name.c_str());
	if (location != -1) {
		glUniform4fv(location, 1, &value[0]);
	}
	else {
		std::cerr << "location not found for" << name << std::endl;
	}
}
void Shader::setUniform(const std::string& name, const glm::vec3& value) const
{
	GLint location = glGetUniformLocation(shaderProgramID, name.c_str());
	if (location != -1) {
		glUniform4fv(location, 1, &value[0]);
	}
	else {
		std::cerr << "location not found for" << name << std::endl;
	}
}

void Shader::setUniform(const std::string& name, const glm::mat4& matrix) const
{
	GLint location = glGetUniformLocation(shaderProgramID, name.c_str());
	if (location != -1) {
		glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
	}
	else {
		std::cerr << "location not found for" << name << std::endl;
	}
}


GLuint Shader::compileShader(GLenum type, const char* source)
{
	//Create Shader
	GLuint shader = glCreateShader(type);
	//Encode
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);
	checkCompileErrors(shader, type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
	return shader;
}

void Shader::checkCompileErrors(GLuint component, const std::string type) const
{
	GLint success;

	if (type != "PROGRAM") {
		glGetShaderiv(component, GL_COMPILE_STATUS, &success);
		if (!success) {
			GLchar InfoLog[1024];
			glGetShaderInfoLog(component, 1024, NULL, InfoLog);
			std::cerr << "Error creating shader type: " << type << InfoLog << std::endl;
			//fprintf(stderr, "Error compiling shader type VERTEX: %s\n", InfoLog);
			exit(1);
		}
		else {
			std::cout << "Successfully created shader type: " << type << std::endl;
		}
	}
	else {
		glGetProgramiv(component, GL_LINK_STATUS, &success);
		if (!success) {
			std::cerr << "Error linking program type: " << type << std::endl;
		}
		else {
			std::cout << "Successfully linked program type: " << type << std::endl;
		}
	}
}

std::string Shader::LoadShaderFromFile(const std::string& filepath)
{
	std::ifstream shaderFile(filepath);
	if (!shaderFile.is_open()) {
		std::cerr << "Failed to open shader file: " << filepath << std::endl;
		return "";
	}
	std::string shaderCode((std::istreambuf_iterator<char>(shaderFile)),
		std::istreambuf_iterator<char>());
	shaderFile.close();

	return shaderCode;
}
