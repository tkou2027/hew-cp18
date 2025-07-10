#include "attack.h"
#include "world.h"
#include "timer.h"
#include "player.h"
#include "cube_top.h"
#include "world_view.h"
#include "world_view_top.h"
#include "item.h"
#include "draw.h"
#include <stdio.h>
#include <cmath>

AttackFaceQueue g_attack_queue;

void attack_face_init(AttackFace* attack);
void attack_face_update(AttackFace* attack);
void attack_face_draw(AttackFace* attack);

void attack_queue_init() {
	g_attack_queue.head_init = 0;
	g_attack_queue.head_active = -1;
	g_attack_queue.time_milli = timer_get_current_time_milli();
}

void attack_queue_init_config(int num, const AttackFace* attacks) {
	g_attack_queue.total = num;
	float time_unit = timer_get_rhythm_step() * 2.0f; // TODO
	for (int i = 0; i < num; ++i) {
		g_attack_queue.attacks[i] = attacks[i];
		g_attack_queue.attacks[i].init_time_offset *= time_unit;
	}
}

void attack_queue_update_init() {


}

void attack_queue_update() {
	// initiate
	// int game_time = timer_get_current_time_milli() - g_attack_queue.time_milli;

	// TODO test map
	while (g_attack_queue.head_init < g_attack_queue.total) {
		// && g_attack_queue.attacks[g_attack_queue.head_init].init_time_offset <= 0) {
	  // while (g_attack_queue.head_init < g_attack_queue.total) {
		AttackFace* attack = g_attack_queue.attacks + g_attack_queue.head_init;
		Item* item_special_list = item_get_item_special_list();
		if (attack->item_wait) {
			bool item_cleared = true;

			for (int i = 0; i <= attack->item_wait_index; ++i) {
				Item* item = item_special_list + i;
				if (item->status != ItemStaus_Gained && item->status != ItemStatus_Dead) {
					item_cleared = false;
				}
			}
			if (!item_cleared) {
				break;
			}
		}
		attack->init_time_offset -= timer_get_delta_time();
		// TODO if condition
		if (attack->init_time_offset > 0) {
			attack->init_time_offset -= timer_get_delta_time();
			break;
		}
		attack_face_init(g_attack_queue.attacks + g_attack_queue.head_init);
		++g_attack_queue.head_init;
	}
	// update
	for (int i = g_attack_queue.head_active; i < g_attack_queue.head_init; ++i) {
		attack_face_update(g_attack_queue.attacks + i);
	}
}

void attack_queue_draw() {
	for (int i = g_attack_queue.head_active; i < g_attack_queue.head_init; ++i) {
		attack_face_draw(g_attack_queue.attacks + i);
	}
}

// single grid
// inside cube???

void attack_face_init(AttackFace* attack) {
	if (attack->respawn_set == true) {
		Player* player = player_get_players();
		player->respawn_x = attack->respawn_x;
		player->respawn_z = attack->respawn_z;
	}
	if (attack->min_x >= attack->max_x || attack->min_z >= attack->max_z) {
		// gen item
		attack->state = AttackFaceState_Melt;
		return;
	}
	attack->state = AttackFaceState_Countdown;
	attack->state_start_time = 0.0f;
	attack->pixel.text = L'\u2573';
	BlockInfo block_info;
	attack->pixel.color_fore = create_color(255, 255, 255);
	attack->pixel_item.color_fore = create_color(255, 255, 0);
	for (int z = attack->min_z; z < attack->max_z; ++z) {
		for (int x = attack->min_x; x < attack->max_x; ++x) {
			if (world_get_visible_block_info_at(
				&block_info,
				x, world_get_map_height_at(x, z), z
			)) {
				block_info.block->is_attacked = true;
				block_info.block->attack_pixel = &attack->pixel;
				//(attack->item && x == attack->item_x && z == attack->item_z) ?
				//&attack->pixel_item : &attack->pixel;
			}
		}
	}
}

void attack_face_update(AttackFace* attack) {
	switch (attack->state) {
	case AttackFaceState_Countdown: {
		attack->state_start_time += timer_get_delta_time();
		float duration = timer_get_rhythm_step() * 3;
		float t = attack->state_start_time / duration;
		int frame = (int)(t * 3) % 3; // TODO config
		attack->pixel.text = frame == 2 ? L'#' : frame == 0 ? L'.' : L'x';
		//attack->pixel.color_fore = lerp(create_color(255, 255, 255), create_color(255, 69, 0), t);
		if (attack->state_start_time > duration) { // TODO
			attack->state = AttackFaceState_Attack;
			attack->state_start_time = 0.0f;
			// attack->pixel.text = L'\u2588';
			attack->pixel.text = L' ';
			attack->pixel.color_fore = create_color(255, 255, 255);
		}
		break;
	}
	case AttackFaceState_Attack: {
		attack->state_start_time += timer_get_delta_time();
		if (attack->state_start_time > 0) { //timer_get_rhythm_step()) { // TODO
			attack->state = AttackFaceState_Melt;
			attack->state_start_time = 0.0f;
			attack->pixel.text = L' ';
		}
		break;
	}
	case AttackFaceState_Melt: {
		attack->state_start_time += timer_get_delta_time();
		if (attack->state_start_time > 0) { // TODO
			attack->state = AttackFaceState_Idle;
			// attack->state_start_time = 0.0f;
			for (int z = attack->min_z; z < attack->max_z; ++z) {
				for (int x = attack->min_x; x < attack->max_x; ++x) {
					int y = world_get_map_height_at(x, z);
					world_remove_block_at(x, y, z);
					// item_map_set_item(ItemType_None, x, y + 1, z);
					// item_map_set_item(ItemType_Test, x, y, z);
				}
			}
			if (attack->item) {
				int y = world_get_map_height_at(attack->item_x, attack->item_z) + 1;
				item_map_set_item_special(attack->item_index, attack->item_x, y, attack->item_z);
			}
		}
		break;
	}
	}
}

void attack_face_draw(AttackFace* attack) {
	return;
	switch (attack->state) {
	case AttackFaceState_Countdown:
	case AttackFaceState_Attack: {
		for (int z = attack->min_z; z < attack->max_z; ++z) {
			for (int x = attack->min_x; x < attack->max_x; ++x) {
				Coord2 pos_screen = world_view_top_get_cube_offset(
					world_view_get_camera_top(),
					x,
					world_get_map_height_at(x, z),
					z
				);
				Color color = attack->state == AttackFaceState_Attack ?
					create_color(255, 0, 0) : create_color(255, 255, 255);
				Pixel pixel = create_pixel(color);
				for (int y = 0; y < 2; ++y) {
					for (int x = 0; x < 4; ++x) {
						draw_pixel(pos_screen.x + x, pos_screen.y + y, pixel);
					}
				}
			}
		}
		break;
	}
	}
}