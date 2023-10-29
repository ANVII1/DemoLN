#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include "Window/window.hpp"
#include <stdio.h>
#include <iostream>

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <tuple>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>

// Default camera values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat ZOOM = 45.0f;

class Camera
{
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Eular Angles
	GLfloat Yaw;
	GLfloat Pitch;
	// Camera options
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;

	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		this->Position = position;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
	}
	// Constructor with scalar values
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		this->Position = glm::vec3(posX, posY, posZ);
		this->WorldUp = glm::vec3(upX, upY, upZ);
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
	}

	void move(glm::vec3 newPosition)
	{
		Position = newPosition;
	}
private:
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		front.y = sin(glm::radians(this->Pitch));
		front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		this->Front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		this->Up = glm::normalize(glm::cross(this->Right, this->Front));
	}
};

struct shaderConstructData
{
	const char* filePath;
	GLuint type;
};

class Shader
{
private:
	std::string readFile(const char* FilePath)
	{
		// ---------- Загрузка кода шейдера
		std::string sourceString;
		std::ifstream sourceFile;

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
		return sourceString;
	}
	GLuint CreateShader(const char* FilePath, GLuint type)
	{
		std::string stringCode = readFile(FilePath);
		char* code = const_cast<char*>(stringCode.c_str());

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
			std::cout << "Ошибка комплиции шейдера:" << std::endl
				<< log << std::endl
				<< "Код шейдера:" << std::endl
				<< code << std::endl;
		}

		return shader;
	}
public:
	GLuint program;
	Shader(std::vector<shaderConstructData> PathesAndTypes)
	{
		std::vector<GLuint> shaders;

		for (int i = 0; i < PathesAndTypes.size(); ++i) {
			shaders.push_back(CreateShader(PathesAndTypes[i].filePath, PathesAndTypes[i].type));
		}

		GLint ok;
		GLchar log[2000];
		// ---------- Сборка программы
		program = glCreateProgram();
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
	}
	void setMat4(glm::mat4 value, const char* varName)
	{
		GLint Location = glGetUniformLocation(program, varName);
		glUniformMatrix4fv(Location, 1, GL_FALSE, glm::value_ptr(value));
	}
	void use()
	{
		glUseProgram(program);
	}
};

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

	std::vector<GLfloat> vertices = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.5f,  0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f
	};

	std::vector<GLfloat> color = { 0.7f,0,0.9f, 0,1,0, 0,0,1, 0.3f,0,0.5f };
	
	GLuint vetexVBO, colorVBO;

	glGenBuffers(1, &vetexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vetexVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &colorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(GLfloat), color.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Kamera Huyamera
	Camera camera(glm::vec3(3.0f, 0.0f, 3.0f));

	Shader ourShader({ {"res/shaders/VertexShader.glsl", GL_VERTEX_SHADER}, {"res/shaders/ColorShader.glsl", GL_FRAGMENT_SHADER} });
	ourShader.use();

	while (!CORE::Window::isShouldClose())
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLfloat radius = 10.0f;
		GLfloat camX = sin(glfwGetTime()) * radius;
		GLfloat camZ = cos(glfwGetTime()) * radius;
		camera.move(glm::vec3(camX, 0.0, camZ));

		// Create camera transformation
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;

		model = glm::rotate(glm::mat4(1.0f),(float)glfwGetTime(),glm::vec3(5.0f,0.0f,1.0f));
		view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));// camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(45.0f), (float)500 / (float)500, 0.1f, 1000.0f);

		ourShader.setMat4(view, "view");
		ourShader.setMat4(projection, "projection");
		ourShader.setMat4(model, "model"); ////////////-----------------------

		CORE::Window::pollEvents();
		glPushMatrix();
		draw(vetexVBO, colorVBO);
		glPopMatrix();
		CORE::Window::swapBuffers();
	}

	glDeleteBuffers(1, &vetexVBO);
	glDeleteBuffers(1, &colorVBO);
	CORE::Window::final();
	return 0;
}

#endif