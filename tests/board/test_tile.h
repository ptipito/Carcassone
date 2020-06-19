#ifndef DEF_TEST_TILE
#define DEF_TEST_TILE

#include <stdlib.h>
#include <stdio.h>
#include "model/board/tile.h"

int test_initial_tile_creation();
void test_CBT_display_tile();
void test_parse_tile_file();
void test_tile_tile_cmp();
void test_turn_tile();
void test_new_empty_tile();
void test_CBT_new_node();
void test_tile_is_valid_loc();
void test_get_node_from_loc();
void test_tile_get_loc_from_str();
void test_tile_get_node_type_from_str();
void test_tile_pars_merchandise_type();
void test_tile_set_single_connexion();
void test_tiles_connect_in();
void test_tile_get_tile_file_path();
void test_tile_pawn_matches_node();
void test_tile_add_pawn();
void test_tile_rm_pawn();
void test_tile_run_all();

void test_tile_list_new();
void test_tile_list_append();
void test_tile_list_append_list();
void test_tile_list_rm();
void test_tile_list_rm_list();


#endif // DEF_TEST_TILE
