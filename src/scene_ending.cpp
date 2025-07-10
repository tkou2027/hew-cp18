#include "scene_ending.h"
#include "scene.h"
#include "draw.h"
#include "world.h"
#include "world_view.h"
#include "audio.h"
#include "timer.h"
#include "input.h"
#include "timer.h"
#include "item.h"
#include <cmath>

static float g_rotation_countdown;

void scene_ending_init() {
    world_set_config(WorldConfigName_Title);
    audio_play_bgm(AudioBgm_Title);

    world_view_init();
    world_view_set_focus(Vec3{ 3.5, 0, 3.5 });
    world_view_ray_set_render_object(false);
    g_rotation_countdown = timer_get_rhythm_step() * 4;
}
void scene_ending_update() {
    if (input_if_key_triggered(InputKey_Confirm)) {
        setScene(SCENE_TITLE);
    }
    g_rotation_countdown -= timer_get_delta_time();
    if (g_rotation_countdown <= 0) {
        g_rotation_countdown = timer_get_rhythm_step() * 4;
        world_view_start_rotation(1);
    }
    world_view_update();
}
void scene_ending_draw() {
    world_view_draw();
    draw_string_fullwidth(20, 3, L"ƒvƒŒƒC‚µ‚Ä‚­‚ê‚Ä‚ ‚è‚ª‚Æ‚¤I", create_color(125, 125, 125), create_color(0, 0, 0));
    float rhythm_pos = timer_get_rhythm_position();
    int pos = (int)rhythm_pos % ITEM_SPECIAL_NUM;
    Color color_button_base = create_color(125, 125, 125);
    float t = clamp(0.0f, rhythm_pos - floorf(rhythm_pos), 1.0f);
    if (pos == 0) {
        float lerp_t = 1 - 4.0f * (t - 0.5f) * (t - 0.5f);
        color_button_base = lerp(color_button_base, create_color(255, 255, 255), lerp_t);
    }
    draw_string(20, 18, L"PRESS ENTER", color_button_base, create_color(0, 0, 0));
    draw_string_fullwidth(20 + 12, 18, L"Z", color_button_base, create_color(0, 0, 0));
    draw_string_fullwidth(20, 20, L"Œø‰Ê‰¹@–‚‰¤°@‚e‚b‰¹Hê", create_color(125, 125, 125), create_color(0, 0, 0));
    draw_string(20, 21, L"BGM incompetech.com", create_color(125, 125, 125), create_color(0, 0, 0));
}