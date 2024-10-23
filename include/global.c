#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "global.h"

void grid_build(Grid *grid)
{
    for (int row = 0; row < SCREEN_HEIGHT; row += CELL_SIZE)
    {
        for (int col = 0; col < SCREEN_WIDTH; col += CELL_SIZE)
        {
            grid->arr[grid->size] = (Cell){.x = col, .y = row};
            grid->size++;
        }
    }
}

/// returns index
int data_get_wall(Data *data, Position *pos)
{
    for (int i = 0; i < data->walls_size; ++i)
    {
        if (data->walls[i].row == pos->row &&
            data->walls[i].column == pos->column)
        {
            return i;
        }
    }

    return -1;
}

/* Data format
p row|column
w [(row|column)(row|column)(row|column)]
*/

void data_load(Data *data, char *file_name)
{
    printf("Loading file '%s'\n", file_name);
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        if (errno == ENOENT)
        {
            fprintf(
                stderr,
                "ERROR: Failed to open file '%s': No such file or directory\n",
                file_name
            );
        }
        else
        {
            fprintf(stderr, "ERROR: Failed to open file '%s'\n", file_name);
        }

        return;
    }

    data->walls_size = 0;

    while ((read = getline(&line, &len, fp)) != -1)
    {
        if (line[0] == 'p')
        {
            int row, column;
            sscanf(line, "p %d|%d", &row, &column);
            data->player_starting_position.row    = row;
            data->player_starting_position.column = column;
        }
        else if (line[0] == 'w')
        {
            int curr = 2;
            while (line[curr] != '\0')
            {
                if (line[curr] == '(')
                {
                    int row, column;

                    sscanf(&line[curr], "(%d|%d)", &row, &column);
                    data->walls[data->walls_size].row    = row;
                    data->walls[data->walls_size].column = column;
                    data->walls_size++;
                    while (line[curr] != ')' && line[curr] != '\0')
                        curr++;
                }
                curr++;
            }
        }
    }

    fclose(fp);
    if (line)
        free(line);
}

void data_save(Data *data, char *file_name)
{
    if (data->player_starting_position.row == -1 ||
        data->player_starting_position.column == -1)
    {
        printf("ERROR: Player starting position is -1\n");
        printf("Set player starting position before saving\n");
        return;
    }

    printf("file name: %s\n", file_name);
    FILE *fptr = fopen(file_name, "w");
    if (fptr == NULL)
    {
        printf("ERROR: Failed to open file '%s': %i\n", file_name, errno);
        return;
    }

    fprintf(
        fptr,
        "p %i|%i\n",
        data->player_starting_position.row,
        data->player_starting_position.column
    );

    fprintf(fptr, "w ");
    fprintf(fptr, "[");
    for (int i = 0; i < data->walls_size; ++i)
    {
        Position pos = data->walls[i];

        fprintf(fptr, "(%i|%i)", pos.row, pos.column);
    }
    fprintf(fptr, "]\n");

    fclose(fptr);
}
