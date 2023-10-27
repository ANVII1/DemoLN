#include <iostream>
#include "Window/window.hpp"
#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <string>

GLuint getShader(const char* FilePath)
{
	// ---------- Загрузка кода шейдера
	std::string sourceString;
	std::ifstream sourceFile;
	//char* code = static_cast<char*>(malloc(sizeof(char)*2000));

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
	catch(std::ifstream::failure e) 
	{ 
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl; 
	}
	code = const_cast<char*>(sourceString.c_str());
	
	// ---------- Компиляция шейдера
	GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(shader,1,&code,NULL);
	glCompileShader(shader);

	/*
	glGetShaderiv(shader,GL_COMPILE_STATUS, &ok);
	if (!ok)
	{
		glGetShaderInfoLog(shader,2000,NULL,log);
		std::cout << log << std::endl;
	}
	*/

	// ---------- Сборка программы
	GLint ok;
	GLchar log[2000];

	GLuint program = glCreateProgram();
	glAttachShader(program,shader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_COMPILE_STATUS, &ok);
	if (!ok)
	{
		glGetProgramInfoLog(program, 2000, NULL, log);
		std::cout << log << std::endl;
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
		glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

int main()
{
	if (!CORE::Window::init(500, 500, "Game"))
		return 0;

	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};

	float color[] = { 0.7f,0,0.9f, 0,1,0, 0,0,1};
	
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
	
	GLuint prog = getShader("res/shaders/test.frag");
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
