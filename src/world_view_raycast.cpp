#include "world_view_raycast.h"

#include "world.h"
#include "draw.h"
#include "cube.h"
// draw
#include "player.h"
#include "item.h"

#include <stdio.h>
#include <memory.h> // memset
#include <cmath>

static bool g_render_object;

void world_view_ray_camera_cast_ray(Ray* ray, const WorldViewRayCamera* camera, int x, int y);
// ray cast
void world_view_ray_precompute_direction(Ray* ray);
// Amanatides and Woo's fast Voxel Traversal
bool world_view_ray_if_hit(RayHitRecord* hit_record, const Ray* ray);
bool world_view_ray_if_hit_object(RayHitRecord* hit_record, const Ray* ray, const Vec3* t_max);
void world_view_ray_init_camera(WorldViewRayCamera* camera,
	const Vec3 look_at, float screen_width, float screen_height, float radians) {
	camera->look_at = look_at;
	camera->screen_width = screen_width;
	camera->screen_height = screen_height;

	// static direction 1 : 2
	float t = 1.0f / sqrtf(5.0f);
	camera->direction = Vec3{ 0, -t * 2.0f, t };

	// rotation
	camera->radians = radians;
	camera->sin_radians = sinf(radians);
	camera->cos_radians = cosf(radians);
	camera->direction = vec3_rotate_reverse_y(&camera->direction, camera->sin_radians, camera->cos_radians);

	Vec3 screen_forward{ 0, 0, 1 };
	Vec3 screen_right{ 1, 0 , 0 };
	camera->grid_forward = vec3_rotate_reverse_y(&screen_forward, camera->sin_radians, camera->cos_radians);
	camera->grid_right = vec3_rotate_reverse_y(&screen_right, camera->sin_radians, camera->cos_radians);
	camera->grid_forward = vec3_get_major_axis(&camera->grid_forward);
	camera->grid_right = vec3_get_major_axis(&camera->grid_right);
	// round to 0, 1

	Vec3 origin = vec3_add(look_at, vec3_multiply(camera->direction, -8.0f)); // TODO adjust factor

	// use constant direction so no need for cross product
	camera->u = Vec3{ 0.25f, 0.0f, 0.0f };
	camera->v = Vec3{ 0.0f, -0.2f, -0.4f };
	// rotation
	camera->u = vec3_rotate_reverse_y(&camera->u, camera->sin_radians, camera->cos_radians);
	camera->v = vec3_rotate_reverse_y(&camera->v, camera->sin_radians, camera->cos_radians);
	Vec3 offset_u = vec3_multiply(camera->u, -(screen_width / 2.0f - 0.5f));
	Vec3 offset_v = vec3_multiply(camera->v, -(screen_height / 2.0f)); // magic
	camera->pos_top_left = vec3_add(vec3_add(origin, offset_u), offset_v);
}

void world_view_ray_camera_cast_ray(Ray* ray, const WorldViewRayCamera* camera, int x, int y) {
	Vec3 offset_u = vec3_multiply(camera->u, x);
	Vec3 offset_v = vec3_multiply(camera->v, y);
	ray->origin = vec3_add(vec3_add(camera->pos_top_left, offset_u), offset_v);
	ray->direction = camera->direction;
}

void world_view_ray_precompute_direction(Ray* ray) {
	// same direction for all orthographic rays
	// TODO divide by zero?

	for (int i = 0; i < 3; ++i) {
		Vec3Axis axis = (Vec3Axis)i;
		// float orig = vec3_get_axis(&ray->origin, axis);
		float dir = vec3_get_axis(&ray->direction, axis);
		if (dir > 0) {
			float inv_dir = 1.0f / dir;
			vec3_set_axis(&ray->inv_direction, axis, inv_dir);
			vec3_set_axis(&ray->step, axis, 1.0f);
			vec3_set_axis(&ray->t_delta, axis, inv_dir); // voxel size / direction
			// vec3_set_axis(&ray->t_max, axis, (ceilf(orig) - orig) * inv_dir); // TODO 
		}
		else if (dir < 0) {
			float inv_dir = 1.0f / dir;
			vec3_set_axis(&ray->inv_direction, axis, inv_dir);
			vec3_set_axis(&ray->step, axis, -1.0f);
			vec3_set_axis(&ray->t_delta, axis, -inv_dir); // voxel size / direction
			// vec3_set_axis(&ray->t_max, axis, (ceilf(orig - 1.0f) - orig) * inv_dir); // TODO 
		}
		else { // == 0
			vec3_set_axis(&ray->inv_direction, axis, 1e6f);
			vec3_set_axis(&ray->step, axis, 0.0f);
			vec3_set_axis(&ray->t_delta, axis, 1e6f); // voxel size / direction
			//vec3_set_axis(&ray->t_max, axis, 1e6f); // TODO 
		}
	}
}

// ray test hit
bool world_view_ray_if_hit(RayHitRecord* hit_record, const Ray* ray) {
	// init tMax
	Vec3 t_max;
	hit_record->block_pos = { floorf(ray->origin.x), floorf(ray->origin.y), floorf(ray->origin.z) };
	for (int i = 0; i < 3; ++i) {
		Vec3Axis axis = (Vec3Axis)i;
		float orig = vec3_get_axis(&ray->origin, axis);
		float dir = vec3_get_axis(&ray->direction, axis);
		float inv_dir = vec3_get_axis(&ray->inv_direction, axis);
		if (dir > 0) {
			vec3_set_axis(&t_max, axis, (floorf(orig) + 1.0f - orig) * inv_dir); // TODO 
		}
		else if (dir < 0) {
			vec3_set_axis(&t_max, axis, (ceilf(orig) - 1.0f - orig) * inv_dir); // TODO 
		}
		else {
			vec3_set_axis(&t_max, axis, 1e6f); // INFINITY
		}
	}

	// traverse
	BlockInfo block_info;
	int max_step = 20;
	Vec3Axis axis = Vec3Axis_X;

	//bool prev_hit_y = false;
	bool hit_y = false;
	hit_record->t = 0;
	while (max_step--) {
		// if hit
		if (world_get_visible_block_info_at(&block_info,
			hit_record->block_pos.x, hit_record->block_pos.y, hit_record->block_pos.z)
			&& block_info.block->status != BlockStatus_Fall) {
			//hit_record->t = t;
			hit_record->hit_pos = vec3_add(ray->origin, vec3_multiply(ray->direction, hit_record->t));
			//hit_record->block_pos = pos;
			hit_record->is_top = hit_y;
			hit_record->is_object = false;
			// fprintf(stderr, "world = %.1f %.1f %.1f\n", hit_pos.x, hit_pos.y, hit_pos.z);
			FaceDir dir = hit_y ? FaceDir_Top : FaceDir_Front; // all sides are front
			cube_get_face_info(&hit_record->face_info, dir, &block_info);
			// *color = cube_get_top_color(&block_info);
			return true;
		}
		if (world_view_ray_if_hit_object(hit_record, ray, &t_max)) {
			return true;
		}

		// update
		if (t_max.x < t_max.y && t_max.x < t_max.z) {
			hit_record->block_pos.x += ray->step.x;
			hit_record->t = t_max.x;
			t_max.x += ray->t_delta.x;
			hit_y = false;
		}
		else if (t_max.y < t_max.z) {
			hit_record->block_pos.y += ray->step.y;
			hit_record->t = t_max.y;
			t_max.y += ray->t_delta.y;
			hit_y = true;
		}
		else {
			hit_record->block_pos.z += ray->step.z;
			hit_record->t = t_max.z;
			t_max.z += ray->t_delta.z;
			hit_y = false;
		}
	}
	return false;
}

bool world_view_ray_if_hit_shadow(WorldViewRayCamera* camera, const RayHitRecord* hit_record) {
	// TODO
	float center_offset_x = hit_record->hit_pos.x - hit_record->block_pos.x - 0.5f;
	float center_offset_z = hit_record->hit_pos.z - hit_record->block_pos.z - 0.5f;
	int x = (int)hit_record->block_pos.x;
	int y = (int)hit_record->block_pos.y;
	int z = (int)hit_record->block_pos.z;

	Vec3 forward = camera->grid_forward;
	Vec3 right = camera->grid_right;
	// dir top
   //  BlockInfo block_info;
	bool top = forward.x * center_offset_x >= 0 && forward.z * center_offset_z >= 0;
	bool left = right.x * center_offset_x <= 0 && right.z * center_offset_z <= 0;
	if (top) {
		if (world_get_visible_active_block_at(x + forward.x, y + 1, z + forward.z)) {
			return true;
		}
	}
	// dir left
	if (left) {
		if (world_get_visible_active_block_at(x - right.x, y + 1, z - right.z)) {
			return true;
		}
	}
	if (top && left) {
		// top left corner
		if (world_get_visible_active_block_at(x + forward.x - right.x, y + 1, z + forward.z - right.z)) {
			return true;
		}
	}
	// dir corner
	return false;
}

bool world_view_ray_if_grid_center(const Vec3* pos, float size_half) {
	float center_dist_x = fabs(pos->x - floorf(pos->x) - 0.5f);
	float center_dist_z = fabs(pos->z - floorf(pos->z) - 0.5f);
	return center_dist_x <= size_half && center_dist_z <= size_half;
	// return center_dist_x * center_dist_x + center_dist_z * center_dist_z <= size_half * size_half;
}

bool world_view_ray_if_grid_center(const Vec3* pos, float size_half_x, float size_half_z, float center_x, float center_y) {
    float center_dist_x = fabs(pos->x - floorf(pos->x) - center_x);
    float center_dist_z = fabs(pos->z - floorf(pos->z) - center_y);
    return center_dist_x <= size_half_x && center_dist_z <= size_half_z;
    // return center_dist_x * center_dist_x + center_dist_z * center_dist_z <= size_half * size_half;
}

bool world_view_ray_if_hit_shadow_object(WorldViewRayCamera* camera, const RayHitRecord* hit_record) {
    if (!g_render_object) {
        return false;
    }
	float center_offset_x = hit_record->hit_pos.x - hit_record->block_pos.x - 0.5f;
	float center_offset_z = hit_record->hit_pos.z - hit_record->block_pos.z - 0.5f;
	int x = (int)hit_record->block_pos.x;
	int y = (int)hit_record->block_pos.y;
	int z = (int)hit_record->block_pos.z;

	// player
	Player* player = player_get_players();
	for (int i = 0; i < PLAYER_NUM; ++i, ++player) {
		// if hit player
		if (player->block_x == x
			&& player->block_y == y + 1
			&& player->block_z == z) {
			// && world_view_ray_if_grid_center(&hit_record->hit_pos, 0.3)) {
			return true;
		}
	}
	// item
	Item* item = item_map_get_item(x, y + 1, z);
	if (item && item->type != ItemType_None) {
		// && world_view_ray_if_grid_center(&hit_record->hit_pos, 0.3)) {
		return true;
	}
	return false;
}

bool world_view_ray_if_hit_object(RayHitRecord* hit_record, const Ray *ray, const Vec3* t_max) {
    if (!g_render_object) {
        return false;
    }
	int x = (int)hit_record->block_pos.x;
	int y = (int)hit_record->block_pos.y;
	int z = (int)hit_record->block_pos.z;
	float t_max_next = (t_max->x < t_max->y && t_max->x < t_max->z) ? t_max->x
		: t_max->y < t_max->z ? t_max->y : t_max->z;
	// player
	Player* player = player_get_players();
	for (int i = 0; i < PLAYER_NUM; ++i, ++player) {
		// if hit player
		if (player->block_x == x
			&& player->block_y == y
			&& player->block_z == z) {
			float t = lerp(hit_record->t, t_max_next, 0.5);
			hit_record->hit_pos = vec3_add(ray->origin, vec3_multiply(ray->direction, t));
			// && world_view_ray_if_grid_center(pos, 0.5)) {
			if (world_view_ray_if_grid_center(&hit_record->hit_pos, 0.25)) {
				hit_record->is_object = true;
                hit_record->is_object_small = false;
				hit_record->pixel = create_pixel(player_get_color(player));
				return true;
			}
		}
	}
	Item* item = item_map_get_item(x, y, z);
	if (item && item->type != ItemType_None) {//}&& world_view_ray_if_grid_center(pos, 0.5)) { // TODO item size
        float t = t_max_next; //  lerp(hit_record->t, t_max_next, 0.5);
		hit_record->hit_pos = vec3_add(ray->origin, vec3_multiply(ray->direction, t));
		// && world_view_ray_if_grid_center(pos, 0.5)) {
		if (world_view_ray_if_grid_center(&hit_record->hit_pos, 0.2, 0.2,
                item->offset_right > 0 ? 0.625f : 0.375f, item->offset_forward < 0 ? 0.375 : 0.625f)) {
			hit_record->is_object = true;
            hit_record->is_object_small = true;
			hit_record->pixel = item_get_pixel(item);
			//
			item_update_spot(item);
			//
			return true;
		}
	}
	return false;
}

struct RayHitRecordPrev {
	bool hit;
	bool is_object;
    bool is_object_small;
	Vec3 block_pos;
};

// draw
void world_view_ray_draw(WorldViewRayCamera* camera) {
	Ray ray;
	memset(&ray, 0, sizeof(Ray));
	ray.direction = camera->direction;

	RayHitRecordPrev prev_line[80];

	world_view_ray_precompute_direction(&ray);
	for (int y = 0; y < camera->screen_height; ++y) {
		for (int x = 0; x < camera->screen_width; ++x) {
			// set origin
			world_view_ray_camera_cast_ray(&ray, camera, x, y);
			RayHitRecord hit_record;
			if (world_view_ray_if_hit(&hit_record, &ray)) {
				if (hit_record.is_object) {
					draw_pixel(x, y, hit_record.pixel);
				}
				else {
					// fprintf(stderr, "screen = %d %d\n", x, y);
					bool is_shadow = world_view_ray_if_hit_shadow(camera, &hit_record);
					Pixel pixel = hit_record.is_top ?
						(is_shadow ? hit_record.face_info.shadow_pixels[0] : hit_record.face_info.pixels[0])
						: hit_record.face_info.pixels[x % 2];
					if (hit_record.is_top && hit_record.face_info.has_pixel_mix) {
						pixel = pixel_get_mixed(&pixel, &hit_record.face_info.pixel_mix);
					}
					draw_pixel(x, y, pixel);
					if (hit_record.is_top
						&& world_view_ray_if_hit_shadow_object(camera, &hit_record)
						&& y >= 1 && prev_line[x].hit && prev_line[x].is_object) {
                        Color shadow_color = color_shade(hit_record.face_info.shadow_colors[0], 0.8);
                        if (prev_line[x].is_object_small) {
                            Pixel* prev_line_pixel = buffer_get_pixel_at(x, y - 1);
                            prev_line_pixel->color_fore = shadow_color;
                        }
                        else {
                            draw_pixel_alpha(x, y, create_pixel(L'\u2580', shadow_color));
                        }
					}
				}
				prev_line[x].hit = true;
				prev_line[x].block_pos = hit_record.block_pos;
				prev_line[x].is_object = hit_record.is_object;
                prev_line[x].is_object_small = hit_record.is_object_small;
			}
			else {
				prev_line[x].hit = false;
			}
		}
	}
}

void world_view_ray_set_render_object(bool render_object) {
    g_render_object = render_object;
}