#ifndef DEF_TEST_PLAYBOARD
#define DEF__TEST_PLAYBOARD

#include <stdlib.h>
#include <stdio.h>
#include"model/board/carcassone_playboard.h"
#include"model/board/tile.h"

void test_new_location();
void test_location_cmp();
void test_new_playboard_node();
void test_connect_is_possible();
void test_node_cmp();
void test_init_playboard();
void test_get_opposite_side();
void test_get_neighbor_loc();
void test_create_neighbor_for();
void test_playboard_get_neighbor();
void test_playboard_set_neighbor();
void test_playboard_is_neigh_null();
void test_playboard_get_edge_nodes();

void test_playboard_run_all();

#endif // DEF_TEST_PLAYBOARD
