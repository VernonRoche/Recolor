#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"

struct game_s
{
    color *cells_init;
    color *cells;
    uint nb_moves_max;
    uint nb_moves_cur;
    uint height;
    uint width;
    bool wrapping;
};

void errorExit(char * errorMessage)
{
    fprintf(stderr,"%s\n", errorMessage);
    exit(EXIT_FAILURE);
}

game game_new(color *cells, uint nb_moves_max)
{
    return game_new_ext(SIZE, SIZE, cells, nb_moves_max, false);
}

game game_new_empty()
{
    return game_new_empty_ext(SIZE, SIZE, false);
}

void game_set_cell_init(game g, uint x, uint y, color c)
{
    g->cells_init[y * game_width(g) + x] = c;
    g->cells[y * game_width(g) + x] = c;
}

void game_set_max_moves(game g, uint nb_max_moves)
{
    g->nb_moves_max = nb_max_moves;
}

uint game_nb_moves_max(cgame g)
{
    return g->nb_moves_max;
}

color game_cell_current_color(cgame g, uint x, uint y)
{
    return g->cells[y * game_width(g) + x];
}

uint game_nb_moves_cur(cgame g)
{
    return g->nb_moves_cur;
}

void game_set_cell_cur(game g, color c, uint x, uint y) // aux
{
    g->cells[y * game_width(g) + x] = c; // set the color of the cell on the x y coordinates
}

void game_play_one_move_aux(game g, color old_col, color new_col, uint x, uint y)
{
    bool isWrapping = game_is_wrapping(g);
    if (x > game_width(g) || y > game_height(g))
        return;
    if (game_cell_current_color(g, x, y) == old_col)
    {
        game_set_cell_cur(g, new_col, x, y);
        if (y != 0)
        {
            game_play_one_move_aux(g, old_col, new_col, x, y - 1);
        }
        else if (isWrapping)
        {
            game_play_one_move_aux(g, old_col, new_col, x, game_height(g) - 1);
        }
        if (y != (game_height(g) - 1))
        {
            game_play_one_move_aux(g, old_col, new_col, x, y + 1);
        }
        else if (isWrapping)
        {
            game_play_one_move_aux(g, old_col, new_col, x, 0);
        }
        if (x != 0)
        {
            game_play_one_move_aux(g, old_col, new_col, x - 1, y);
        }
        else if (isWrapping)
        {
            game_play_one_move_aux(g, old_col, new_col, game_width(g) - 1, y);
        }
        if (x != (game_width(g) - 1))
        {
            game_play_one_move_aux(g, old_col, new_col, x + 1, y);
        }
        else if (isWrapping)
        {
            game_play_one_move_aux(g, old_col, new_col, 0, y);
        }
    }
}

void game_play_one_move(game g, color c)
{
    g->nb_moves_cur += 1;
    color last_played = game_cell_current_color(g,0,0);
    if (last_played == c)
        return;
    game_play_one_move_aux(g, last_played, c, 0, 0);
}

game game_copy(cgame g)
{
    game new_game = game_new_ext(g->width, g->height, g->cells, g->nb_moves_max, g->wrapping);
    new_game->nb_moves_cur = g->nb_moves_cur;
    return new_game;
}

void game_delete(game g)
{
    if (g != NULL)
    {
        free(g->cells);
        free(g->cells_init);
        free(g);
    }
}

bool game_is_over(cgame g)
{
    if (game_nb_moves_max(g) < game_nb_moves_cur(g))
        return false;

    color last_played = game_cell_current_color(g,0,0);
    uint height = game_height(g);
    uint width = game_width(g);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (game_cell_current_color(g, x, y) != last_played)
                return false;
        }
    }
    return true;
}

void game_restart(game g)
{
    g->nb_moves_cur = 0;
    uint height = game_height(g);
    uint width = game_width(g);
    for (uint i = 0; i < height*width; i++)
    {
        g->cells[i] = g->cells_init[i];
    }
}

///////////////////////// V2 /////////////////////////

bool game_is_wrapping(cgame g)
{
    return g->wrapping;
}

game game_new_empty_ext(uint width, uint height, bool wrapping)
{

    color cells[width*height];
    for(int i = 0; i < width*height; i++)
    {
        cells[i] = 0;
    }

    game ngame = game_new_ext(width, height, cells, 0, wrapping);
    return ngame;
}

game game_new_ext(uint width, uint height, color *cells, uint nb_moves_max, bool wrapping)
{
    game ngame = malloc(sizeof(struct game_s));
    if (ngame == NULL)
    {
        errorExit("Allocation problem");
    }
    ngame->cells_init = malloc(width * height * sizeof(color));
    if (ngame->cells_init == NULL)
    {
        free(ngame);
        errorExit("Allocation problem for cells_init");
    }
    ngame->nb_moves_max = nb_moves_max;
    ngame->nb_moves_cur = 0;

    ngame->cells = malloc(width * height * sizeof(color));
    if (ngame->cells == NULL)
    {
        free(ngame->cells_init);
        free(ngame);
        errorExit("Allocation problem for cells");
    }
    for (uint i = 0; i < width * height; i++)
    {
        ngame->cells[i] = cells[i];
        ngame->cells_init[i] = cells[i];
    }
    ngame->width = width;
    ngame->height = height;
    ngame->wrapping = wrapping;
    return ngame;
}

uint game_height(cgame game)
{
    return game->height;
}

uint game_width(cgame game)
{
    return game->width;
}