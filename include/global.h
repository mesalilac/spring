#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdbool.h>

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 800

#define CELL_SIZE   20
#define MAX_CELLS   ((SCREEN_WIDTH / CELL_SIZE) * (SCREEN_HEIGHT / CELL_SIZE))
#define MAX_ROWS    SCREEN_HEIGHT / CELL_SIZE - 1
#define MAX_COLUMNS SCREEN_WIDTH / CELL_SIZE - 1

#define BACKGROUND_COLOR 28, 28, 28, 255
#define GRID_COLOR       32, 32, 32, 255

typedef struct
{
    int x, y;
} Cell;

typedef struct
{
    Cell arr[MAX_CELLS];
    int size;
} Grid;

typedef struct
{
    int row;
    int column;
} Position;

typedef struct
{
    Position walls[MAX_CELLS];
    int walls_size;
    Position player_starting_position;
} Data;

void grid_build(Grid *grid);
int data_get_wall(Data *data, Position *pos);
void data_load(Data *data, char *file_name);
void data_save(Data *data, char *file_name);

#endif // GLOBAL_H
