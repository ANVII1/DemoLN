#include <iostream>
#include "Window/window.hpp"
#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <tuple>

struct shaderConstructData 
{
	const char* filePath;
	GLuint type;
};

GLuint CreateShader(const char* FilePath, GLuint type) 
{
	// ---------- Загрузка кода шейдера
	std::string sourceString;
	std::ifstream sourceFile;

	char* code;

	sourceFile.exceptions(std::ifstream::badbit);
	try {
		sourceFile.open(FilePath);

		if (!sourceFile.is_open())
		{
			std::cout << "Shader is not found" << std::endl;
		}

		std::stringstream sourceStream;
		sourceStream << sourceFile.rdbuf();
		sourceFile.close();
		sourceString = sourceStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	code = const_cast<char*>(sourceString.c_str());

	//std::cout << code << std::endl;
	
	// ---------- Компиляция шейдера
	GLint ok;
	GLchar log[2000];

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &code, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);

	// Проверка на правильность компиляции
	if (!ok) {
		glGetShaderInfoLog(shader, 2000, NULL, log);
		std::cout << "Ошибка комплиции шейдера:" << std::endl << log << std::endl;
	}

	return shader;
}

GLuint getShaderProgram(std::vector<shaderConstructData> PathesAndTypes)
{
	std::vector<GLuint> shaders;

	for (int i = 0; i < PathesAndTypes.size(); ++i) {
		shaders.push_back(CreateShader(PathesAndTypes[i].filePath, PathesAndTypes[i].type));
	}

	GLint ok;
	GLchar log[2000];
	// ---------- Сборка программы
	GLuint program = glCreateProgram();
	for (int i = 0; i < shaders.size(); ++i) {
		glAttachShader(program, shaders[i]);
	}
	
	glLinkProgram(program);
	glGetProgramiv(program, GL_COMPILE_STATUS, &ok);

	// Проверка на правильность компиляции
	if (!ok) {
		glGetProgramInfoLog(program, 2000, NULL, log);
		std::cout << "Ошибка комплиции шейдерной программы:" << std::endl << log << std::endl;
	}

	return program;
}


void draw(GLuint vertexVBO, GLuint colorVBO)
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
		glColorPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

int main()
{
	setlocale(LC_ALL, "Russian");
	if (!CORE::Window::init(500, 500, "Game"))
		return 0;

	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.5f,  0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f
	};

	float color[] = { 0.7f,0,0.9f, 0,1,0, 0,0,1, 1,0,0};
	
	GLuint vertexVBO;
	GLuint colorVBO;

	glGenBuffers(1, &vertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &colorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	GLuint prog = getShaderProgram({ {"res/shaders/VertexShader.glsl", GL_VERTEX_SHADER} , {"res/shaders/ColorShader.glsl", GL_FRAGMENT_SHADER} });
	glUseProgram(prog);
	

	while (!CORE::Window::isShouldClose())
	{
		CORE::Window::pollEvents();
		glPushMatrix();
			draw(vertexVBO,colorVBO);
		glPopMatrix();
		CORE::Window::swapBuffers();
	}

	glDeleteBuffers(1,&vertexVBO);
	glDeleteBuffers(1, &colorVBO);
	CORE::Window::final();
	return 0;
}
