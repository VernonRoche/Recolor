#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

bool test_game_new_ext(void)
{
    color cells[12 * 13] = {0, 0, 0, 2, 0, 2, 1, 0, 1, 0, 3, 0, 0, 3, 3, 1, 1, 1, 1, 3, 2, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 0, 3, 3, 2, 2, 3, 1, 0, 3, 2, 1, 1, 1, 2, 2, 0, 2, 1, 2, 3, 3, 3, 3, 2, 0, 1, 0, 0, 0, 3, 3, 0, 1, 1, 2, 3, 3, 2, 1, 3, 1, 1, 2, 2, 2, 0, 0, 1, 3, 1, 1, 2, 1, 3, 1, 3, 1, 0, 1, 0, 1, 3, 3, 3, 0, 3, 0, 1, 0, 0, 2, 1, 1, 1, 3, 0, 1, 3, 1, 0, 0, 0, 3, 2, 3, 1, 0, 0, 1, 3, 3, 1, 1, 2, 2, 3, 2, 0, 0, 2, 2, 0, 2, 3, 0, 1, 1, 1, 2, 3, 0, 1, 0, 0, 3, 1, 2, 3, 1, 1, 2, 0, 2, 1};
    game new_game = game_new_ext(12, 13, cells, 10, true);
    if (new_game == NULL)
    {
        game_delete(new_game);
        fprintf(stderr, "The game is not properly initialized!(Faulty function : game_new_ext) \n");
        return false;
    }
    if (!game_is_wrapping(new_game))
    {
        game_delete(new_game);
        fprintf(stderr, "Wrong wrapping state . Faulty function : game_new_ext \n");
        return false;
    }
    if (game_nb_moves_max(new_game) != 10)
    {
        game_delete(new_game);
        fprintf(stderr, "The number of max moves is not properly initialized.(Faulty function : game_new_ext)\n");
        return false;
    }
    if (game_nb_moves_cur(new_game) != 0)
    {
        game_delete(new_game);
        fprintf(stderr, "The number of current moves is not properly initialized.(Faulty function : game_new_ext)\n");
        return false;
    }
    if (game_width(new_game) != 12)
    {
        game_delete(new_game);
        fprintf(stderr, "Wrong game width.(Faulty function : game_new_ext)\n");
        return false;
    }
    if (game_height(new_game) != 13)
    {
        game_delete(new_game);
        fprintf(stderr, "Wrong game height.(Faulty function : game_new_ext)\n");
        return false;
    }
    for (uint x = 0; x < game_width(new_game); x++)
    {
        for (uint y = 0; y < game_height(new_game); y++)
        {
            if (game_cell_current_color(new_game, x, y) != cells[(y * game_width(new_game)) + x])
            {
                game_delete(new_game);
                fprintf(stderr, "At least a color cell is not properly initialized.(Faulty function : game_new_ext)\n");
                return false;
            }
        }
    }
    game_delete(new_game);
    return true;
}

bool test_game_new_empty_ext(void)
{
    game new_game = game_new_empty_ext(5, 6, true);
    if (new_game == NULL)
    {
        game_delete(new_game);
        fprintf(stderr, "The game is not properly initialized!(Faulty function : game_new_empty_ext)\n");
        return false;
    }
    if (!game_is_wrapping(new_game))
    {
        game_delete(new_game);
        fprintf(stderr, "Wrong wrapping state . Faulty function : game_new_empty_ext \n");
        return false;
    }
    if (game_nb_moves_max(new_game) != 0)
    {
        game_delete(new_game);
        fprintf(stderr, "The number of max moves is not properly initialized.(Faulty function : game_new_empty_ext)\n");
        return false;
    }
    if (game_width(new_game) != 5)
    {
        game_delete(new_game);
        fprintf(stderr, "Wrong game width.(Faulty function : game_new_empty_ext)\n");
        return false;
    }
    if (game_height(new_game) != 6)
    {
        game_delete(new_game);
        fprintf(stderr, "Wrong game height.(Faulty function : game_new_empty_ext)\n");
        return false;
    }
    for (uint x = 0; x < game_width(new_game); x++)
    {
        for (uint y = 0; y < game_height(new_game); y++)
        {
            if (game_cell_current_color(new_game, x, y) != 0)
            {
                game_delete(new_game);
                fprintf(stderr, "At least a color cell is not properly initialized.(Faulty function : game_new_empty_ext)\n");
                return false;
            }
        }
    }
    if (game_nb_moves_cur(new_game) != 0)
    {
        game_delete(new_game);
        fprintf(stderr, "The number of current moves is not properly initialized.(Faulty function : game_new_empty_ext)\n");
        return false;
    }
    game_delete(new_game);
    return true;
}

bool test_game_set_cell_init()
{
    game gamex = game_new_empty_ext(12, 12, false);
    game_set_cell_init(gamex, 4, 2, 3);
    game_set_cell_init(gamex, 6, 1, 2);
    game_set_cell_init(gamex, 8, 9, 0);
    if (game_cell_current_color(gamex, 4, 2) != 3 || game_cell_current_color(gamex, 6, 1) != 2 || game_cell_current_color(gamex, 8, 9) != 0)
    {
        game_delete(gamex);
        fprintf(stderr, "The fuction game_set_cell_init is faulty.(It didn't change the color of a set cell)\n");
        return false;
    }
    game_set_cell_init(gamex, 0, 0, 2);
    game_play_one_move(gamex, 3);
    game_restart(gamex);
    if (game_cell_current_color(gamex, 0, 0) != 2)
    {
        game_delete(gamex);
        fprintf(stderr, "The fuction game_set_cell_init is faulty.(It didn't permanently modify the initial color of a set cell)\n");
        return false;
    }
    game_delete(gamex);
    return true;
}

bool test_game_set_max_moves()
{
    game new_game = game_new_empty_ext(5, 5, false);
    game_set_max_moves(new_game, 15);
    if (game_nb_moves_max(new_game) != 15)
    {
        game_delete(new_game);
        fprintf(stderr, "game_set_max_moves did not store the right number of max moves");
        return false;
    }
    game_delete(new_game);
    return true;
}

bool test_game_nb_moves_max()
{
    color cells[12 * 12] = {0, 0, 0, 2, 0, 2, 1, 0, 1, 0, 3, 0, 0, 3, 3, 1, 1, 1, 1, 3, 2, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 0, 3, 3, 2, 2, 3, 1, 0, 3, 2, 1, 1, 1, 2, 2, 0, 2, 1, 2, 3, 3, 3, 3, 2, 0, 1, 0, 0, 0, 3, 3, 0, 1, 1, 2, 3, 3, 2, 1, 3, 1, 1, 2, 2, 2, 0, 0, 1, 3, 1, 1, 2, 1, 3, 1, 3, 1, 0, 1, 0, 1, 3, 3, 3, 0, 3, 0, 1, 0, 0, 2, 1, 1, 1, 3, 0, 1, 3, 1, 0, 0, 0, 3, 2, 3, 1, 0, 0, 1, 3, 3, 1, 1, 2, 2, 3, 2, 0, 0, 2, 2, 0, 2, 3, 0, 1, 1, 1, 2, 3, 0, 1};
    game new_game = game_new_ext(12, 12, cells, 13, false);
    if (game_nb_moves_max(new_game) != 13)
    {
        game_delete(new_game);
        fprintf(stderr, "game_nb_moves_max did not store the right number of moves max");
        return false;
    }
    game_delete(new_game);
    return true;
}

bool test_game_cell_current_color()
{
    color cells[12 * 12] = {0, 0, 0, 2, 0, 2, 1, 0, 1, 0, 3, 0, 0, 3, 3, 1, 1, 1, 1, 3, 2, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 0, 3, 3, 2, 2, 3, 1, 0, 3, 2, 1, 1, 1, 2, 2, 0, 2, 1, 2, 3, 3, 3, 3, 2, 0, 1, 0, 0, 0, 3, 3, 0, 1, 1, 2, 3, 3, 2, 1, 3, 1, 1, 2, 2, 2, 0, 0, 1, 3, 1, 1, 2, 1, 3, 1, 3, 1, 0, 1, 0, 1, 3, 3, 3, 0, 3, 0, 1, 0, 0, 2, 1, 1, 1, 3, 0, 1, 3, 1, 0, 0, 0, 3, 2, 3, 1, 0, 0, 1, 3, 3, 1, 1, 2, 2, 3, 2, 0, 0, 2, 2, 0, 2, 3, 0, 1, 1, 1, 2, 3, 0, 1};
    game new_game = game_new_ext(12, 12, cells, 12, false);
    game_set_cell_init(new_game, 2, 3, GREEN);
    if (game_cell_current_color(new_game, 2, 3) != GREEN)
    {
        game_delete(new_game);
        fprintf(stderr, "game_cell_current_color did not store the right color");
        return false;
    }
    game_set_cell_init(new_game, 3, 3, YELLOW);
    if (game_cell_current_color(new_game, 3, 3) != YELLOW)
    {
        game_delete(new_game);
        fprintf(stderr, "game_cell_current_color did not store the right color");
        return false;
    }
    game_delete(new_game);
    return true;
}

bool test_game_nb_moves_cur()
{
    color cells[12 * 12] = {0, 0, 0, 2, 0, 2, 1, 0, 1, 0, 3, 0, 0, 3, 3, 1, 1, 1, 1, 3, 2, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 0, 3, 3, 2, 2, 3, 1, 0, 3, 2, 1, 1, 1, 2, 2, 0, 2, 1, 2, 3, 3, 3, 3, 2, 0, 1, 0, 0, 0, 3, 3, 0, 1, 1, 2, 3, 3, 2, 1, 3, 1, 1, 2, 2, 2, 0, 0, 1, 3, 1, 1, 2, 1, 3, 1, 3, 1, 0, 1, 0, 1, 3, 3, 3, 0, 3, 0, 1, 0, 0, 2, 1, 1, 1, 3, 0, 1, 3, 1, 0, 0, 0, 3, 2, 3, 1, 0, 0, 1, 3, 3, 1, 1, 2, 2, 3, 2, 0, 0, 2, 2, 0, 2, 3, 0, 1, 1, 1, 2, 3, 0, 1};
    game new_game = game_new_ext(12, 12, cells, 12, false);
    for (int i = 0; i < 4; i++)
    {
        if (game_nb_moves_cur(new_game) != i)
        {
            game_delete(new_game);
            fprintf(stderr, "function nb_moves_cur is not following the real number of current moves\n");
            return false;
        }
        game_play_one_move(new_game, i);
    }
    if (game_nb_moves_cur(new_game) != 4)
    {
        game_delete(new_game);
        fprintf(stderr, "function nb_moves_cur is not following the real number of current moves\n");
        return false;
    }
    game_delete(new_game);
    return true;
}

bool test_game_play_one_move()
{
    game new_game = game_new_empty_ext(5, 5, false);
    for (int i = 0; i < NB_COLORS; i++) // test pour tt les couleurs
    {
        if (game_nb_moves_cur(new_game) != i)
        {
            game_delete(new_game);
            fprintf(stderr, "function game_play_one_move is not playing the right number of moves\n");
            return false;
        }
        game_play_one_move(new_game, i);
        for (uint y = 0; y < game_height(new_game); y++)
        {
            for (uint x = 0; x < game_width(new_game); x++)
            {
                if (game_cell_current_color(new_game, y, x) != i)
                {
                    game_delete(new_game);
                    fprintf(stderr, "function game_play_one_move is not changing the color of the cells\n");
                    return false;
                }
            }
        }
    }
    game_delete(new_game);
    game second_game = game_new_empty_ext(6, 12, true);
    for (int j = 0; j < NB_COLORS; j++) // test pour tt les couleurs
    {
        if (game_nb_moves_cur(second_game) != j)
        {
            game_delete(second_game);
            fprintf(stderr, "function game_play_one_move is not playing the right number of moves\n");
            return false;
        }
        game_play_one_move(second_game, j);
    }
    game_delete(second_game);
    return true;
}

bool test_game_delete(void)
{
    game new_game = game_new_empty();
    game_delete(new_game);
    return true;
}

bool test_game_copy()
{
    color cells[3 * 3] = {0, 1, 2, 3, 0, 3, 2, 1, 0};
    game new_game = game_new_ext(3, 3, cells, 12, false);
    game second_game = game_copy(new_game);
    if (second_game == NULL)
    {
        game_delete(new_game);
        game_delete(second_game);
        fprintf(stderr, "not enough space for malloc in game_copy\n");
        return false;
    }
    if (game_is_wrapping(new_game)!=game_is_wrapping(second_game))
    {
        game_delete(new_game);
        fprintf(stderr, "Wrong wrapping state . Faulty function : game_new_empty_ext \n");
        return false;
    }
    if (game_width(new_game) != game_width(second_game))
    {
        game_delete(new_game);
        fprintf(stderr, "Wrong game width.(Faulty function : game_new_empty_ext)\n");
        return false;
    }
    if (game_height(new_game) != game_height(second_game))
    {
        game_delete(new_game);
        fprintf(stderr, "Wrong game height.(Faulty function : game_new_empty_ext)\n");
        return false;
    }
    if (game_is_over(new_game) != game_is_over(second_game)) // game is over
    {
        game_delete(new_game);
        game_delete(second_game);
        fprintf(stderr, "game_copy is giving a winning game when it was not\n");
        return false;
    }
    if (game_nb_moves_cur(new_game) != game_nb_moves_cur(second_game))
    { // game nb moves cur
        game_delete(new_game);
        game_delete(second_game);
        fprintf(stderr, "game_copy is not copying the good number of current moves\n");
        return false;
    }
    for (uint x = 0; x < 3; x++)
    {
        for (uint y = 0; y < 3; y++)
        {
            if (game_cell_current_color(new_game, y, x) != game_cell_current_color(second_game, y, x))
            { // meme grille
                game_delete(new_game);
                game_delete(second_game);
                fprintf(stderr, "game_copy did not copy cells correctly\n");
                return false;
            }
        }
    }
    if (game_nb_moves_max(new_game) != game_nb_moves_max(second_game))
    { // mov max
        game_delete(new_game);
        game_delete(second_game);
        fprintf(stderr, "game_copy is not copying the good number of max moves\n");
        return false;
    }
    game_delete(new_game);
    game_delete(second_game);
    return true;
}

bool test_game_is_over()
{
    color cells[12 * 12] = {0, 0, 0, 2, 0, 2, 1, 0, 1, 0, 3, 0, 0, 3, 3, 1, 1, 1, 1, 3, 2, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 0, 3, 3, 2, 2, 3, 1, 0, 3, 2, 1, 1, 1, 2, 2, 0, 2, 1, 2, 3, 3, 3, 3, 2, 0, 1, 0, 0, 0, 3, 3, 0, 1, 1, 2, 3, 3, 2, 1, 3, 1, 1, 2, 2, 2, 0, 0, 1, 3, 1, 1, 2, 1, 3, 1, 3, 1, 0, 1, 0, 1, 3, 3, 3, 0, 3, 0, 1, 0, 0, 2, 1, 1, 1, 3, 0, 1, 3, 1, 0, 0, 0, 3, 2, 3, 1, 0, 0, 1, 3, 3, 1, 1, 2, 2, 3, 2, 0, 0, 2, 2, 0, 2, 3, 0, 1, 1, 1, 2, 3, 0, 1};
    game new_game = game_new_ext(12, 12, cells, 12, false);
    color solutions[12] = {3, 1, 3, 1, 0, 3, 1, 0, 1, 3, 2, 0};

    if (game_nb_moves_cur(new_game) <= game_nb_moves_max(new_game))
    {
        for (int i = 0; i < game_height(new_game); i++)
        {

            for (int j = 0; j < game_width(new_game); j++)
            {

                if (game_cell_current_color(new_game, 0, 0) != game_cell_current_color(new_game, j, i))
                {
                    if (game_is_over(new_game))
                    {
                        game_delete(new_game);
                        fprintf(stderr, "Faulty function : game_is_over");
                        return false;
                    }
                }
            }
        }
    }

    for (unsigned int i = 0; i < 12; i++)
    {
        game_play_one_move(new_game, solutions[i]);
    }
    if (!game_is_over(new_game))
    {
        fprintf(stderr, "Faulty function : game_is_over");
        game_delete(new_game);
        return false;
    }
    game_delete(new_game);
    return true;
}

bool test_game_restart()
{
    color cells[SIZE * SIZE] = {0, 0, 0, 2, 0, 2, 1, 0, 1, 0, 3, 0, 0, 3, 3, 1, 1, 1, 1, 3, 2, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 0, 3, 3, 2, 2, 3, 1, 0, 3, 2, 1, 1, 1, 2, 2, 0, 2, 1, 2, 3, 3, 3, 3, 2, 0, 1, 0, 0, 0, 3, 3, 0, 1, 1, 2, 3, 3, 2, 1, 3, 1, 1, 2, 2, 2, 0, 0, 1, 3, 1, 1, 2, 1, 3, 1, 3, 1, 0, 1, 0, 1, 3, 3, 3, 0, 3, 0, 1, 0, 0, 2, 1, 1, 1, 3, 0, 1, 3, 1, 0, 0, 0, 3, 2, 3, 1, 0, 0, 1, 3, 3, 1, 1, 2, 2, 3, 2, 0, 0, 2, 2, 0, 2, 3, 0, 1, 1, 1, 2, 3, 0, 1};
    game new_game = game_new_ext(12, 12, cells, 12, false);
    game_play_one_move(new_game, 2);
    game_restart(new_game);
    if (game_cell_current_color(new_game, 0, 0) != 0)
    {
        fprintf(stderr, "First cell did not get reinitialized as expected. Faulty function : game_restart\n");
        game_delete(new_game);
        return false;
    }
    if (game_nb_moves_cur(new_game) != 0)
    {
        fprintf(stderr, "Number of moves made did not get reinitialized as expected. Faulty function : game_restart\n");
        game_delete(new_game);
        return false;
    }
    game_delete(new_game);
    return true;
}

bool test_game_is_wrapping(void)
{
    game gamex = game_new_empty_ext(5, 5, true);
    if (!game_is_wrapping(gamex))
    {
        game_delete(gamex);
        fprintf(stderr, "Faulty function : game_is_wrapping \n");
        return false;
    }
    game_delete(gamex);
    return true;
}

bool test_game_height(void)
{
    game gamex = game_new_empty_ext(5, 6, true);
    if (game_height(gamex) != 6)
    {
        game_delete(gamex);
        return false;
    }
    game_delete(gamex);
    return true;
}

bool test_game_width(void)
{
    game gamex = game_new_empty_ext(5, 6, true);
    if (game_width(gamex) != 5)
    {
        game_delete(gamex);
        return false;
    }
    game_delete(gamex);
    return true;
}

void usage(int argc, char *argv[])
{
    fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    if (argc == 1)
        usage(argc, argv);

    // start test
    fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
    bool ok = false;
    if (strcmp("game_new_ext", argv[1]) == 0)
        ok = test_game_new_ext();
    else if (strcmp("game_new_empty_ext", argv[1]) == 0)
        ok = test_game_new_empty_ext();
    else if (strcmp("game_is_wrapping", argv[1]) == 0)
        ok = test_game_is_wrapping();
    else if (strcmp("game_height", argv[1]) == 0)
        ok = test_game_height();
    else if (strcmp("game_width", argv[1]) == 0)
        ok = test_game_width();
    else if (strcmp("game_set_cell_init", argv[1]) == 0)
        ok = test_game_set_cell_init();
    else if (strcmp("game_set_max_moves", argv[1]) == 0)
        ok = test_game_set_max_moves();
    else if (strcmp("game_nb_moves_max", argv[1]) == 0)
        ok = test_game_nb_moves_max();
    else if (strcmp("game_cell_current_color", argv[1]) == 0)
        ok = test_game_cell_current_color();
    else if (strcmp("game_nb_moves_cur", argv[1]) == 0)
        ok = test_game_nb_moves_cur();
    else if (strcmp("game_play_one_move", argv[1]) == 0)
        ok = test_game_play_one_move();
    else if (strcmp("game_delete", argv[1]) == 0)
        ok = test_game_delete();
    else if (strcmp("game_copy", argv[1]) == 0)
        ok = test_game_copy();
    else if (strcmp("game_is_over", argv[1]) == 0)
        ok = test_game_is_over();
    else if (strcmp("game_restart", argv[1]) == 0)
        ok = test_game_restart();
    else
    {
        fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // print test result
    if (ok)
    {
        fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
        return EXIT_SUCCESS;
    }
    else
    {
        fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
        return EXIT_FAILURE;
    }
}
