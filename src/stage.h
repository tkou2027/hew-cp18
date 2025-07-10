#pragma once
#include "vec.h"
#include "world.h"
#include "player.h"
#include "item.h"
// #include "world_view_top.h"

enum StageName {
    StageName_None,
    // stage
    StageName_Stage0,
    StageName_Stage1,
    StageName_Stage2,
    StageName_Num
};

enum StageState {
    StageState_Idle,
    StageState_TransIn,
    StageState_Play,
    StageState_TransOut
};

enum StageEndingState {
    StageEndingState_None,
    StageEndingState_ItemDone,
    StageEndingState_EndingBlockWait,
    StageEndingState_EndingCountdown
};

//// camera settings
//struct Camera {
//    Coord2 center;
//    Vec3 focus;
//};
//

struct Stage {
    StageName name;
    // stage settings
    WorldConfigName world_config;
    // trans in
    Vec3 player_pos_init_in[PLAYER_NUM]; // TOOD 2 players?
    Vec3 camera_init_in;
    // play
    Vec3 player_pos_init_play[PLAYER_NUM];
    Vec3 camera_init_play;

    // stage state
    StageState state;
    // camera
    Vec3 camera_focus;
    // ending
    StageEndingState ending_state;
    float ending_countdown;
    Vec3 ending_pos;
    Pixel ending_pixel;
    StageName next;
    
};

void stage_game_init();
void stage_game_enter(StageName stage);
void stage_game_update();
void stage_game_draw();
bool stage_game_if_ended();