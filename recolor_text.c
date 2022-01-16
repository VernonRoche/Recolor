#include "game.h"
#include "game_io.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

// Auteurs : Garion,Kristo,Alex,Nicolas;
//test de commit Alex
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
    if(ext == 1 && game_nb_moves_cur(g) == 0)
    {
        printf("Jouer un coup: ");
        char i;
        // print numeros
        for(i = '0'; i < '0'+ nb_colors && i <= '9'; i++)
        {
            printf("%c,", i);
        }
        // print lettres
        for(i = 'A'; i < 'A' + nb_colors - 10; i++)
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
        printf("Jouer un coup: (num couleur%sou r pour redemarrer ou q pour quitter)\n", (nb_colors >= 10) ? ", lettre " : " " );
    }
}

game game_create_grid(uint width, uint height, bool isWrapping, uint nb_colors)
{
    game g = game_new_empty_ext(width, height, isWrapping);
    game_set_max_moves(g, (isWrapping) ? (sqrt(width*height)* nb_colors/ 4)/2 : (sqrt(width*height)* nb_colors/ 4));
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
    int nb_colors = NB_COLORS; // by default nb_colors = 4
    game new_game;
    if ((argc != 1 && argc!=2 && argc < 4) || argc > 5)
    {
        fprintf(stderr, "Wrong number of arguments\n");
        usage(argc, argv);
    }
    if (argc == 1) // if there's no argument
    {
        color cells[SIZE * SIZE] = {0, 0, 0, 2, 0, 2, 1, 0, 1, 0, 3, 0, 0, 3, 3, 1, 1, 1, 1, 3, 2, 0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 0, 3, 3, 2, 2, 3, 1, 0, 3, 2, 1, 1, 1, 2, 2, 0, 2, 1, 2, 3, 3, 3, 3, 2, 0, 1, 0, 0, 0, 3, 3, 0, 1, 1, 2, 3, 3, 2, 1, 3, 1, 1, 2, 2, 2, 0, 0, 1, 3, 1, 1, 2, 1, 3, 1, 3, 1, 0, 1, 0, 1, 3, 3, 3, 0, 3, 0, 1, 0, 0, 2, 1, 1, 1, 3, 0, 1, 3, 1, 0, 0, 0, 3, 2, 3, 1, 0, 0, 1, 3, 3, 1, 1, 2, 2, 3, 2, 0, 0, 2, 2, 0, 2, 3, 0, 1, 1, 1, 2, 3, 0, 1};
        new_game = game_new(cells, 12);
        print_game(new_game, false, nb_colors);
    }
    if(argc == 2){
        new_game = game_load(argv[1]);
        uint maxi=0;
        for(uint x=0;x<game_width(new_game);x++){
            for(uint y=0;y<game_height(new_game);y++){
                    if(game_cell_current_color(new_game,x,y)>maxi) maxi=game_cell_current_color(new_game,x,y);
            }
        }
        nb_colors=maxi+1;
        print_game(new_game,game_is_wrapping(new_game),maxi+1);
    }
    if (argc >= 4) // 3 arguments -> is wrapping
    {
        uint width = atoi(argv[1]);
        uint height = atoi(argv[2]);
        if (width == 0 || height == 0)
        {
            fprintf(stderr, "Wrong size%sgiven to recolor game\n", (width == 0 && height == 0) ? "s " : " ");
            usage(argc, argv);
        }
        int testIsWrapping = (strcmp(argv[3], "false") == 0 || strcmp(argv[3], "no") == 0) ? false : (strcmp(argv[3], "true") == 0 || strcmp(argv[3], "yes") == 0) ? true : -1;
        if (testIsWrapping == -1)
        {
            fprintf(stderr, "Can't determine wether wrapping is true or false\n");
            usage(argc, argv);
        }
        bool isWrapping = testIsWrapping;
        printf("wrapping = %s\n", (isWrapping) ? "true" : "false");
        if (argc == 5) // 4 arguments -> nb_colors
        {
            nb_colors = atoi(argv[4]);
            if (nb_colors <= 1 || nb_colors > 16)
            {
                fprintf(stderr, "Wrong nb_color given to recolor game\n");
                usage(argc, argv);
            }
        }
        new_game = game_create_grid(width, height, isWrapping, nb_colors);
        print_game(new_game, true, nb_colors);
    }
    while (!game_is_over(new_game))
    {

        int c = getchar();
        if (c == EOF)
        {
            game_delete(new_game);
            return EXIT_SUCCESS;
        }
        if (c == 'q')
        {
            printf("DOMMAGE\n");
            game_delete(new_game);
            return EXIT_SUCCESS;
        }
        if (c == 'r')
        {
            game_restart(new_game);
            print_game(new_game, false, nb_colors);
        }
        if (nb_colors <= 9)
        {
            if (c >= '0' && c <= ('0' + nb_colors - 1))
            {
                c = c - 48;
                game_play_one_move(new_game, c);
                print_game(new_game, false, nb_colors);
            }
        }
        else
        {
            if (c >= '0' && c <= '9')
            {
                c = c - 48;
                game_play_one_move(new_game, c);
                print_game(new_game, false, nb_colors);
            }
            if (c >= 'A' && c <= 'A' + nb_colors - 11)
            {
                c = c - 55;
                game_play_one_move(new_game, c);
                print_game(new_game, nb_colors, false);
            }
        }
    }
    if (game_nb_moves_cur(new_game) <= game_nb_moves_max(new_game))
    {
        printf("BRAVO\n");
    }
    game_delete(new_game);
    return EXIT_SUCCESS;
}
