#include "cell.h"

#include <cstdlib>

namespace cell
{

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
		else if (cell.type == PREY)
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
		if (x < 0 || x > (WIDTH / 2) || y < 0 || y >(HEIGHT / 2))
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

}