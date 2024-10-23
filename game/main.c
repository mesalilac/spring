#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/global.h"

// TODO: Load data file
// TODO: Move player in map

typedef struct
{
    Position pos;
} Player;

int main(int argc, char **argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(
            stderr, "ERROR: Failed to initalize video: %s\n", SDL_GetError()
        );
        exit(1);
    }

    SDL_Window *win = SDL_CreateWindow(
        "Game", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN

    );
    if (win == NULL)
    {
        fprintf(stderr, "ERROR: Failed to create window: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (ren == NULL)
    {
        fprintf(
            stderr, "ERROR: Failed to create renderer: %s\n", SDL_GetError()
        );
        exit(1);
    }

    if (TTF_Init() == -1)
    {
        fprintf(stderr, "ERROR: Failed to initalize: %s\n", SDL_GetError());
        exit(1);
    }

    TTF_Font *font = TTF_OpenFont("yudit.ttf", 14);
    if (font == NULL)
    {
        fprintf(stderr, "ERROR: Failed to open font: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Event event;
    bool quit = false;
    Grid grid = {.size = 0};
    grid_build(&grid);

    Data data = {
        .walls_size               = 0,
        .player_starting_position = {-1, -1},
    };

    for (int i = 1; i < argc; ++i)
    {
        data_load(&data, argv[i]);
        break;
    }

    Player player = {
        .pos =
            {.row    = data.player_starting_position.row,
                  .column = data.player_starting_position.column}
    };

    SDL_Rect input_rect = {
        .x = SCREEN_WIDTH / 2 - 180 / 2,
        .y = SCREEN_HEIGHT / 2 - 30 / 2,
        .w = 180,
        .h = 30
    };

    bool loading        = false;
    char file_name[100] = "level-";
    int file_name_size  = strlen(file_name);

    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    if (loading == false)
                    {
                        if (event.key.keysym.sym == 'l')
                        {
                            file_name[0]   = 'l';
                            file_name[1]   = 'e';
                            file_name[2]   = 'v';
                            file_name[3]   = 'e';
                            file_name[4]   = 'l';
                            file_name[5]   = '-';
                            file_name[6]   = '\0';
                            file_name_size = strlen(file_name);
                            loading        = true;
                        }
                        // TODO: check if new pos isn't already occupied by a
                        // wall
                        if (event.key.keysym.sym == 'w')
                        {
                            Position new_pos = {
                                .row    = player.pos.row - 1,
                                .column = player.pos.column
                            };

                            int wall = data_get_wall(&data, &new_pos);

                            if (wall == -1)
                                player.pos.row--;
                        }
                        if (event.key.keysym.sym == 'a')
                        {
                            Position new_pos = {
                                .row    = player.pos.row,
                                .column = player.pos.column - 1
                            };

                            int wall = data_get_wall(&data, &new_pos);

                            if (wall == -1)
                                player.pos.column--;
                        }
                        if (event.key.keysym.sym == 's')
                        {
                            Position new_pos = {
                                .row    = player.pos.row + 1,
                                .column = player.pos.column
                            };

                            int wall = data_get_wall(&data, &new_pos);

                            if (wall == -1)
                                player.pos.row++;
                        }
                        if (event.key.keysym.sym == 'd')
                        {
                            Position new_pos = {
                                .row    = player.pos.row,
                                .column = player.pos.column + 1
                            };

                            int wall = data_get_wall(&data, &new_pos);

                            if (wall == -1)
                                player.pos.column++;
                        }
                        if (event.key.keysym.sym == SDLK_DELETE)
                        {
                            data.walls_size                      = 0;
                            data.player_starting_position.row    = -1;
                            data.player_starting_position.column = -1;
                        }
                    }
                    else
                    {
                        if (event.key.keysym.sym == SDLK_RETURN)
                        {
                            int old_size = file_name_size;

                            file_name[file_name_size] = '.';
                            file_name_size++;
                            file_name[file_name_size] = 'd';
                            file_name_size++;
                            file_name[file_name_size] = 'a';
                            file_name_size++;
                            file_name[file_name_size] = 't';
                            file_name_size++;
                            file_name[file_name_size] = 'a';
                            file_name_size++;
                            file_name[file_name_size] = '\0';
                            if (loading)
                            {
                                data_load(&data, file_name);
                                loading = false;
                                player.pos.row =
                                    data.player_starting_position.row;
                                player.pos.column =
                                    data.player_starting_position.column;
                            }
                            // Remove the extention from file name
                            file_name[old_size] = '\0';
                            file_name_size      = strlen(file_name);
                        }
                        else if (event.key.keysym.sym == SDLK_DELETE)
                        {
                            file_name[0]   = 'l';
                            file_name[1]   = 'e';
                            file_name[2]   = 'v';
                            file_name[3]   = 'e';
                            file_name[4]   = 'l';
                            file_name[5]   = '-';
                            file_name[6]   = '\0';
                            file_name_size = strlen(file_name);
                        }
                        else if (event.key.keysym.sym == SDLK_BACKSPACE)
                        {
                            file_name_size--;
                            file_name[file_name_size] = '\0';
                        }
                        else
                        {
                            file_name[file_name_size] = event.key.keysym.sym;
                            file_name_size++;
                            file_name[file_name_size] = '\0';
                        }
                    }
                    break;
            }
        }

        SDL_SetRenderDrawColor(ren, BACKGROUND_COLOR);
        SDL_RenderClear(ren);

        for (int i = 0; i < grid.size; ++i)
        {
            Cell cell     = grid.arr[i];
            SDL_Rect rect = {
                .x = cell.x, .y = cell.y, .w = CELL_SIZE, .h = CELL_SIZE
            };

            Position pos = {
                .row    = cell.y / CELL_SIZE,
                .column = cell.x / CELL_SIZE,
            };

            if (data_get_wall(&data, &pos) != -1)
            {
                SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
                SDL_RenderFillRect(ren, &rect);
            }
            else if (player.pos.row == pos.row &&
                     player.pos.column == pos.column)
            {
                SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
                SDL_RenderFillRect(ren, &rect);
            }
            else
            {
                SDL_SetRenderDrawColor(ren, GRID_COLOR);
                SDL_RenderDrawRect(ren, &rect);
            }
        }

        if (loading == true)
        {
            SDL_SetRenderDrawColor(ren, 80, 80, 80, 255);
            SDL_RenderFillRect(ren, &input_rect);

            SDL_Color color = {255, 255, 255, 255};
            SDL_Surface *surface =
                TTF_RenderText_Blended(font, file_name, color);
            SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surface);

            SDL_Rect text_rect = {
                .x = input_rect.x + 5,
                .y = input_rect.y + 5,
                .w = surface->w,
                .h = surface->h,
            };

            SDL_SetRenderDrawColor(ren, 80, 80, 80, 255);
            SDL_RenderDrawRect(ren, &text_rect);
            SDL_RenderCopy(ren, texture, NULL, &text_rect);

            free(surface);
            SDL_DestroyTexture(texture);
        }

        SDL_RenderPresent(ren);
    }

    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(ren);
    SDL_Quit();
}
