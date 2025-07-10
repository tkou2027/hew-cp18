#include "player_move.h"
#include "player.h"
#include "timer.h"
#include "world.h"
#include "world_view.h"
#include "input.h"
#include <cmath>
#include <memory.h>

// collision detection
PlayerMoveType player_move_test_collision_move(Player* player, Vec3 direction);
PlayerMoveType player_move_test_collision_jump(Player* player, Vec3 direction);
// check movement
void player_move_try_move_single(Player* player, Vec3 direction);
void player_move_try_jump(Player* player, Vec3 direction);
// move update
void player_move_update_move(Player* player, const PlayerMoveAtomic* move, float t);
void player_move_update_fall(Player* player, const PlayerMoveAtomic* move, float t);
void player_move_update_jump(Player* player, PlayerMoveAtomic* move, float t);
void player_move_update_jump_move(Player* player, const PlayerMoveAtomic* move, float t);

// round to grid, x, z set to 0.5
Vec3 player_move_round_position(const Vec3* v) {
    return Vec3{ floorf(v->x) + 0.5f, roundf(v->y), floorf(v->z) + 0.5f };
}

void player_move_init_movement(PlayerMoveAtomic* move) {
    memset(move, 0, sizeof(PlayerMoveAtomic));
}

Vec3 player_move_get_input_direction(Player* player) {
    Vec3 direction{ 0, 0, 0 };
    Vec3 desired_direction{ 0, 0, 0 };
    if (input_if_key_pressed(InputKey_Up)) {
        // direction.z += 1;
        desired_direction.z += 1;
        direction = vec3_add(direction, world_view_get_forward());
    }
    // if (GetKeyPress(KEY_DOWN)) {
    // if (inport(PK_DOWN)) {
    if (input_if_key_pressed(InputKey_Down)) {
        // direction.z -= 1;
        desired_direction.z -= 1;
        direction = vec3_sub(direction, world_view_get_forward());
    }
    // if (inport(PK_LEFT)) {
    if (input_if_key_pressed(InputKey_Left)) {
        // if (GetKeyPress(KEY_LEFT)) {
        // direction.x -= 1;
        desired_direction.x -= 1;
        direction = vec3_sub(direction, world_view_get_right());
        player->face_left = true;
    }
    // if (inport(PK_RIGHT)) {
    if (input_if_key_pressed(InputKey_Right)) {
        // if (GetKeyPress(KEY_RIGHT)) {
        // direction.x += 1;
        desired_direction.x += 1;
        direction = vec3_add(direction, world_view_get_right());
        player->face_left = false;
    }
    player->desired_direction = desired_direction;
    return direction;
}

// collision detection
PlayerMoveType player_move_test_collision_move(Player* player, Vec3 direction) {
    int x = player->pos.x;
    int y = player->pos.y;
    int z = player->pos.z;
    // either is 1
    int dx = direction.x;
    int dz = direction.z;
    // hit
    if (world_if_block_ground(x + dx, y, z + dz)) {
        return PlayerMoveType_Hit;
    }
    if (!world_if_block_passable(x + dx, y, z + dz)) {
        return PlayerMoveType_Edge;
    }
    if (world_if_block_ground(x + dx, y - 1, z + dz)) {
        return PlayerMoveType_Move;
    }
    if (world_if_block_ground(x + dx, y - 2, z + dz)) {
        return PlayerMoveType_Fall;
    }
    return PlayerMoveType_Edge;
}

PlayerMoveType player_move_test_collision_jump(const Vec3 *pos, const Vec3 *direction) {
    int x = pos->x;
    int y = pos->y;
    int z = pos->z;
    // either or none is 1
    int dx = direction->x;
    int dz = direction->z;
    bool moved = dx || dz;
    // top
    if (!world_if_block_passable(x, y + 1, z)) {
        return world_if_block_ground(x, y + 1, z) ? PlayerMoveType_Hit : PlayerMoveType_Edge;
    }
    // target
    if (moved && !world_if_block_passable(x + dx, y + 1, z + dz)) {
        return world_if_block_ground(x + dx, y + 1, z + dz) ? PlayerMoveType_Hit : PlayerMoveType_Edge;
    }
    // landing
    if (moved && !world_if_block_ground(x + dx, y, z + dz)) {
        return PlayerMoveType_Move;
    }
    return moved ? PlayerMoveType_JumpMove : PlayerMoveType_Jump;
}

PlayerMoveType player_move_test_collision_jump(Player* player, Vec3 direction) {
    int x = player->pos.x;
    int y = player->pos.y;
    int z = player->pos.z;
    // either or none is 1
    int dx = direction.x;
    int dz = direction.z;
    bool moved = dx || dz;
    // top
    if (!world_if_block_passable(x, y + 1, z)) {
        return world_if_block_ground(x, y + 1, z) ? PlayerMoveType_Hit : PlayerMoveType_Edge;
    }
    // target
    if (moved && !world_if_block_passable(x + dx, y + 1, z + dz)) {
        return PlayerMoveType_Jump;
        // return world_if_block_ground(x + dx, y + 1, z + dz) ? PlayerMoveType_Hit : PlayerMoveType_Edge;
    }
    // landing
    if (moved && !world_if_block_ground(x + dx, y, z + dz)) {
        return PlayerMoveType_Jump;
        // return PlayerMoveType_Move;
    }
    return moved ? PlayerMoveType_JumpMove : PlayerMoveType_Jump;
}

PlayerState player_move_check_movement(Player* player, Vec3 direction, bool jump) {
    bool moved_x = direction.x != 0;
    bool moved_z = direction.z != 0;
    if (!moved_x && !moved_z && !jump) {
        return PlayerState_Default;
    }
    memset(&player->move_state, 0, sizeof(PlayerMoveState));
    if (jump) {
        player_move_try_jump(player, direction);
    }
    else {
        // no jump
        if (moved_x && moved_z) {
            // move diagnal
        }
        else {
            // move single direction
            player_move_try_move_single(player, direction);

        }
    }
    if (player->move_state.moves_num > 0) {
        return PlayerState_Moving; // TODO
    }
    return PlayerState_Default;
}

void player_move_try_move_single(Player* player, Vec3 direction) {
    PlayerMoveAtomic move;
    player_move_init_movement(&move);
    move.type = player_move_test_collision_move(player, direction);
    move.pos_start = player->pos;
    if (move.type == PlayerMoveType_Fall) {
        move.direction = Vec3{ direction.x, -1, direction.z };
        move.has_sound_end = true;
        move.sound_end = AudioSound_Fall;
    }
    else {
        move.direction = direction;
    }
    move.pos_target = vec3_add(player->pos, move.direction);
    move.time_moved = 0;
    move.init = false;
    player->move_state.moves[0] = move;
    player->move_state.curr_move_id = 0;
    player->move_state.moves_num = 1;
}

void player_move_try_jump(Player* player, Vec3 direction) {
    PlayerMoveType move_type = player_move_test_collision_jump(player, direction);
    if (move_type != PlayerMoveType_Jump
        && move_type != PlayerMoveType_JumpMove) {
        return;
    }
    PlayerMoveAtomic move;
    move.type = move_type;
    move.pos_start = player->pos;
    if (move_type == PlayerMoveType_JumpMove) {
        move.direction = Vec3{ direction.x, 2.5, direction.z };
    }
    else {
        move.direction = Vec3{ 0, 2.5, 0 };
    }
    move.pos_target = vec3_add(player->pos, move.direction);
    move.time_moved = 0;
    move.init = false;
    move.has_sound_start = true;
    move.sound_start = AudioSound_Jump;
    player->move_state.moves[0] = move;
    player->move_state.curr_move_id = 0;
    player->move_state.moves_num = 1;
}

// update
PlayerState player_move_update(Player* player) {
    PlayerMoveAtomic* move = &player->move_state.moves[player->move_state.curr_move_id];

    if (!move->init) {
        move->init = true;
        if (move->has_sound_start) {
            audio_play_sound(move->sound_start);
        }
    }

    move->time_moved += timer_get_delta_time(); // TODO
    float t = move->time_moved * timer_get_rhythm_step_inv() * 2.0f;
    t = clamp(0.0f, t, 1.0f);

    switch (move->type) {
    case PlayerMoveType_Move:
    {
        player_move_update_move(player, move, t);
        break;
    }
    case PlayerMoveType_Fall: {
        player_move_update_fall(player, move, t);
        break;
    }
    case PlayerMoveType_Jump:
    {
        player_move_update_jump(player, move, t);
        break;
    }
    case PlayerMoveType_JumpMove:
    {
        player_move_update_jump_move(player, move, t);
        break;
    }
    default: {
        // audio_play_sound(AudioSound_Fall);
        t = 1.0f;
        break;
    }
    }
    // next state
    if (t >= 1.0f) {
        if (move->has_sound_end) {
            audio_play_sound(move->sound_end);                                                              
        }

        player->move_state.curr_move_id++;
        if (player->move_state.curr_move_id >= player->move_state.moves_num) {
            // player_state_on_enter_default(player);
            return PlayerState_Default;
        }
    }
    return PlayerState_Moving;
}

void player_move_update_move(Player* player, const PlayerMoveAtomic* move, float t) {
    float t_move = t * t * t;
    if (move->direction.x) {
        player->pos.x = lerp(move->pos_start.x, move->pos_target.x, t_move);
    }
    if (move->direction.z) {
        player->pos.z = lerp(move->pos_start.z, move->pos_target.z, t_move);
    }
}

void player_move_update_fall(Player* player, const PlayerMoveAtomic* move, float t) {
    float t_move = t * t * t;
    if (move->direction.x) {
        player->pos.x = lerp(move->pos_start.x, move->pos_target.x, t_move);
    }
    if (move->direction.z) {
        player->pos.z = lerp(move->pos_start.z, move->pos_target.z, t_move);
    }
    player->pos.y = t_move > 0.75f ? move->pos_target.y : move->pos_start.y;
}

void player_move_update_jump(Player* player, PlayerMoveAtomic* move, float t) {
    if (t <= 0.5) {
        Vec3 direction = player_move_get_input_direction(player);
        if (direction.x != 0 || direction.z != 0) {
            PlayerMoveType move_type = player_move_test_collision_jump(&move->pos_start, &direction);
            if (move_type == PlayerMoveType_JumpMove) {
                move->type = PlayerMoveType_JumpMove;
                move->direction.x = direction.x;
                move->direction.z = direction.z;
                move->pos_target.x += direction.x;
                move->pos_target.z += direction.z;
                player_move_update_jump_move(player, move, t);
                return;
            }
        }
    }
    float t_jump = 1 - 4.0f * (t - 0.5f) * (t - 0.5f);
    player->pos.y = lerp(move->pos_start.y, move->pos_target.y, t_jump);
}

void player_move_update_jump_move(Player* player, const PlayerMoveAtomic* move, float t) {
    if (t > 0.853) {
        // on floor
        player->pos.y = move->pos_start.y + 1;
    }
    else {
        float t_jump = 1 - 4.0f * (t - 0.5f) * (t - 0.5f);
        player->pos.y = lerp(move->pos_start.y, move->pos_target.y, t_jump);
    }
    float t_move = t * t * t;
    //fprintf(stderr, "t = %.2f\n", move->time_moved);
    if (move->direction.x) {
        player->pos.x = lerp(move->pos_start.x, move->pos_target.x, t_move);
    }
    if (move->direction.z) {
        player->pos.z = lerp(move->pos_start.z, move->pos_target.z, t_move);
    }
}