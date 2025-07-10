#include "player.h"
#include "vec.h"
#include "timer.h"
#include "input.h"
#include "scene_game.h"
#include "world.h"
#include "world_view.h"
#include "world_view_top.h"
#include "cube.h"
#include "draw.h"

#include "item.h"

#include "player_move.h"

#include <cmath>
#include <memory.h>

static Player players[PLAYER_NUM];

// single palyer
// init
void player_init(Player* player);
void player_set_position(Player* player, const Vec3 *pos);
// draw
void player_draw(Player* player);
// update
void player_update(Player* player);
void player_update_default(Player* player);
void player_update_moving(Player* player);
// state transformation
void player_state_on_enter_default(Player* player);
void player_state_on_enter_movement(Player* player);
void player_state_on_enter_hit(Player* player);

// public start
Player* player_get_players() {
    return players;
}

void player_game_set_position(const Vec3* pos) {
    for (int i = 0; i < PLAYER_NUM; ++i) {
        player_set_position(players + i, pos + i);
    }
}
// game players
void player_game_init() {
    player_init(&players[0]);
}

void player_game_update() {
    player_update(&players[0]);
}

void player_game_draw() {
    player_draw(&players[0]);
}

// public end


// single player
void player_init(Player* player) {
    memset(player, 0, sizeof(Player));
    player->state = PlayerState_Default;
    player->respawn_x = 3;
    player->respawn_z = 3;
    // sprite_load(&player->sprite, L"assets/isoplayer.txt");
}

void player_set_position(Player* player, const Vec3 *pos) {
    player->pos.x = pos->x;
    player->pos.y = pos->y;
    player->pos.z = pos->z;
}

// hurt
void player_set_hurt(Player* player) {
    if (player->hurt) {
        return;
    }
    player->hurt = true;
    // TODO config
    player->hurt_countdown = timer_get_rhythm_step() * 2;
}

void player_update(Player* player) {
    switch (player->state) {
    case PlayerState::PlayerState_Default: {
        player_update_default(player);
        break;
    }
    case PlayerState_Moving: {
        player_update_moving(player);
        break;
    }
    case PlayerState_Hit: {
        // player_update_hit(player);
    }
    default: {
        break;
    }
    }

    // status
    
    // update current block and standing block
    // Vec3 round_position = player_move_round_position(&player->pos);
    player->block_x = (int)floorf(player->pos.x);
    player->block_y = (int)floorf(player->pos.y);
    player->block_z = (int)floorf(player->pos.z);
    player->block_stand_prev = player->block_stand;
    if (player->pos.y - player->block_y < 1e06) {
        world_get_visible_block_info_at(&player->block_stand,
            player->block_x, player->block_y - 1.0f, player->block_z);
    }
    else {
        player->block_stand.block = NULL;
    }

    // update hurt
    if (player->hurt) {
        player->hurt_countdown -= timer_get_delta_time();
        if (player->hurt_countdown <= 0) {
            player->hurt = false;
        }
    }
    //if (player->block_stand.block
    //    && player->block_stand.block->attack
    //    && player->block_stand.block->attack->state == AttackFaceState_Attack) {
    //    player_set_hurt(player);
    //}

    // update item
    if (player->item_gained) {
        player->item_countdown -= timer_get_delta_time();
        if (player->item_countdown <= 0) {
            player->item_gained = false;
        }
    }
    Item* item = item_map_get_item(player->block_x, player->block_y, player->block_z);
    if (item && item->type != ItemType_None) {
        player->item_gained = true;
        player->item = *item;
        player->item_countdown = timer_get_rhythm_step(); // TODO
        item_map_gain_item(player->block_x, player->block_y, player->block_z);
    }
}

void player_update_default(Player* player) {
    // TODO
    BlockInfo block_info;
    if (!world_get_visible_block_info_at(&block_info,
        floorf(player->pos.x), floorf(player->pos.y) - 1.0f, floorf(player->pos.z))) {
        player->pos.y -= 1.0f;
        if (player->pos.y <= -5) {
            // TODO
            // player->pos.y = 10;
            int respawn_y = world_get_map_height_at(player->respawn_x, player->respawn_z);
            player->pos = Vec3{ player->respawn_x + 0.5f, respawn_y + 5.0f, player->respawn_z + 0.5f};
        }
        return;
    }
    Vec3 direction = player_move_get_input_direction(player);

    bool jump = input_if_key_pressed(InputKey_Jump);

    if (input_if_key_pressed(InputKey_RotateLeft)) {
        world_view_start_rotation(-1);
    }
    if (input_if_key_pressed(InputKey_RotateRight)) {
        world_view_start_rotation(1);
    }

    // check movement attempt
    PlayerState next_state = player_move_check_movement(player, direction, jump);
    if (next_state == PlayerState_Moving) {
        player_state_on_enter_movement(player);
    }
}

void player_update_moving(Player* player) {
    PlayerState next_state = player_move_update(player);
    if (next_state == PlayerState_Default) {
        player_state_on_enter_default(player);
    }
}

void player_state_on_enter_hit(Player* player) {
    player->state = PlayerState_Hit;
}

void player_state_on_enter_movement(Player* player) {
    player->state = PlayerState_Moving;
    // memset(&player->move_state, 0, sizeof(PlayerMoveState));
    //player->movement = movement;
    //player->time_moved = 0;
    // player->pos_move_start = player->pos;
   //  player->pos_move_target = vec3_add(player->pos, movement);
}

void player_state_on_enter_default(Player* player) {
    player->state = PlayerState_Default;
    // snap to grid (0.5)
    player->pos = player_move_round_position(&player->pos);
    //player->pos.x = (int)player->pos.x;
    //player->pos.y = (int)player->pos.y;
    //player->pos.z = (int)player->pos.z;
}

// Draw
void player_draw_shadow(Player* player) {
    const int probs = 2;

    Vec3 right = world_view_get_right();
    Vec2 desired_offset{ player->desired_direction.x, -player->desired_direction.z };
    Vec3 shadow_prob_pos[probs] = {
        player->pos,
        vec3_add(player->pos, vec3_multiply(right, 0.2f))
    };
    BlockInfo block_info;

    for (int i = 0; i < probs; ++i) {
        //float player_z = world_view_top_get_z_value(world_view_get_camera_top()->dir,
        //    floor(player->pos.x),
        //    floor(player->pos.z));
        //world_get_block_info_below(&block_info, shadow_prob_pos[i]);

        float player_z = world_view_top_get_z_value(world_view_get_camera_top()->dir,
            floor(player->pos.x),
            floor(player->pos.z));

        float player_pixel_x = player->pos_screen.x + i + desired_offset.x;
        float player_pixel_y = player->pos_screen.y + desired_offset.y;
        // float player_z = buffer_z_get_at(player_pixel_x, player_pixel_y );

        Vec3 player_world_pos = world_view_top_screen2world(world_view_get_camera_top(),
            Vec2{ player_pixel_x + 0.125f, player_pixel_y + (desired_offset.y < 0 ? 1.1f : 1.0f) }, round(player->pos.y));
        world_get_block_info_below(&block_info, player_world_pos);

        if (block_info.block) {
            Color shadow_color = cube_get_shadow_color(&block_info);
            Pixel shadow_pixels[2] = {
                create_pixel(L'\u2584', shadow_color), // top
                create_pixel(L'\u2580', shadow_color) // down
            };
            player_world_pos.y = block_info.pos.y + 1;
            Coord2 shadow_pos_screen = coord2_round(
                world_view_top_world2screen(world_view_get_camera_top(), player_world_pos)
            );

            for (int y = 0; y < 2; ++y) {
                int pixel_x = player_pixel_x;
                int pixel_y = shadow_pos_screen.y - 1 + y;
                float  depth_z = buffer_z_get_at(pixel_x, pixel_y);
                bool visible = depth_z >= player_z;
                if (depth_z > player_z) { // fix flash
                    Vec3 forward = world_view_get_forward();
                    visible = !world_get_visible_block_at(block_info.pos.x - forward.x, block_info.pos.y + 1, block_info.pos.z - forward.z)
                        && world_get_visible_block_at(block_info.pos.x - forward.x, block_info.pos.y, block_info.pos.z - forward.z);
                }
                if (visible) {
                    draw_pixel_alpha(pixel_x, pixel_y, shadow_pixels[y]);
                }
            }
        }
    }
    
}

Color player_get_color(Player* player) {
    Color player_color = // player->hurt ? create_color(255, 255, 255) :
        player->item_gained ? item_get_color(&player->item, true) : create_color(0, 0, 255);
    return player_color;
}

void player_draw(Player* player) {
    player->pos_screen = coord2_round(
        world_view_top_world2screen(world_view_get_camera_top(), player->pos)
    );
    // fprintf(stderr, "world = %.2f %.2f %.2f; screen = %d %d\n",
    //    player->pos.x, player->pos.y, player->pos.z, player->pos_screen.x, player->pos_screen.y);
    // draw_offset
    player->pos_screen.x -= 1; // half width
    player->pos_screen.y -= 1; // height

    player_draw_shadow(player);

    Color player_color = player_get_color(player);
    Pixel pixel_body = create_pixel(player_color);
    Pixel pixel_head = create_pixel(L'"', create_color(255, 255, 255), player_color);
    // draw_sprite(player->pos_screen, &player->sprite);
    
    float player_z = world_view_top_get_z_value(world_view_get_camera_top()->dir,
        floor(player->pos.x),
        floor(player->pos.z));
    //Vec2 desired_offset{ 0, 0 };
    //if (player->state == PlayerState_Default) {
    //    desired_offset.x = player->desired_direction.x;
    //    desired_offset.y = -player->desired_direction.z;
    //}
    Vec2 desired_offset{ player->desired_direction.x, -player->desired_direction.z };

    float pos = timer_get_rhythm_position() * 0.25;
    int frame = (int)(pos * 12) % 12;
    
    for (int i = 0; i < 2; ++i) {
        int pixel_x = player->pos_screen.x + i + desired_offset.x;
        int pixel_y = player->pos_screen.y + desired_offset.y;
        float depth_z = buffer_z_get_at(pixel_x, pixel_y);
        bool visible = depth_z <= player_z;
        if (depth_z == player_z + 1 && desired_offset.y == 1) { // fix flash
            Vec3 forward = world_view_get_forward();
            visible = !world_get_visible_block_at(floorf(player->pos.x) - forward.x, player->pos.y, floorf(player->pos.z) - forward.z);
        }
        if (visible) {
            bool face = player->face_left && i == 0 || !player->face_left && i == 1;
            bool blink = frame == 2 || frame == 4;
            draw_pixel(pixel_x, pixel_y, face && !blink ? pixel_head : pixel_body);
        }
        else {
            Pixel blocked = create_pixel(L'\u2593', player_color);
            draw_pixel_alpha(pixel_x, pixel_y, blocked);
        }

    }
    //draw_pixel(player->pos_screen.x, player->pos_screen.y, player->face_left ? pixel_head : pixel_body);
    //draw_pixel(player->pos_screen.x + 1, player->pos_screen.y, player->face_left ? pixel_body : pixel_head);
}


