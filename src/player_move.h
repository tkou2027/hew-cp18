#pragma once
// player move logic
// a separate file because player.cpp becomes too large and kimoi
#include "player.h"

// snap to grid
Vec3 player_move_round_position(const Vec3* v);
// check movement
PlayerState player_move_check_movement(Player* player, Vec3 direction, bool jump);
// move update
PlayerState player_move_update(Player* player);

// input
Vec3 player_move_get_input_direction(Player* player);