#pragma once
#include "pixel.h"
//enum AttackAimState {
//    AttackAimState_Cooldown,
//    AttackAimState_Delay,
//    AttackAimState_Attack,
//};
//
//struct AttackAim {
//    AttackAimState state;
//    float state_start_time;
//    Vec3 aim;
//};
//
//struct AttackCountdown {
//    Vec3 aim;
//    float countdown;
//    Color color;
//};
//
//struct AttackAxis {
//    Vec3Axis axis;
//    int value;
//};

enum AttackFaceState {
    AttackFaceState_Idle,
    AttackFaceState_Countdown,
    AttackFaceState_Attack,
    AttackFaceState_Melt,
};

//#define ATTACK_FACE_NUM 10
//

struct AttackFace {
    // config
    int min_x;
    int min_z;
    int max_x;
    int max_z;
    // int init_time_milli;
    float init_time_offset;
    // special item
    bool item;
    int item_index;
    int item_x;
    int item_z;
    // wait for item
    bool item_wait;
    int item_wait_index;
    // set respawn
    bool respawn_set;
    int respawn_x;
    int respawn_z;

    // state
    AttackFaceState state;
    float state_start_time;
    Pixel pixel;
    Pixel pixel_item;
};

struct AttackFaceQueue {
    AttackFace attacks[25];
    int head_init;
    int head_active;
    int total;
    int time_milli;
};

void attack_queue_init();
void attack_queue_init_config(int num, const AttackFace* attacks);
void attack_queue_update();
void attack_queue_draw();

//inline void attack_update(AttackAim* attack) {
//    int rhythm_pos = timer_get_rhythm_position();
//    int frame = rhythm_pos % 4;
//    switch (attack->state) {
//    case AttackAimState_Cooldown: {
//        attack->state_start_time += timer_get_delta_time();
//        // if (attack->state_start_time > 1.0f) {
//        if (frame == 1) {
//            Player* player = player_get_players();
//            BlockInfo block_info;
//            world_get_block_info_below(&block_info, player->pos);
//            attack->aim = block_info.pos;
//            attack->state_start_time = 0;
//            attack->state = AttackAimState_Delay;
//        }
//        break;
//    }
//    case AttackAimState_Delay: {
//        attack->state_start_time += timer_get_delta_time();
//        // if (attack->state_start_time > 1.0f) {
//        if (frame == 3) {
//            attack->state_start_time = 0;
//            attack->state = AttackAimState_Attack;
//        }
//        break;
//    }
//    case AttackAimState_Attack: {
//        attack->state_start_time += timer_get_delta_time();
//        // if (attack->state_start_time > 1.0f) {
//        if (frame == 0) {
//            attack->state_start_time = 0;
//            attack->state = AttackAimState_Cooldown;
//        }
//        break;
//    }
//    }
//}
//
//inline void attack_draw(AttackAim* attack, Vec2 camera_offset) {
//    switch (attack->state) {
//    case AttackAimState_Delay: 
//    case AttackAimState_Attack: {
//        // Vec2 pos_screen = world_view_top_world2screen(attack->aim, camera_offset);
//        Coord2 pos_screen = world_view_top_get_cube_offset(
//            world_view_get_camera_top(),
//            floorf(attack->aim.x),
//            floorf(attack->aim.y),
//            floorf(attack->aim.z)
//        );
//        int rhythm_pos = timer_get_rhythm_position() * 2.0f;
//        int frame = rhythm_pos % 2;
//        if (attack->state == AttackAimState_Delay && !frame) {
//            // flash
//            return;
//        }
//        Color color = attack->state == AttackAimState_Delay ?
//            create_color(255, 255, 255) : create_color(255, 0, 0);
//        Pixel pixel = create_pixel(color);
//        for (int y = 0; y < 2; ++y) {
//            for (int x = 0; x < 4; ++x) {
//                draw_pixel(pos_screen.x + x, pos_screen.y + y, pixel);
//            }
//        }
//        
//        //fprintf(stderr, "aim world = %.2f %.2f %.2f; screen = %.1f %.1f\n",
//        //    attack->aim.x, attack->aim.y, attack->aim.z, pos_screen.x, pos_screen.y);
//        break;
//    }
//    }
//}
//
//inline void attack_countdown_update(AttackCountdown* attack) {
//    Player* player = player_get_players();
//    BlockInfo block_info;
//    world_get_block_info_below(&block_info, player->pos);
//    if (!block_info.block) {
//        return;
//    }
//    if (vec3_if_equal(&block_info.pos, &attack->aim)) {
//        attack->countdown += timer_get_delta_time();
//    }
//    else {
//        attack->aim = block_info.pos;
//        attack->countdown = 0;
//        attack->color = cube_get_top_color(&block_info);
//    }
//}
//
//inline void attack_countdown_draw(AttackCountdown* attack, Vec2 camera_offset);

