
// Cell Data Structures
enum cell_type
{
	PREDATOR = 0,
	PREY = 1,
	DEAD = 2
};

typedef struct cell_info
{
	cell_type type;
	int health;
	int x;
	int y;
} CELL;

extern const int WIDTH;
extern const int HEIGHT;
extern const int CELLCOUNT;

extern CELL Cells[];

namespace cell
{
	void update(CELL &cell);
	int getIndex(int x, int y);
	void generatePrey(int x, int y);
	void newPrey(int x, int y);
	void move(CELL &cell, int x, int y);
}