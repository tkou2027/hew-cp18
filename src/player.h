#pragma once
#include "vec.h"
#include "sprite.h"
#include "world.h"
#include "item.h"
#include "audio.h"

#define PLAYER_NUM 1

enum PlayerState
{
    PlayerState_Default,
    PlayerState_Moving,
    PlayerState_Hit,
};

//enum PlayerMoveType
//{
//    PlayerMoveType_Idel, // default
//    PlayerMoveType_MoveSingle,
//    PlayerMoveType_MoveDiagnal,
//    PlayerMoveType_Jump,
//};

enum PlayerMoveType
{
    PlayerMoveType_Idel, // default
    PlayerMoveType_Move,
    PlayerMoveType_Hit,
    PlayerMoveType_Edge,
    PlayerMoveType_Jump,
    PlayerMoveType_Fall,
    PlayerMoveType_JumpMove,
};

struct PlayerMoveAtomic {
    PlayerMoveType type;
    Vec3 direction;
    Vec3 pos_start;
    Vec3 pos_target;
    float time_moved;
    bool init;
    bool has_sound_start;
    AudioSound sound_start;
    bool has_sound_end;
    AudioSound sound_end;
};

struct PlayerMoveState {
    PlayerMoveAtomic moves[2];
    int curr_move_id;
    int moves_num;
};

//enum PlayerMoveCollisionType
//{
//    PlayerMoveCollisionType_Pass,
//    PlayerMoveCollisionType_Hit,
//    PlayerMoveCollisionType_Fall,
//    PlayerMoveCollisionType_Edge,
//    PlayerMoveCollisionType_Jump
//};

//struct PlayerMovement {
//    PlayerMoveType type;
//    Vec3 pos_start;
//    Vec3 pos_targets[2];
//};

struct Player {
    PlayerState state;
    Vec3 pos; // world pos
    Sprite sprite;
    Coord2 pos_screen;
    Vec2 sprite_dir; // face direction

    // state
    PlayerMoveState move_state;
	// draw
    bool face_left;
	Vec3 desired_direction;
    //PlayerMovement movement;
    //float time_moved;
    //Vec3 pos_move_start;
    //Vec3 pos_move_target;
    int block_x;
    int block_y;
    int block_z;
    BlockInfo block_stand;
    BlockInfo block_stand_prev;

    // attack
    bool hurt;
    float hurt_countdown;

	// get item
	bool item_gained;
	float item_countdown;
    Item item;

    // respawn
    int respawn_x;
    int respawn_z;
};

Player* player_get_players();

void player_game_init();
void player_game_set_position(const Vec3* pos);
// void player_set_position(Player* player, const Vec3* pos);
void player_game_update();
Color player_get_color(Player *player); // ray draw
void player_game_draw();