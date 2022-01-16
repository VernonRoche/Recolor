#version minimale de cmake nécessaire
cmake_minimum_required(VERSION 2.6)
#nom du projet
project (recolor_text)
#définit le nom du programme ainsi que
#ses sources
include(CTest)
enable_testing()

set(CMAKE_C_FLAGS "-std=c99 -Wall") ### options communes
set(CMAKE_C_FLAGS_DEBUG "-g -DDEBUG --coverage") ### options spécifiques
set(CMAKE_C_FLAGS_RELEASE "-O3") ### options spécifiques

add_library(game game.c game.h game_io.c game_io.h)
#EXECUTABLE MAIN
add_executable(recolor_text recolor_text.c)
add_executable(recolor_solve recolor_solver.c)
#EXECUTABLES TEST
add_executable(test_game_V2 test_game_V2.c)
add_executable(test_game_io test_game_io.c)

add_executable(game_io_tester game_io_tester.c)

target_link_libraries(recolor_text m game)
target_link_libraries(recolor_solve m game)
target_link_libraries(test_game_V2 m game)
target_link_libraries(test_game_io m game)
target_link_libraries(game_io_tester m game)



add_test(test_game_V2.game_new_ext ./test_game_V2 game_new_ext)
add_test(test_game_V2.game_new_empty_ext ./test_game_V2 game_new_empty_ext)
add_test(test_game_V2.game_is_wrapping ./test_game_V2 game_is_wrapping)
add_test(test_game_V2.game_height ./test_game_V2 game_height)
add_test(test_game_V2.game_width ./test_game_V2 game_width)
add_test(test_game_V2.game_set_max_moves  ./test_game_V2 game_set_max_moves)
add_test(test_game_V2.game_nb_moves_max  ./test_game_V2 game_nb_moves_max)
add_test(test_game_V2.game_cell_current_color  ./test_game_V2 game_cell_current_color)
add_test(test_game_V2.game_nb_moves_cur  ./test_game_V2 game_nb_moves_cur)
add_test(test_game_V2.game_play_one_move  ./test_game_V2 game_play_one_move)
add_test(test_game_V2.game_copy  ./test_game_V2 game_copy)
add_test(test_game_V2.game_set_cell_init  ./test_game_V2 game_set_cell_init)
add_test(test_game_V2.game_is_over  ./test_game_V2 game_is_over)
add_test(test_game_V2.game_restart  ./test_game_V2 game_restart)
add_test(test_game_V2.game_delete  ./test_game_V2 game_delete)

add_test(test_game_io ./test_game_io game_io)
