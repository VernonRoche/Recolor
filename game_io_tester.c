#include "game.h"
#include "game_io.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

// Auteurs : Garion,Kristo,Alex,Nicolas;
// test de commit Alex
// test de commit Nicolas
// test de commit Garion
// test de commit Kristo

void print_game(game g, bool ext, int nb_colors) // la fonction fonctionne
{
    printf("nb coups joués : %d ; nb coups max : %d\n", game_nb_moves_cur(g), game_nb_moves_max(g)); // trouver comment recup les coups joués
    for (int y = 0; y < game_height(g); y++)
    {
        for (int x = 0; x < game_width(g); x++)
        {
            printf("%c", (game_cell_current_color(g, x, y) >= 10) ? ('A' + (game_cell_current_color(g, x, y) - 10)) : (char)(game_cell_current_color(g, x, y) + 48));
        }
        printf("\n");
    }
    if (ext == 1 && game_nb_moves_cur(g) == 0)
    {
        printf("Jouer un coup: ");
        char i;
        // print numeros
        for (i = '0'; i < '0' + nb_colors && i <= '9'; i++)
        {
            printf("%c,", i);
        }
        // print lettres
        for (i = 'A'; i < 'A' + nb_colors - 10; i++)
        {
            printf("%c,", i);
        }
        printf("r ou q ; r pour redémarrer ou q pour quitter)\n");
    }
    else if (game_nb_moves_cur(g) == 0 && ext == 0)
    {
        printf("Jouer un coup: (0,1,2,3,r ou q ;  r pour redémarrer ou q pour quitter)\n");
    }
    else
    {
        printf("Jouer un coup: (num couleur%sou r pour redemarrer ou q pour quitter)\n", (nb_colors >= 10) ? ", lettre " : " ");
    }
}

game game_create_grid(uint width, uint height, bool isWrapping, uint nb_colors)
{
    game g = game_new_empty_ext(width, height, isWrapping);
    game_set_max_moves(g, 12);
    for (uint y = 0; y < game_height(g); y++)
    {
        for (uint x = 0; x < game_width(g); x++)
        {
            game_set_cell_init(g, x, y, rand() % nb_colors);
        }
    }
    return g;
}

void usage(int argc, char *argv[])
{
    fprintf(stderr, "Usage: %s <width> <height> <wrapping: yes/no> <number_colors>\n", argv[0]);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    color cells[SIZE * SIZE] = {0, 0, 0, 2, 0, 2, 1, 0, 1, 0, 3, 0, 0, 3, 3, 1, 1, 1, 1, 3, 2, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 0, 3, 3, 2, 2, 3, 1, 0, 3, 2, 1, 1, 1, 2, 2, 0, 2, 1, 2, 3, 3, 3, 3, 2, 0, 1, 0, 0, 0, 3, 3, 0, 1, 1, 2, 3, 3, 2, 1, 3, 1, 1, 2, 2, 2, 0, 0, 1, 3, 1, 1, 2, 1, 3, 1, 3, 1, 0, 1, 0, 1, 3, 3, 3, 0, 3, 0, 1, 0, 0, 2, 1, 1, 1, 3, 0, 1, 3, 1, 0, 0, 0, 3, 2, 3, 1, 0, 0, 1, 3, 3, 1, 1, 2, 2, 3, 2, 0, 0, 2, 2, 0, 2, 3, 0, 1, 1, 1, 2, 3, 0, 1};
    game new_game = game_new(cells, 12);
    game_save(new_game, "classicgame.sav");
    game_delete(new_game);

    return EXIT_SUCCESS;
}
