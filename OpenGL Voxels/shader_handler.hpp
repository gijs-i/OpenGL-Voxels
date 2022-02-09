#ifndef SHADER_HANDLER
#define SHADER_HANDLER

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

std::string readFile(std::string fileSource) {
	std::string source;
	std::ifstream file(fileSource);
	if (file.is_open()) {
		std::stringstream sstream;
		std::string line;
		while (std::getline(file,line)) {
			sstream << line << std::endl;
		}
		sstream << '\0';
		source = sstream.str();
		return source;
	}
	else {
		std::cout << "[File] " << fileSource << " couldn't be opened!" << std::endl;
		return "";
	}
}

unsigned int compileShader(GLenum shaderType, const char* shaderSource) {
	unsigned int shader;
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		const char* shaderTypeString;
		shaderTypeString = shaderType == GL_VERTEX_SHADER ? "Vertex" : "Fragment";
		std::cout << "[OpenGL] " << shaderTypeString << " shader compilation failed: \n----------\n" << infoLog << std::endl;
	}
	return shader;
}

unsigned int createShaderProgram(std::string vertexSource, std::string fragmentSource) {
	//Vertex shader source code
	std::string vertexShaderSource = readFile(vertexSource);

	//Fragment shader source code
	std::string fragmentShaderSource = readFile(fragmentSource);

	//Create vertex shader
	unsigned int vertexShader;
	vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource.c_str());


	//Create fragment shader
	unsigned int fragmentShader;
	fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource.c_str());


	//Create final shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgram);

	return shaderProgram;
}

void tekenOlavBal() {
	std::cout << "O==================D" << std::endl;
}

#endif