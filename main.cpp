#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "cell.h"

#include <iostream>
#include <cstdlib>

// settings variables
const int WIDTH = 1000;
const int HEIGHT = 1000;

const float FHEIGHT = (float)HEIGHT;
const float FWIDTH = (float)WIDTH;
const  int CELLCOUNT = (HEIGHT / 2) * (WIDTH / 2);

// Prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


CELL Cells[CELLCOUNT];

/* Array SCREEN_POS has all creatures positions
*  next pos will be +/- 1 or 0 for X and Y
*  pos conversion for index will be: ((WIDTH * Y) + X)
*  next pos conversion for index will be: ((WIDTH * (Y + y_change)) + (X + x_change))
*  check next pos for out of bounds and type
*  convert index to screen coords: (X * (2.0f / FWIDTH)) - 1, (Y * (2.0f / FHEIGHT)) - 1
*/


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Predators and Prey", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 2 x 2 Pixel Quad located bottom left
	float vertices[] =
	{
		-1.0f, -1.0f + (4.0f / FHEIGHT), 0,
		-1.0f + (4.0f / FWIDTH), -1.0f + (4.0f / FHEIGHT), 0,
		-1.0f + (4.0f / FWIDTH), -1.0f, 0,

		-1.0f, -1.0f + (4.0f / FHEIGHT), 0,
		-1.0f , -1.0f, 0,
		-1.0f + (4.0f / FWIDTH), -1.0f, 0,
	};

	Shader shader("Shaders/VertexShader.txt", "Shaders/CellShader.txt");

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// Set all cells to dead
	for (int i = 0; i < CELLCOUNT; i++)
	{
		Cells[i].type = DEAD;
	}


	glm::vec3 transform;

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		Sleep(100);

		// Clear Screen
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		glBindVertexArray(VAO);

		for (int i = 0; i < CELLCOUNT; i++)
		{
			// Only want to bother with living cells
			if (Cells[i].type != DEAD)
			{
				// Draw then updated to avoid drawing dead cell left behind from movement
				transform = glm::vec3((Cells[i].x * (4.0f / FWIDTH)), (Cells[i].y * (4.0f / FHEIGHT)), 0.0f);
				shader.setVec3("transform", transform);
				shader.setInt("type", Cells[i].type);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				cell::update(Cells[i]);
			}
		}

		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Process key input
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		cell::newPrey(WIDTH / 4, HEIGHT / 4);

}
