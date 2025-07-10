#include "stage.h"
#include "stage_config.h"
// screen
#include "draw.h"
// game components
#include "world.h"
#include "world_view.h"
#include "world_view_top.h"
#include "player.h"
#include "item.h"
#include "attack.h"
#include "timer.h"
#include "vec.h"
#include <cmath>
#include <memory.h> // memset
#include <stdlib.h>

static Stage g_stages[StageName_Num];
static StageName g_stage_curr;
static StageName g_stage_next;

// stage_ending
static bool g_game_end;
//static float g_end_countdown;

void stage_update(Stage* stage);
void stage_draw(Stage* stage);
void stage_destroy(Stage* stage);
void stage_update_trans_in(Stage* stage);
void stage_update_play(Stage* stage);
void stage_update_trans_out(Stage* stage);
void stage_draw_trans_in(Stage* stage);
void stage_draw_play(Stage* stage);
void stage_draw_trans_out(Stage* stage);
void stage_state_on_enter_trans_in(Stage* stage);
void stage_state_on_enter_play(Stage* stage);
void stage_state_on_enter_trans_out(Stage* stage);
// ending
void stage_set_ending_block(Stage* stage);
bool stage_test_ending_block(Stage* stage);
void stage_update_play_ending(Stage* stage);

void stage_init_stage(Stage* stage,
    Vec3 player_pos_init_in,
    Vec3 camera_init_in,
    Vec3 player_pos_init_play,
    Vec3 camera_init_play
) {
    stage->player_pos_init_in[0] = player_pos_init_in;
    stage->camera_init_in = camera_init_in;
    stage->player_pos_init_play[0] = player_pos_init_in;
    stage->camera_init_play = camera_init_in;
}

// factory
void stage_game_init() {
    g_game_end = false;
    g_stage_curr = StageName_None;
    memset(g_stages, 0, sizeof(Stage) * StageName_Num);
    //int screen_width_half = buffer_get_width() / 2;
    //int screen_height_half = buffer_get_height() / 2;

    for (int i = 1; i < StageName_Num; ++i) {
        g_stages[i].name = (StageName)i;
        stage_config_init(g_stages + i);
    }
}

void stage_game_enter(StageName stage) {
    //if (stage == g_stage_curr) {
    //    return;
    //}
    // clean up item map
    stage_destroy(&g_stages[g_stage_curr]);
    g_stages[g_stage_curr].state = StageState_Idle;
    g_stage_curr = stage;
    stage_state_on_enter_trans_in(&g_stages[g_stage_curr]);
}

void stage_game_update() {
    stage_update(&g_stages[g_stage_curr]);
}

void stage_game_draw() {
    stage_draw(&g_stages[g_stage_curr]);
}

// single stage
void stage_init(Stage* stage) {
    stage->state = StageState_Idle;
}

void stage_update(Stage* stage) {
    switch (stage->state) {
    case StageState_TransIn: {
        stage_update_trans_in(stage);
        break;
    }
    case StageState_Play: {
        stage_update_play(stage);
        break;
    }
    case StageState_TransOut: {
        stage_update_trans_out(stage);
        break;
    }
    default: {
        break;
    }
    }
}

void stage_draw(Stage* stage) {
    switch (stage->state) {
    case StageState_TransIn: {
        stage_draw_trans_in(stage);
        break;
    }
    case StageState_Play: {
        stage_draw_play(stage);
        break;
    }
    case StageState_TransOut: {
        stage_draw_trans_out(stage);
        break;
    }
    default: {
        break;
    }
    }
}

// transform in
// trans in init
void stage_state_on_enter_trans_in(Stage* stage) {
     audio_play_bgm(AudioBgm_Game);
     // TODO test map
     //stage_state_on_enter_play(stage);
     //return;

    stage->state = StageState_TransIn;
    stage->ending_state = StageEndingState_None;
    // init
    world_set_config(stage->world_config);
    // init item map
    item_map_init();

    // player
    // init player pos
    player_game_set_position(stage->player_pos_init_in);
    // camera
    // init camera focus
    stage->camera_focus = stage->camera_init_in;
    world_view_init(); // resset direction
    world_view_set_focus(stage->camera_focus);

    world_init_fall(22);
}
// trans in update
void stage_update_trans_in(Stage* stage) {
   
    // TODO test map
     //stage_state_on_enter_play(stage);
     //return;
    // TODO trans in animation
    // stage_state_on_enter_play(stage);
    bool update = false;
    BlockInfo block_info;
    for (int y = 1; y < world_get_height(); ++y) {
        for (int z = 0; z < world_get_depth(); ++z) {
            for (int x = 0; x < world_get_width(); ++x) {
                if (world_get_visible_block_info_at(&block_info, x, y, z)) {
                    Block* block = block_info.block;
                    if (block->status == BlockStatus_Fall) {
                        block->offset_y -= 1.0f * (x * 0.5 + z * 0.5 + (10.0 - y) + (22 - block->offset_y) / 22.0f) * timer_get_delta_time();
                        if (block->offset_y <= 0) {
                            block->offset_y = 0;
                            block->status = BlockStatus_Active;
                        }
                        update = true;
                    }
                }
            }
        }
    }
    if (!update) {
        stage_state_on_enter_play(stage);
    }
}
// trans in draw
void stage_draw_trans_in(Stage* stage) {
    world_view_draw();
}

// play
// play init
void stage_state_on_enter_play(Stage* stage) {
    stage->state = StageState_Play;
    // player
    // init player pos
    player_game_set_position(stage->player_pos_init_play);

    // camera
    // init camera focus
    stage->camera_focus = stage->camera_init_play;
    world_view_set_focus(stage->camera_focus);

    // TODO multiple stages
    // attack_queue_init();
	// stage items and attacks
	stage_config_init_actions(stage);
}
// play update
void stage_update_play(Stage* stage) {
	world_view_update();
    // if (!world_view_if_rotating()) {
    if (
        !world_view_if_rotating() &&
        stage->ending_state != StageEndingState_EndingCountdown) {
        item_map_update();
        player_game_update();
        attack_queue_update();
    }

    stage_update_play_ending(stage);

    //Player* player = player_get_players();
    // player status
    // 
    // item TODO
    //Item *item = item_map_get_item(player->block_x, player->block_y, player->block_z);
    //if (item && item->type != ItemType_None) {
    //    item->type = ItemType_None;
    //    if (stage->item_index_curr < stage->item_num - 1) {
    //        ++stage->item_index_curr;
    //        ItemConfig item = stage->item_list[stage->item_index_curr];
    //        item_map_set_item(item.type, item.pos.x, item.pos.y, item.pos.z);
    //    }
    //}

}
// play draw
void stage_draw_play(Stage* stage) {
    //world_view_set_focus(stage->camera_focus);
    world_view_draw();

    if (!world_view_if_rotating()) {
        attack_queue_draw();

        item_map_draw();

        player_game_draw();
    }

    item_ui_draw();
}

// transform out
void stage_state_on_enter_trans_out(Stage* stage) {
    stage->state = StageState_TransOut;
    item_ui_ending_init(); // score
}
void stage_update_trans_out(Stage* stage) {
    item_ui_ending_update();
    if (item_ui_ending_if_done()) {
        // TODO block fall and score page
        if (item_ui_ending_if_retry()) {
            item_ui_ending_init(); // score
            stage_game_enter(stage->name);
        }
        else if (stage->next != StageName_None) {
            item_ui_ending_init(); // score
            stage_game_enter(stage->next);
        }
        else {
            item_ui_ending_init(); // score
            g_game_end = true;
        }
        return;
    }

}
void stage_draw_trans_out(Stage* stage) {
    // TODO
    world_view_draw();
    player_game_draw();
    item_ui_draw();
}

void stage_destroy(Stage* stage) {
    item_map_destroy();
}

void stage_update_ending_pixel(Stage* stage) {
    float rhythm_pos = timer_get_rhythm_position() * 2;
    int pos = (int)rhythm_pos % ITEM_SPECIAL_NUM;
    int pos_next = (pos + 1) % ITEM_SPECIAL_NUM;
    ItemSpecialInfo* palette = item_get_item_special_info();
    float t = clamp(0.0f, rhythm_pos - floorf(rhythm_pos), 1.0f);
    stage->ending_pixel.text = L'#';
    stage->ending_pixel.color_fore = lerp(palette[pos].color, palette[pos_next].color, t * t);
}

void stage_set_ending_block(Stage *stage) {
    int x = stage->ending_pos.x;
    int z = stage->ending_pos.z;
    int y = world_get_map_height_at(x, z);
    stage->ending_pos.y = y;
    BlockInfo block_info;
    if (world_get_visible_block_info_at(
        &block_info,
        x, world_get_map_height_at(x, z), z
    )) {
        block_info.block->is_ending = true;
        stage_update_ending_pixel(stage);
        block_info.block->ending_pixel = &stage->ending_pixel;
    }
}

bool stage_test_ending_block(Stage* stage) {
    Player* player = player_get_players();
    return player->state == PlayerState_Default
        && player->block_x == stage->ending_pos.x
        && player->block_y == stage->ending_pos.y + 1
        && player->block_z == stage->ending_pos.z;
}

void stage_update_play_ending(Stage* stage) {
    switch (stage->ending_state) {
    case StageEndingState_None: {
        if (item_if_done()) {
            stage->ending_state = StageEndingState_ItemDone;
            stage->ending_countdown = timer_get_rhythm_step() * 2;
        }
        break;
    }
    case StageEndingState_ItemDone: {
        stage->ending_countdown -= timer_get_delta_time();
        if (stage->ending_countdown <= 0) {
            stage_set_ending_block(stage);
            stage->ending_state = StageEndingState_EndingBlockWait;
        }
        break;
    }
    case StageEndingState_EndingBlockWait: {
        stage_update_ending_pixel(stage);
        if (stage_test_ending_block(stage)) {
            stage->ending_state = StageEndingState_EndingCountdown;
            Player* player = player_get_players();
            player->desired_direction.x = 0;
            player->desired_direction.z = 0;
            // TODO play sound
            stage->ending_countdown = timer_get_rhythm_step() * 2;
        }
        break;
    }
    case StageEndingState_EndingCountdown: {
        stage_update_ending_pixel(stage);
        stage->ending_countdown -= timer_get_delta_time();
        if (stage->ending_countdown <= 0) {
            stage_state_on_enter_trans_out(stage);
        }
        break;
    }
    }
}

bool stage_game_if_ended() {
	return g_game_end;
}