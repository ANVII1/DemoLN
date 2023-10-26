#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

using namespace std;

void draw() 
{
	GLfloat verteces[] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};
	glColor3f(1,1,1);
	glVertexPointer(3, GL_FLOAT, 0, verteces);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
}

int main()
{
	if (!glfwInit())
		return -1;
	GLFWwindow* window = glfwCreateWindow(500, 500, "Game", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (!window)
		glfwTerminate();

	if (!gladLoadGL())
		return -1;

	//glEnable(GL_DEPTH_TEST);
	//glFrustum(1,1,2,2,-1,1);
	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};
	/*
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	*/
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glPushMatrix();
			draw();
		glPopMatrix();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}
