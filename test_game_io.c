#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "game_io.h"

bool test_game_io()
{
    color cells[16] = {0, 1, 2, 3, 1, 1, 0, 2, 0, 1, 3, 2, 1, 3, 0, 1};
    game test_game = game_new_ext(4,4,cells,6,true);
    game_save(test_game,"testgame.rec");
    game_delete(test_game);
    test_game = game_load("testgame.rec");
    if(game_nb_moves_max(test_game)!=6)
    {
        game_delete(test_game);
        system("rm testgame.rec");
        fprintf(stderr, "Number of max moves is wrong. Problem detected in game_save or game_load\n");
        return false;
    }
    for (uint x = 0; x < game_width(test_game); x++)
    {
        for (uint y = 0; y < game_height(test_game); y++)
        {
            if (game_cell_current_color(test_game, x, y) != cells[(y * game_width(test_game)) + x])
            {
                game_delete(test_game);
                system("rm testgame.rec");
                fprintf(stderr, "At least a color cell is not properly saved. Problem detected in game_save or game_load\n");
                return false;
            }
        }
    }
    if(game_is_wrapping(test_game)!=true){
        game_delete(test_game);
        system("rm testgame.rec");
        fprintf(stderr, "Wrapping parameter is wrong. Problem detected in game_save or game_load\n");
        return false;
    }
    if(game_width(test_game)!=4 || game_height(test_game)!=4){
        game_delete(test_game);
        system("rm testgame.rec");
        fprintf(stderr, "Width or height is wrong. Problem detected in game_save or game_load\n");
        return false;
    }
    if(game_nb_moves_cur(test_game)!=0){
        game_delete(test_game);
        system("rm testgame.rec");
        fprintf(stderr, "Number of cur moves is wrong. Problem detected in game_save or game_load\n");
        return false;
    }
    game_delete(test_game);
    system("rm testgame.rec");
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
    if (strcmp("game_io", argv[1]) == 0)
        ok = test_game_io();
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