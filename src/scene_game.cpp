#pragma once
// world
#include "world.h"
#include "world_view.h"

#include "stage.h"
#include "attack.h"

// player
#include "player.h"

#include "audio.h"
#include "animation.h"

#include "scene.h"

void scene_game_init() {
    // world
    // world_init();
    world_view_init();
    world_view_ray_set_render_object(true);;

    // player
    player_game_init();
    // reset
    stage_game_init();

    // start audio and set rhythm
    audio_play_bgm(AudioBgm_Game);

    stage_game_enter(StageName_Stage0);
}

void scene_game_update() {
    stage_game_update();
    if (stage_game_if_ended()) {
        setScene(SCENE_ENDING);
    }
}

void scene_game_draw() {
    stage_game_draw();
    // test rhythm
    // draw_animation_test();
    return;
}