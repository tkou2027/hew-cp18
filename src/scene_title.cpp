#include "scene_title.h"
#include "scene.h"
#include "draw.h"
#include "world.h"
#include "world_view.h"
#include "timer.h"
#include "audio.h"
#include "item.h"
#include "audio.h"
#include "input.h"
#include <cmath>

static float g_rotation_countdown;
static Sprite g_title_sprite;
static bool g_start_pressed;
static float g_start_pressed_countdown;

void scene_title_init() {
    world_set_config(WorldConfigName_Title);
    // world_init_fall(24);
    // must have audio to use rhythm
    // start audio
    // TODO
    audio_play_bgm(AudioBgm_Title);

    sprite_load(&g_title_sprite, L"assets/title.txt");
    //const wchar_t* title_str = L" /|/|(_  /  /   /   |/__(__(";
    //sprite_init(&g_title_sprite, 14, 2, title_str, create_color(255, 255, 255));
    
    world_view_init();
    world_view_set_focus(Vec3{ 3.5, 0, 3.5 });
	world_view_ray_set_render_object(false);
    g_rotation_countdown = timer_get_rhythm_step() * 4;

    g_start_pressed = false;
}

void scene_title_update() {
    if (g_start_pressed) {
        g_start_pressed_countdown -= timer_get_delta_time();
        if (g_start_pressed_countdown <= 0) {
            world_view_ray_set_render_object(true);
            setScene(SCENE_GAME);
        }
    }
    // if (!g_start_pressed && inport(PK_ENTER)) {
    if (!g_start_pressed && input_if_key_triggered(InputKey_Confirm)) {
        g_start_pressed = true;
        g_start_pressed_countdown = timer_get_rhythm_step() * 4;
		// world_view_ray_set_render_object(true);
        // setScene(SCENE_GAME);
        audio_play_sound(AudioSound_Select);
    }

    g_rotation_countdown -= timer_get_delta_time();
    if (g_rotation_countdown <= 0) {
        g_rotation_countdown = timer_get_rhythm_step() * 4;
        world_view_start_rotation(1);
    }
    world_view_update();
}

void scene_title_draw() {
    
    // draw_sprite_alpha(Coord2{ 20, 6 }, &g_title_sprite);
    int offset_x = 24;
    int offset_y = 0;
    float rhythm_pos = timer_get_rhythm_position();
    int pos = (int)rhythm_pos % ITEM_SPECIAL_NUM;
    int pos_next = (pos + 1) % ITEM_SPECIAL_NUM;
    ItemSpecialInfo* palette = item_get_item_special_info();
    float t = clamp(0.0f, rhythm_pos - floorf(rhythm_pos), 1.0f);
    Color color_base = lerp(palette[pos].color, palette[pos_next].color, t * t);
    Color color_shade = create_color(5, 0, 141);
    for (int y = 0; y < g_title_sprite.height; ++y) {
        Color color = lerp(color_base, color_shade, (float)y / g_title_sprite.height);
        for (int x = 0; x < g_title_sprite.width; ++x) {
            Pixel* p = g_title_sprite.pixels + y * g_title_sprite.width + x;
            p->color_fore = color;
            draw_pixel_alpha(x + offset_x, y + offset_y, *p);
        }
        
    }
    // draw_string(20, 10, L"MELT", create_color(255, 255, 255), create_color(0, 0, 0));
    Color color_button_base = create_color(125, 125, 125);
    if (pos == 0) {
        float lerp_t = 1 - 4.0f * (t - 0.5f) * (t - 0.5f);
        color_button_base = lerp(color_button_base, create_color(255, 255, 255), lerp_t);
    }
    world_view_draw();
    draw_string(36, 18, L"PRESS ENTER", color_button_base, create_color(0, 0, 0));
    draw_string_fullwidth(36 + 12, 18, L"ÅZ", color_button_base, create_color(0, 0, 0));
}