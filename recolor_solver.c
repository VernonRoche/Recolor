#include "game.h"
#include "game_io.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <assert.h>

char *strrev(char *string)
{
    char *end = string;
    char *begin = string;
    char tmp;
    while (*end != '\0')
    {
        end++;
    }
    end--;
    while (end != begin && begin < end)
    {
        tmp = *end;
        *end = *begin;
        *begin = tmp;
        begin++;
        end--;
    }
    return string;
}

void print_game(game g) // la fonction fonctionne
{
    printf("nb coups joués : %d ; nb coups max : %d\n", game_nb_moves_cur(g), game_nb_moves_max(g)); // trouver comment recup les coups joués
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            printf("%d", game_cell_current_color(g, j, i));
        }
        printf("\n");
    }
    if (game_nb_moves_cur(g) == 0)
    {
        printf("Jouer un coup: (0,1,2,3,r ou q ;  r pour redémarrer ou q pour quitter)\n");
    }
    else
    {
        printf("Jouer un coup: (num couleur ou r pour redemarrer ou q pour quitter)\n");
    }
}

void find_shortest_depth(game original_game, uint nb_colors, int *depth)
{
    if (game_is_over(original_game))
    {
        int nb_cur = game_nb_moves_cur(original_game);
        if (nb_cur < *depth) // si on trouve une solution avant le nb de coups max, alors on pose la limite a ce nombre de coups
        {
            *depth = nb_cur;
            return;
        }
    }

    if (game_nb_moves_cur(original_game) == *depth)
    {
        return;
    }

    game game_copied = game_copy(original_game); // On copie la game pour ne pas la modifier

    uint color_before = game_cell_current_color(original_game, 0, 0);
    for (uint i = 0; i < nb_colors; i++) // On parcourt tous les coups jouables (et optimisés)
    {
        if (i != color_before)
        {
            game_play_one_move(game_copied, i);                 // On joue le coup
            find_shortest_depth(game_copied, nb_colors, depth); // On appelle la fonction elle même sur la grille après coup
            game_delete(game_copied);
            game_copied = game_copy(original_game); // On récupère l'ancienne grille pour la prochaine itération
        }
    }
    game_delete(game_copied);
}

void append_last_color(uint last_color, char *result)
{
    char toAppend = (last_color >= 0 && last_color <= 9) ? '0' + last_color : 'A' + last_color;
    strncat(result, &toAppend, 1);
}

void solver_nb_solutions_aux(game original_game, uint nb_colors, int depth, int *nb_solutions) // rec terminale
{
    if (game_is_over(original_game))
    {
        if (game_nb_moves_cur(original_game) != 0) // si la game est déjà gagnée, il n'y a pas de solutions
        {
            (*nb_solutions)++;
            return;
        }
    }

    if (game_nb_moves_cur(original_game) == depth)
    {
        return;
    }

    game game_copied = game_copy(original_game); // On copie la game pour ne pas la modifier

    uint color_before = game_cell_current_color(original_game, 0, 0);
    for (uint i = 0; i < nb_colors; i++) // On parcourt tous les coups jouables (et optimisés)
    {
        if (i != color_before)
        {
            game_play_one_move(game_copied, i);                                   // On joue le coup
            solver_nb_solutions_aux(game_copied, nb_colors, depth, nb_solutions); // On appelle la fonction elle même sur la grille après coup
            game_delete(game_copied);
            game_copied = game_copy(original_game); // On récupère l'ancienne grille pour la prochaine itération
        }
    }
    game_delete(game_copied);
}

int solver_nb_solutions(game original_game, uint nb_colors)
{
    int nb_solutions = 0;
    printf("\nInitializing number of solutions finder\n");
    solver_nb_solutions_aux(original_game, nb_colors, game_nb_moves_max(original_game), &nb_solutions); // trouve le nb de solutions de profondeur depth
    return nb_solutions;
}

bool solver_find_min_aux(game original_game, uint nb_colors, int depth, char *result)
{
    color color_before = game_cell_current_color(original_game, 0, 0);

    if (game_is_over(original_game))
    {
        if (game_nb_moves_cur(original_game) != 0) // si la game est déjà gagnée, il n'y a pas de solutions
        {
            append_last_color(color_before, result);
            return true;
        }
    }

    if (game_nb_moves_cur(original_game) == depth)
        return false;

    game game_copied = game_copy(original_game); // On copie la game pour ne pas la modifier

    for (uint i = 0; i < nb_colors; i++) // On parcourt tous les coups jouables (et optimisés)
    {
        if (i != color_before)
        {
            game_play_one_move(game_copied, i);                                     // On joue le coup
            bool save = solver_find_min_aux(game_copied, nb_colors, depth, result); // On appelle la fonction elle même sur la grille après coup
            if (save)
            {
                if (game_nb_moves_cur(original_game) != 0)
                    append_last_color(color_before, result);
                game_delete(game_copied);
                return true;
            }
            game_delete(game_copied);
            game_copied = game_copy(original_game); // On récupère l'ancienne grille pour la prochaine itération
        }
    }
    game_delete(game_copied);
    return false;
}

char * solver_find_min(game original_game, uint nb_colors)
{
    printf("Initializing shortest solution finder\n");
    int depth = game_nb_moves_max(original_game);
    //printf("Finding shortest depth\n");
    find_shortest_depth(original_game, nb_colors, &depth);
    //printf("Found. Shortest depth is %d\n", depth);
    char *solution = malloc(sizeof(char) * depth + 1);
    solution[0] = '\0';
    solver_find_min_aux(original_game, nb_colors, depth, solution);
    strrev(solution);
    return solution;
}

char *solver_find_one(game original_game, int nb_colors)
{
    char tempActual[game_nb_moves_max(original_game) + 1];
    uint cpt = 0;
    char last = '0' + game_cell_current_color(original_game, 0, 0);
    char *solution = malloc(sizeof(char) * game_nb_moves_max(original_game) + 1);
    char charToPlay = '0';
    game test_game = game_copy(original_game);

    printf("\nInitializing random solution finder\n");

    while (true)
    {
        while (!game_is_over(test_game) && game_nb_moves_cur(test_game) <= game_nb_moves_max(test_game))
        {
            while (charToPlay == last)
            {
                charToPlay = '0' + rand() % nb_colors;
            }
            last = charToPlay;
            color colorToPlay = charToPlay - '0';
            strncat(tempActual, &charToPlay, 1);
            game_play_one_move(test_game, colorToPlay);
        }
        if (game_nb_moves_cur(test_game) <= game_nb_moves_max(test_game))
        {
            strcpy(solution, tempActual);
            game_delete(test_game);
            return solution;
        }
        cpt++;
        last = '0' + game_cell_current_color(original_game, 0, 0);
        game_restart(test_game);
        //printf("attempt n°%d\n", cpt);  for debug purposes
        tempActual[0] = '\0';
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

void print_solution_in_file(FILE *file, char *solution)
{
    uint solution_size = strlen(solution);
    for (uint i = 0; i < solution_size; i++)
    {
        fprintf(file, "%c", solution[i]);
        if(i!=solution_size-1)
            fprintf(file, " ");
        else
            fprintf(file,"\n");

    }
    fclose(file);
}

void usage()
{
    printf("recolor_solve FIND_ONE|NB_SOL|FIND_MIN <nom_fichier_pb> <prefix_fichier_sol>\n");
}

char *remove_and_cat(char *filename, char *extension)
{ //Cette fonction verifie si le fichier filename.extension existe, le supprime si oui et renvoit le propre nom du fichier(filename.extension)
    FILE *test_file = fopen(strcat(filename, extension), "r");
    if (test_file != NULL){
        fclose(test_file);
        remove(filename);
    }
    return filename;
}

int main(int argc, char **argv)
{
    // nom_fichier_pb -> nom du fichier à charger
    // prefix_fichier_sol -> nom du fichier dans lequel sauvegarder
    srand(time(NULL));
    if (argc == 4)
    {
        char *solution;
        game loaded_game;
        uint nb_colors;
        argc--;
        loaded_game = game_load(argv[2]);
        color max = 0, cur_color;
        uint nb_sol;
        for (uint x = 0; x < game_width(loaded_game); x++)
        {
            for (uint y = 0; y < game_height(loaded_game); y++)
            {
                cur_color = game_cell_current_color(loaded_game, x, y);
                if (max < cur_color)
                {
                    max = cur_color;
                }
            }
        }
        nb_colors = max + 1;
        if (strcmp(argv[1], "FIND_ONE") == 0)
        {
            printf("%s\n", argv[3]);
            FILE *saving_file = fopen(remove_and_cat(argv[3], ".sol"), "w");
            nb_sol = solver_nb_solutions(loaded_game, nb_colors);
            if (nb_sol == 0)
            {
                fprintf(saving_file, "NO SOLUTION\n");
                fclose(saving_file);
            }
            else
            {
                solution = solver_find_one(loaded_game, nb_colors);
                print_solution_in_file(saving_file, solution);
                printf("Done ! Check solution in the file \"%s\"\n", argv[3]);
                printf("Solution is %s\n", solution);

            }
        }

        else if (strcmp(argv[1], "NB_SOL") == 0)
        {
            FILE *sol = fopen(remove_and_cat(argv[3], ".nbsol"), "w");
            nb_sol = solver_nb_solutions(loaded_game, nb_colors);
            fprintf(sol, "NB_SOL = %d\n", nb_sol);
            fclose(sol);
            printf("Done ! The number of solutions is in the file \"%s\"\n", argv[3]);
            printf("Number of solutions is %d\n", nb_sol);
        }

        else if (strcmp(argv[1], "FIND_MIN") == 0)
        {
            FILE *sol = fopen(remove_and_cat(argv[3], ".sol"), "w");
            solution = solver_find_min(loaded_game, nb_colors);
            print_solution_in_file(sol, solution);
            printf("Done ! Solution is in the file \"%s\"\n", argv[3]);
            printf("Solution is %s\n", solution);
        }
        else
        {
            printf("Please enter a proper mode name !\n");
            usage();
        }
        game_delete(loaded_game);
    }
    else
    {
        usage();
    }

    return EXIT_SUCCESS;
}