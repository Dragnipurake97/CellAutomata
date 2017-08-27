#include "cell.h"

#include <cstdlib>
#include <iostream>

namespace cell
{

	// Update a cell ready to draw
	void update(CELL &cell)
	{
		if (cell.isNew)
		{
			cell.isNew = false;
			return;
		}

		int x, y, index;

		// Calculate new location
		x = (std::rand() % 3) - 1; // Minus 1 after to allow for negative values
		y = (std::rand() % 3) - 1;

		index = getIndex(cell.x + x, cell.y + y);


		// Manage health
		if (cell.type == PREDATOR)
		{
			// Check if prey can be eaten
			consumePrey(cell);

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
		int check = std::rand() % 4; // Choses a random cell to check first
		bool hasBred = false;
		int adjChecked = 0;

		// While prey hasn't eaten or all cells checked
		while (!hasBred && adjChecked < 5)
		{
			switch (check)
			{
			case(0):
				if (Cells[getIndex(x + 1, y)].type == DEAD && x + 1 < (WIDTH / 2))
				{
					newPrey(x + 1, y);
					hasBred = true;
					return;
				}
				adjChecked++;
				check = 1; // Assign check to next if statement so it checks there next
				break;
			case(1):
				if (Cells[getIndex(x - 1, y)].type == DEAD && x - 1 > 0)
				{
					newPrey(x - 1, y);
					hasBred = true;
					return;
				}
				adjChecked++;
				check = 2;
				break;
			case(2):
				if (Cells[getIndex(x, y + 1)].type == DEAD && y + 1 < (HEIGHT / 2))
				{
					newPrey(x, y + 1);
					hasBred = true;
					return;
				}
				adjChecked++;
				check = 3;
				break;
			case(3):
				if (Cells[getIndex(x, y - 1)].type == DEAD && y - 1 > 0)
				{
					newPrey(x, y - 1);
					hasBred = true;
					return;
				}
				adjChecked++;
				check = 0;
				break;
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
		Cells[index].isNew = true;
	}

	void move(CELL &cell, int x, int y)
	{
		// If out of bounds then return
		if (x > WIDTH / 2 || x < 0 || y > HEIGHT / 2 || y < 0)
		{
			return;
		}

		int index = getIndex(x, y);

		if (Cells[index].type == DEAD)
		{
			Cells[index].type = cell.type;
			Cells[index].health = cell.health;
			Cells[index].x = x;
			Cells[index].y = y;
			cell.type = DEAD;
			cell.health = 0;
		}
	}

	void overwriteCell(int x, int y, cell_type type)
	{
		int index = getIndex(x, y);
		Cells[index].type = type;
		Cells[index].x = x;
		Cells[index].y = y;
		Cells[index].health = 2;
		Cells[index].isNew = true;
	}

	void consumePrey(CELL &cell)
	{
		int check = std::rand() % 8; // Choses a random cell to check first
		bool hasEaten = false;
		int adjChecked = 0;

		// While prey hasn't eaten or all cells checked
		while (!hasEaten && adjChecked < 8)
		{
			switch (check)
			{
			case(0):
				if (Cells[getIndex(cell.x + 1, cell.y)].type == PREY && cell.x + 1 < (WIDTH / 2))
				{
					overwriteCell(cell.x + 1, cell.y, PREDATOR);
					cell.health++;
					hasEaten = true;
					return;
				}
				adjChecked++;
				check = 1; // Assign check to next if statement so it checks there next
				break;
			case(1):
				if (Cells[getIndex(cell.x - 1, cell.y)].type == PREY && cell.x - 1 > 0)
				{
					overwriteCell(cell.x - 1, cell.y, PREDATOR);
					cell.health++;
					hasEaten = true;
					return;
				}
				adjChecked++;
				check = 2;
				break;
			case(2):
				if (Cells[getIndex(cell.x, cell.y + 1)].type == PREY && cell.y + 1 < (HEIGHT / 2))
				{
					overwriteCell(cell.x, cell.y + 1, PREDATOR);
					cell.health++;
					hasEaten = true;
					return;
				}
				adjChecked++;
				check = 3;
				break;
			case(3):
				if (Cells[getIndex(cell.x, cell.y - 1)].type == PREY && cell.y - 1 > 0)
				{
					overwriteCell(cell.x, cell.y - 1, PREDATOR);
					cell.health++;
					hasEaten = true;
					return;
				}
				adjChecked++;
				check = 4;
				break;
			case(4):
				if (Cells[getIndex(cell.x + 1, cell.y + 1)].type == PREY && cell.x + 1 < (WIDTH / 2) && cell.y + 1 < (HEIGHT / 2))
				{
					overwriteCell(cell.x + 1, cell.y + 1, PREDATOR);
					cell.health++;
					hasEaten = true;
					return;
				}
				adjChecked++;
				check = 5;
				break;
			case(5):
				if (Cells[getIndex(cell.x + 1, cell.y - 1)].type == PREY && cell.x + 1 < (WIDTH / 2) && cell.y - 1 > 0)
				{
					overwriteCell(cell.x + 1, cell.y - 1, PREDATOR);
					cell.health++;
					hasEaten = true;
					return;
				}
				adjChecked++;
				check = 6;
				break;
			case(6):
				if (Cells[getIndex(cell.x - 1, cell.y - 1)].type == PREY && cell.x - 1 > 0 && cell.y - 1 > 0)
				{
					overwriteCell(cell.x - 1, cell.y - 1, PREDATOR);
					cell.health++;
					hasEaten = true;
					return;
				}
				adjChecked++;
				check = 7;
				break;
			case(7):
				if (Cells[getIndex(cell.x - 1, cell.y + 1)].type == PREY && cell.x - 1 > 0 && cell.y + 1 < (HEIGHT / 2))
				{
					overwriteCell(cell.x - 1, cell.y + 1, PREDATOR);
					cell.health++;
					hasEaten = true;
					return;
				}
				adjChecked++;
				check = 0;
				break;
			}

		}

	}
}