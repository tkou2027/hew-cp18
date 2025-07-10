#include "item.h"
#include "world.h"
#include "world_view.h"
#include "world_view_top.h"
#include "sprite.h"
#include "draw.h"
#include "timer.h"
#include "audio.h"
#include "input.h"
#include <stdlib.h> // calloc free
#include <cmath>
#include <memory.h>
#include <wchar.h> // swprintf

// item map
static Item** g_item_map;
static int g_map_height;
static int g_map_width;
static int g_map_depth;
static int g_map_size;
static int g_map_width_depth;

// item list
static int g_item_special_num;
static Item g_item_special_list[ITEM_SPECIAL_NUM]; // TODO
static int g_item_special_combo_tail;
static ItemSpecialInfo g_special_items_info[ITEM_SPECIAL_NUM];

// ui
static Pixel pixel_horizontal;
static Pixel pixel_corner;
static Pixel pixel_vertical;
// ui ending
static bool g_item_ui_ending;
static bool g_item_ui_ending_score;
static bool g_item_ui_ending_done;
static float g_item_ui_ending_countdown;
static int g_item_ui_ending_index;
static bool g_item_ui_ending_retry;

// init constants
void item_init() {
    g_item_map = NULL;

    g_item_special_num = 0; // TODO
    memset(g_special_items_info, 0, sizeof(ItemSpecialInfo) * ITEM_SPECIAL_NUM);
    for (int i = 0; i < ITEM_SPECIAL_NUM; ++i) {
        g_special_items_info[i].index = i;
        g_item_special_list[i].type = ItemType_Special;
        g_item_special_list[i].index = i;
        g_item_special_list[i].is_small = true;
    }
    g_special_items_info[0].color = create_color(255, 0, 0);
    g_special_items_info[1].color = create_color(255, 127, 0);
    g_special_items_info[2].color = create_color(255, 255, 0);
    g_special_items_info[3].color = create_color(0, 255, 0);
    g_special_items_info[4].color = create_color(0, 156, 255);
    g_special_items_info[5].color = create_color(255, 0, 127);
    g_special_items_info[6].color = create_color(75, 0, 130);

    pixel_horizontal = create_pixel(L'-', create_color(0, 0, 0));
    pixel_vertical = create_pixel(L'|', create_color(0, 0, 0));
    pixel_corner = create_pixel(L'+', create_color(0, 0, 0));
}

// init for stage
void item_map_init() {
    // init map
    g_map_width = world_get_width();
    g_map_height = world_get_height();
    g_map_depth = world_get_depth();
    g_map_width_depth = g_map_width * g_map_depth;
    g_map_size = g_map_height * g_map_width_depth;
    g_item_map = (Item**)calloc(g_map_size, sizeof(Item));
    // init special item status
    for (int i = 0; i < ITEM_SPECIAL_NUM; ++i) {
        Item* item = g_item_special_list + i;
        item->status = ItemStatus_Idle;
		item->next_expected = false;
		item->combo_start = false;
		item->combo_countdown = 0;
    }
	g_item_ui_ending = false;
	g_item_ui_ending_done = false;
}

void item_map_init_special_items(int num, const Coord2 *offsets) {
    g_item_special_num = num;
	g_item_special_combo_tail = -1;
    for (int i = 0; i < num; ++i) {
        Item* item = g_item_special_list + i;
        item->offset_right = offsets[i].x;
        item->offset_forward = offsets[i].y;
		item->next_expected = i == 0;
        item->spot_status = ItemSpotStatus_Idle;
    }
}

void item_map_update() {
    // special item fall
    for (int i = 0; i < ITEM_SPECIAL_NUM; ++i) {
        Item* item = g_item_special_list + i;
        if (item->status == ItemStatus_Generated) {
            int x = floorf(item->pos.x);
            int y = floorf(item->pos.y);
            int z = floorf(item->pos.z);
            if (!world_get_visible_block_info_at(&item->block_standing, x, y - 1, z)) {
                item_map_set_item(NULL, x, y, z);
                item->pos.y -= 1.0f;
                if (item->pos.y < 0) {
                    item->status = ItemStatus_Dead;
                }
                else {
                    item_map_set_item(item, x, y - 1, z);
                }
            }
        }
		if (item->combo_countdown > 0) {
			item->combo_countdown = fmaxf(0, item->combo_countdown - timer_get_delta_time());
		}

        if (item->spot_countdown > 0) {
            item->spot_countdown = fmaxf(0, item->spot_countdown - timer_get_delta_time());
            if (item->spot_status == ItemSpotStatus_WaitSpotted) {
                if (item->spot_countdown == 0 || item->status == ItemStaus_Gained) {
                    item->spot_status = ItemSpotStatus_Failed;
                }
            }
        }
    }
}

void item_map_draw() {
    // draw special items
    for (int i = 0; i < ITEM_SPECIAL_NUM; ++i) {
        Item* item = g_item_special_list + i;
        if (item->status == ItemStatus_Generated) {
            item_draw(item);
        }
    }
}

void item_map_destroy() {
    free(g_item_map);
    g_item_map = NULL;
}

// item map manipulations
void item_map_set_item(Item* item, int x, int y, int z) {
    if (x >= 0 && x < g_map_width
        && y >= 0 && y < g_map_height
        && z >= 0 && z < g_map_depth) {
        int i = y * g_map_width_depth + z * g_map_width + x;
        g_item_map[y * g_map_width_depth + z * g_map_width + x] = item;
    }
}

void item_map_set_item_special(int index, int x, int y, int z) {
    if (x >= 0 && x < g_map_width
        && y >= 0 && y < g_map_height
        && z >= 0 && z < g_map_depth) {
        Item* item = &g_item_special_list[index];
        g_item_map[y * g_map_width_depth + z * g_map_width + x] = item;
        item->status = ItemStatus_Generated;
        item->pos = Vec3{ (float)x + 0.5f, (float)y, (float)z + 0.5f };
    }
}

bool item_map_gain_item(int x, int y, int z) {
	Item* item = item_map_get_item(x, y, z);
	if (!item) {
		return false;
	}
	item->status = ItemStaus_Gained;
	// combo
	item->combo_start = item->index == 0 || g_item_special_combo_tail != item->index - 1;
	if (item->index > 0 && g_item_special_combo_tail == item->index - 1) {
		item->combo_countdown = timer_get_rhythm_step();
	}
	g_item_special_combo_tail = item->index;
	audio_play_sound(item->next_expected ? AudioSound_ItemGain : AudioSound_ItemGainBad);
	// update expected
	item->next_expected = false;
	for (int i = item->index; i < g_item_special_num && i < ITEM_SPECIAL_NUM; ++i) {
		if (g_item_special_list[i].status == ItemStatus_Generated
			|| g_item_special_list[i].status == ItemStatus_Idle) {
			g_item_special_list[i].next_expected = true;
			break;
		}
	}
    item_map_set_item(NULL, x, y, z);
}

Item* item_map_get_item(int x, int y, int z) {
    if (x >= 0 && x < g_map_width
        && y >= 0 && y < g_map_height
        && z >= 0 && z < g_map_depth) {
        return g_item_map[y * g_map_width_depth + z * g_map_width + x];
    }
    return NULL;
}
// item map manipulations end

Color item_get_color(Item* item, bool base_color_only) {
    if (item->type == ItemType_Special) {
        Color base_color = g_special_items_info[item->index].color;
        if (base_color_only) {
            return base_color;
        }
        float rhythm_pos = timer_get_rhythm_position() * (item->next_expected ? 1.0f : 2.0f);
        int pos = (int)rhythm_pos % ITEM_SPECIAL_NUM;
        //int frame = pos % frames;
        if (!item->next_expected && pos != item->index) {
            return base_color;
        }
        float t = clamp(0.0f, rhythm_pos - floorf(rhythm_pos), 1.0f);
        Color color_light = create_color(255, 255, 255);
		float lerp_t = 1 - 4.0f * (t - 0.5f) * (t - 0.5f);
        return lerp(base_color, color_light, lerp_t);
    }
    return create_color(211, 211, 211);
}

Pixel item_get_pixel(Item* item) {
	Color item_color = item_get_color(item);
	bool has_block_below = item->block_standing.block; //block_info.block;
	return create_pixel(L'\u2582', has_block_below ? cube_get_shadow_color(&item->block_standing) : create_color(0, 0, 0), item_color);
}

void item_update_spot(Item* item) {
    if (
        (item->spot_status == ItemSpotStatus_Idle || item->spot_status == ItemSpotStatus_WaitSpotted)) {
        audio_play_sound(AudioSound_Spot);
        item->spot_status = ItemSpotStatus_Spotted;
        item->spot_countdown = timer_get_rhythm_step();
    }
}

// draw a single item
void item_draw_small(Item* item) {
    // draw item
    // shadow
    // BlockInfo block_info;
    // world_get_block_info_below(&block_info, item->pos);
    // item
    Coord2 screen_pos = coord2_round(
        world_view_top_world2screen(
            world_view_get_camera_top(),
            Vec3{
                item->pos.x,
                item->pos.y,
                item->pos.z
            }
        )
    );
    // offset
    Vec3 forward = world_view_get_forward();
    Vec3 right = world_view_get_right();
    float factor_right = right.x * item->offset_right + right.z * item->offset_forward; 
	float factor_forward = forward.x * item->offset_right + forward.z * item->offset_forward;
    screen_pos.x += -0.5f + 0.5f * factor_right; // [-1, 1] -> [-1, 0]
	screen_pos.y += -0.5f - 0.5f * factor_forward; // [-1, 1] -> [0, -1]

    // pixel
    Pixel item_pixel = item_get_pixel(item);

    // z-buffer
    float depth_z = buffer_z_get_at(screen_pos.x, screen_pos.y);
    float item_z = world_view_top_get_z_value(world_view_get_camera_top()->dir,
        floor(item->pos.x),
        floor(item->pos.z));
    bool spotted = depth_z <= item_z;
    // draw
    if (spotted) {
        
        draw_pixel(screen_pos.x, screen_pos.y, item_pixel);
        //if (item->next_expected) {
        //    draw_pixel_alpha(screen_pos.x, screen_pos.y - 1, create_pixel(L'+', item_pixel.color_back));
        //    draw_pixel_alpha(screen_pos.x - 1, screen_pos.y, create_pixel(L'+', item_pixel.color_back));
        //    draw_pixel_alpha(screen_pos.x + 1, screen_pos.y, create_pixel(L'+', item_pixel.color_back));
        //    draw_pixel_alpha(screen_pos.x, screen_pos.y + 1, create_pixel(L'+', item_pixel.color_back));
        //}
    }
    if (spotted &&
        (item->spot_status == ItemSpotStatus_Idle || item->spot_status == ItemSpotStatus_WaitSpotted)) {
        audio_play_sound(AudioSound_Spot);
        item->spot_status = ItemSpotStatus_Spotted;
        item->spot_countdown = timer_get_rhythm_step();
    }
    else if (!spotted && item->spot_status == ItemSpotStatus_Idle) {
        audio_play_sound(AudioSound_ItemSpawn);
        item->spot_status = ItemSpotStatus_WaitSpotted;
        item->spot_countdown = timer_get_rhythm_step() * 8;
    }
}

void item_draw(Item* item) {
    if (item->is_small) {
        item_draw_small(item);
        return;
    }
    float item_z = world_view_top_get_z_value(world_view_get_camera_top()->dir,
        floor(item->pos.x),
        floor(item->pos.z));
    BlockInfo block_info;
    world_get_block_info_below(&block_info, item->pos);

    // draw shadow    
    //Vec3 shadow_prob_pos = item->pos;
    //if (block_info.block) {
    //    Coord2 shadow_pos_screen = coord2_round(
    //        world_view_top_world2screen(
    //            world_view_get_camera_top(), shadow_prob_pos
    //        )
    //    );
    //    Pixel shadow_pixels[2];
    //    Color shadow_color = cube_get_shadow_color(&block_info);
    //    shadow_pixels[0] = create_pixel(item->type == ItemType_Special ? L'\u2580' : L'\u259D', shadow_color); // top right
    //    shadow_pixels[1] = create_pixel(item->type == ItemType_Special ? L'\u2580' : L'\u2598', shadow_color); // top left
    //    for (int x = 0; x < 2; ++x) {
    //        int pixel_x = shadow_pos_screen.x + x - 1;
    //        int pixel_y = shadow_pos_screen.y;
    //        float depth_z = buffer_z_get_at(pixel_x, pixel_y);
    //        if (depth_z <= item_z) {
    //            draw_pixel_alpha(pixel_x, pixel_y, shadow_pixels[x]);
    //        }
    //    }
    //}

    // draw item
    Coord2 screen_pos = coord2_round(
        world_view_top_world2screen(
            world_view_get_camera_top(),
            Vec3{
                item->pos.x,
                item->pos.y,
                item->pos.z
            }
        )
    );
    screen_pos.x -= 0.5 - 0.5 * (world_view_get_forward().x + world_view_get_forward().z);
    screen_pos.y -= 0.5 - 0.5 * (world_view_get_right().x + world_view_get_right().z);
    Pixel item_pixels[2];
    Color item_color = create_color(255, 255, 0);
    if (item->type == ItemType_Special) {
        item_color = g_special_items_info[item->index].color;
    }
    bool has_block_below = block_info.block;
    item_pixels[0] = create_pixel(item->type == ItemType_Special ? L'\u2583m' : L'\u2597', has_block_below ? cube_get_shadow_color(&block_info) : create_color(0, 0, 0), item_color); // top right
    item_pixels[1] = create_pixel(ItemType_Special ? L'\u2583' : L'\u2596', item_color); // top left
    for (int i = 0; i < 1; ++i) {
        int pixel_x = screen_pos.x + i;
        int pixel_y = screen_pos.y;
        float depth_z = buffer_z_get_at(pixel_x, pixel_y);
        if (depth_z <= item_z) {
            draw_pixel(pixel_x, pixel_y, item_pixels[i]);
        }
    }
    // draw_sprite_alpha(screen_pos, &g_item_sprites[item->type]);
}

void item_ui_draw_horizontal(int offset_x, int offset_y, Color color) {
    pixel_corner.color_fore = color;
    pixel_horizontal.color_fore = color;
    draw_pixel_alpha(offset_x, offset_y, pixel_corner);
    draw_pixel_alpha(offset_x + 1, offset_y, pixel_horizontal);
    draw_pixel_alpha(offset_x + 2, offset_y, pixel_corner);
}

// ui and game logic

void item_ui_ending_init() {
	g_item_ui_ending = true;
    g_item_ui_ending_score = false;
	g_item_ui_ending_done = false;
    g_item_ui_ending_retry = false;
	g_item_ui_ending_countdown = 0;
	g_item_ui_ending_index = -1;
}

void item_ui_ending_update() {
    if (g_item_ui_ending_score) {
        if (input_if_key_pressed(InputKey_Confirm)) {
            g_item_ui_ending_done = true;
            g_item_ui_ending_retry = false;
            audio_play_sound(AudioSound_Select);
        }
        else if (input_if_key_pressed(InputKey_Retry)) {
            g_item_ui_ending_done = true;
            g_item_ui_ending_retry = true;
            audio_play_sound(AudioSound_Select);
        }
    }
	g_item_ui_ending_countdown -= timer_get_delta_time();
	if (g_item_ui_ending_countdown <= 0) {
        ++g_item_ui_ending_index;
        while (g_item_ui_ending_index < g_item_special_num
            && g_item_special_list[g_item_ui_ending_index].status != ItemStaus_Gained) {
            ++g_item_ui_ending_index;
        }
		if (g_item_ui_ending_index >= g_item_special_num) {
			// g_item_ui_ending_done = true;
            g_item_ui_ending_score = true;
			return;
		}
		// TODO play sound
        audio_play_sound(AudioSound_Select);
		g_item_ui_ending_countdown = timer_get_rhythm_step() * 0.5;
	}
}

bool item_ui_ending_if_done() {
	return g_item_ui_ending_done;
}

bool item_ui_ending_if_retry() {
    return g_item_ui_ending_retry;
}

void item_ui_draw_hint(int index, int direction) {
    int offset_x = 40 + 20;
    int offset_y = 11;
    Pixel p_text = create_pixel(L'R', create_color(0, 0, 0), g_special_items_info[index].color);
    //draw_pixel(offset_x, offset_y, p);
}

void item_ui_draw() {
    int offset_x = 10;
    int offset_y = 2;
    int height = 2;
    // score
    bool score_all_ordered = true;
    bool score_all_spot = true;
    int combo_max = 0;
    int spot_cnt = 0;
    //Pixel pixel_l = create_pixel(L'[', create_color(0, 0, 0));
    //Pixel pixel_r = create_pixel(L']', create_color(0, 0, 0));
    // Pixel pixel_corner = create_pixel(L'+', create_color(0, 0, 0));
    // Pixel pixel_horizontal = create_pixel(L'-', create_color(0, 0, 0));
    // Pixel pixel_vertical = create_pixel(L'', create_color(0, 0, 0));
    item_ui_draw_horizontal(offset_x, offset_y, g_special_items_info[0].color);
    int combo_cnt = 0;
    for (int i = 0; i < g_item_special_num; ++i) {
        Item* item = g_item_special_list + i;
        // combo
        if (item->combo_start) {
            combo_cnt = 1;
            if (item->index > 0) {
                score_all_ordered = false;
            }
        }
        else {
            ++combo_cnt;
        }
        combo_max = combo_cnt > combo_max ? combo_cnt : combo_max;
        if (item->spot_status != ItemSpotStatus_Spotted) {
            score_all_spot = false;
        }
        else {
            ++spot_cnt;
        }
        Color color = g_special_items_info[i].color;
        pixel_vertical.color_fore = color;
        int y = offset_y + 1 + i * height;
        draw_pixel_alpha(offset_x, y, pixel_vertical);
        if (item->status == ItemStaus_Gained) {
            Color color_shine = item_get_color(item);
            draw_pixel_alpha(offset_x + 1, y, create_pixel(L'\u2588', color_shine));
        }
        else if (item->status == ItemStatus_Generated) {
            draw_pixel_alpha(offset_x + 1, y, create_pixel(L'\u2591', create_color(164, 163, 156)));
        }
        draw_pixel_alpha(offset_x + 2, y, pixel_vertical);
        // text
        if (g_item_ui_ending && item->index >= g_item_ui_ending_index) {
            Color score_color = color;
            if (item->index == g_item_ui_ending_index) {
                score_color = lerp(create_color(255, 255, 255), color, g_item_ui_ending_countdown / timer_get_rhythm_step());
                int offset = 0;
                if (item->spot_status == ItemSpotStatus_Spotted) {
                    draw_string_fullwidth(offset_x + 3, y, L"見つけた", score_color, create_color(0, 0, 0));
                    offset += 9;

                }
                wchar_t buf[16];
                swprintf(buf, sizeof(buf) / sizeof(*buf), L"+%d", combo_cnt);
                draw_string(offset_x + 3 + offset, y, buf, score_color, create_color(0, 0, 0)); // TODO alpha
                offset += 2;
                draw_string_fullwidth(offset_x + 3 + offset, y, L"連続", score_color, create_color(0, 0, 0));
            }

        }
        else if (item->combo_countdown > 0) {
            Color combo_color = lerp(create_color(255, 255, 255), color, item->combo_countdown / timer_get_rhythm_step());
            draw_string_fullwidth(offset_x + 3, y, L"連続！", combo_color, create_color(0, 0, 0)); // TODO alpha
        }
        else if (item->spot_status == ItemSpotStatus_Spotted && item->spot_countdown > 0) {
            float t = 1 - item->spot_countdown * timer_get_rhythm_step_inv();
            Color combo_color = lerp(color, create_color(0, 0, 0), t * t * t);
            draw_string_fullwidth(offset_x + 3, y, L"みっけ！", combo_color, create_color(0, 0, 0)); // TODO alpha
        }
        else if (item->spot_status == ItemSpotStatus_WaitSpotted && item->spot_countdown > 0) {
            float t = item->spot_countdown * timer_get_rhythm_step_inv() * 0.125;
            Color spot_color = color;
            int col_left = floorf(t * 8);
            Pixel p_left = create_pixel(color);
            for (int i = 0; i < col_left; ++i) {
                draw_pixel(offset_x + 3 + i, y, p_left);
            }
        }
        item_ui_draw_horizontal(offset_x, y + 1, color);
    }
    int offset_x_end = 56;
    int offset_y_end = 1;
    if (g_item_ui_ending_score) {
        //if (score_all_spot && score_all_ordered) {
        //    
        //    draw_string_fullwidth(offset_x, offset_y_end + 2, L"完ぺき！！", color_fore, create_color(0, 0, 0));
        //}
        //else {
            float rhythm_pos = timer_get_rhythm_position() * 2;
            int pos = (int)rhythm_pos % ITEM_SPECIAL_NUM;
            int pos_next = (pos + 1) % ITEM_SPECIAL_NUM;
            float t = clamp(0.0f, rhythm_pos - floorf(rhythm_pos), 1.0f);
            Color color_fore = lerp(g_special_items_info[pos].color, g_special_items_info[pos_next].color, t * t);
            Color color_bland = create_color(255, 255, 255);
            wchar_t buf[16];
            draw_string_fullwidth(offset_x_end, offset_y_end + 2, L"ステージクリア！", color_fore, create_color(0, 0, 0));
            draw_string_fullwidth(offset_x_end, offset_y_end + 3, L"時間内に見つける", score_all_spot ? color_fore : color_bland, create_color(0, 0, 0));
            swprintf(buf, sizeof(buf) / sizeof(*buf), L"%d/7", spot_cnt);
            draw_string(offset_x_end + 17, offset_y_end + 3, buf, color_fore, create_color(0, 0, 0));
            draw_string_fullwidth(offset_x_end, offset_y_end + 4, L"順番に集める", score_all_ordered ? color_fore : color_bland, create_color(0, 0, 0));
            swprintf(buf, sizeof(buf) / sizeof(*buf), L"%d/7", combo_max);
            draw_string(offset_x_end + 17, offset_y_end + 4, buf, color_fore, create_color(0, 0, 0));
        //}
        // item_ui_draw_score();
        float posf = timer_get_rhythm_position() * 0.25;
        float t2 = posf - floorf(posf);
        float lerp_t = 1 - 4.0f * (t2 - 0.5f) * (t2 - 0.5f);
        Color color_icon = lerp(create_color(125, 125, 125), color_bland, lerp_t);
        draw_string_fullwidth(offset_x_end, offset_y_end + 5, L"〇", color_icon, create_color(0, 0, 0));
        draw_string(offset_x_end + 3, offset_y_end + 5, L"CONTINUE", create_color(255, 255, 255), create_color(0, 0, 0));
        draw_string_fullwidth(offset_x_end, offset_y_end + 6, L"△", color_icon, create_color(0, 0, 0));
        draw_string(offset_x_end + 3, offset_y_end + 6, L"RETRY", create_color(255, 255, 255), create_color(0, 0, 0));
    }
}

bool item_if_done() {
    for (int i = 0; i < g_item_special_num; ++i) {
        if (g_item_special_list[i].status == ItemStatus_Idle || g_item_special_list[i].status == ItemStatus_Generated) {
            return false;
        }
    }
    return true;
}

ItemSpecialInfo* item_get_item_special_info() {
    return g_special_items_info;
}

Item* item_get_item_special_list() {
    return g_item_special_list;
};