#ifndef DEF_TEST_GAME
#define DEF_TEST_GAME
#include <stdlib.h>
#include <stdio.h>
#include "model/game/game.h"

void test_game_initiate();
void test_game_rim_to_playboard_update_one_side();
void test_game_node_transfer_rim_to_playboard();
void test_game_play_tile_in();
void test_game_play_pawn_in();
void test_game_run_all();
void test_game_merge_constructs_with_neighbor();
void test_game_update_constructs();
void test_game_can_play_pawn_in();

#endif // DEF_TEST_GAME
