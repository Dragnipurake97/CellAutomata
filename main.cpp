#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

#include <iostream>
#include <cstdlib>

// settings variables
const unsigned int WIDTH = 700;
const unsigned int HEIGHT = 700;

const float FHEIGHT = (float)HEIGHT;
const float FWIDTH = (float)WIDTH;
const int CELLCOUNT = (HEIGHT / 2) * (WIDTH / 2);

// Cell Data Structures
enum cell_type
{
	PREDATOR = 0,
	PREY = 1,
	DEAD = 2
};

typedef struct cell
{
	cell_type type;
	int health;
	int x;
	int y;
	int index = ((WIDTH / 2) * y) + x;
} CELL;

// Prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void update(CELL &cell);
int getIndex(int x, int y);
void generatePrey(int x, int y);
void newPrey(int x, int y);
void move(CELL &cell, int x, int y);


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

		//Sleep(1000);

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

				update(Cells[i]);
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
		newPrey(WIDTH / 4, HEIGHT / 4);

}

// Update a cell ready to draw
void update(CELL &cell)
{
	int x, y, index;

	// Calcualte new location
	x = (std::rand() % 3) - 1; // Minus 1 after to allow for negative values
	y = (std::rand() % 3) - 1;

	index = getIndex(cell.x + x, cell.y + y);


	// Manage health
	if (cell.type == PREDATOR)
	{
		cell.health -= 1;
		if (cell.health == 0)
			cell.type = DEAD;
	}
	else if(cell.type == PREY)
	{
		cell.health += 1;
		if (cell.health > 5)
		{
			generatePrey(cell.x, cell.y);
			cell.health = 1;
		}
	}

	move(cell, cell.x + x, cell.y + y);

}

// Gets index of array from x,y; returns -1 if out of bounds
int getIndex(int x, int y)
{
	// Check bounds
	if (x < 0 || x > (WIDTH / 2) || y < 0 || y > (HEIGHT / 2))
	{
		return -1;
	}

	return ((WIDTH / 2) * y) + x;
}

// Generates a PREY from a PREY at x, y
void generatePrey(int x, int y)
{
	// Check that the coords are valid
	// then check adjacent spots and create new if room

	int spawn = (std::rand() % 4);

	if (x + 1 < (WIDTH / 2) && spawn == 0)
	{
		if (Cells[getIndex(x + 1, y)].type == DEAD)
		{
			newPrey(x + 1, y);
			return;
		}
	}
	else if (x - 1 > 0 && spawn == 1)
	{
		if (Cells[getIndex(x - 1, y)].type == DEAD)
		{
			newPrey(x - 1, y);
			return;
		}
	}
	else if (y + 1 < (HEIGHT / 2) && spawn == 2)
	{
		if (Cells[getIndex(x, y + 1)].type == DEAD)
		{
			newPrey(x, y + 1);
			return;
		}
	}
	else if (y - 1 > 0 && spawn == 3)
	{
		if (Cells[getIndex(x, y - 1)].type == DEAD)
		{
			newPrey(x, y - 1);
			return;
		}
	}

}

// Generates a new PREY at x, y
void newPrey(int x, int y)
{
	// If out of bounds then return
	if (x > WIDTH / 2 || x < 0 || y > HEIGHT / 2 || y < 0)
	{
		return;
	}

	int index = getIndex(x, y);

	// Check that it can be made
	if (Cells[index].type == PREDATOR || Cells[index].type == PREY)
	{
		return;
	}

	Cells[index].type = PREY;
	Cells[index].health = 1;
	Cells[index].x = x;
	Cells[index].y = y;
}

void move(CELL &cell, int x, int y)
{
	// If out of bounds then return
	if (x > WIDTH / 2 || x < 0 || y > HEIGHT / 2 || y < 0)
	{
		return;
	}

	int index = getIndex(x, y);

	// Cannot move onto another cell
	if (Cells[index].type != PREY)
	{
		cell.type = DEAD;
		cell.x = x;
		cell.y = y;
		Cells[index] = cell;
		Cells[index].type = PREY;
	}
}
