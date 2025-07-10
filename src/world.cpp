#define _CRT_SECURE_NO_WARNINGS
#include "world.h"
#include <cmath>
#include <memory.h> // malloc
#include <stdio.h> // file
#include <string.h> // string

// multiple maps loaded on game start
struct WorldConfig{
    int height;
    int width;
    int depth;
    BlockType* map;
};

static int g_world_height;
static int g_world_width;
static int g_world_depth;
static Vec3IntBoundary g_world_boundary;

static int g_world_width_depth;
static int g_world_size;

Block* g_world_map;
BlockPrototype g_world_block_prototypes[BlockType_Num];
int *g_world_map_height;

WorldConfig g_world_configs[WorldConfigName_Num];

// utils //
// init utils
void world_init_prototypes();
void world_init_create(int height, int width, int depth);
void world_load_text(const wchar_t* filename);
// config init
void world_init_configs();
void world_config_create(WorldConfig* config, int height, int width, int depth);
void world_config_load_text(WorldConfig* config, const wchar_t* filename);

// public
void world_init() {
    world_init_prototypes();
    // world_load_text(L"assets/maps/map.txt");
    world_init_configs();
}

void world_set_config(WorldConfigName name) {
    WorldConfig* config = &g_world_configs[name];
    if (!config) {
        return;
    }
    world_init_create(config->height, config->width, config->depth);
    // init status
    for (int y = 0; y < g_world_height; ++y) {
        for (int z = 0; z < g_world_depth; ++z) {
            for (int x = 0; x < g_world_width; ++x) {
                int index = y * g_world_width_depth + z * g_world_width + x;
                BlockType type = config->map[index];
                g_world_map[index].type = type;
                g_world_map[index].status = BlockStatus_Active; // TODO
                g_world_map[index].is_attacked = false; // TODO
                g_world_map[index].is_ending = false; // TODO
                if (type > 0) {
                    g_world_map_height[z * g_world_width + x] = y; // init height map
                }
            }
        }
    }
}

void world_destroy() {
    // free(g_world_collision_map);
    for (int i = 0; i < WorldConfigName_Num; ++i) {
        if (g_world_configs[i].map) {
            free(g_world_configs[i].map);
            g_world_configs[i].map = NULL;
        }
    }
    free(g_world_map);
    free(g_world_map_height);
    // g_world_collision_map = NULL;
    g_world_map = NULL;
    g_world_map_height = NULL;
}

const Block* world_get_map() {
    return g_world_map;
}

int world_get_height() {
    return g_world_height;
}

int world_get_width() {
    return g_world_width;
}

int world_get_depth() {
    return g_world_depth;
}

int* world_get_map_height() {
	return g_world_map_height;
}
int world_get_map_height_at(int x, int z) {
	return g_world_map_height[z * g_world_width + x];
}

Vec3IntBoundary world_get_boundary() {
    return g_world_boundary;
}

void world_remove_block_at(int x, int y, int z) {
	if (x >= 0 && x < g_world_width
		&& y >= 0 && y < g_world_height
		&& z >= 0 && z < g_world_depth) {
		g_world_map[y * g_world_width_depth + z * g_world_width + x].type = BlockType_Air;
        int height_map_index = z * g_world_width + x;
        if (g_world_map_height[height_map_index] == y) {
            --g_world_map_height[height_map_index];
        }
		// return &g_world_map[y * g_world_width_depth + z * g_world_width + x];
	}
}

const Block* world_get_block_at(int x, int y, int z) {
    if (x >= 0 && x < g_world_width
        && y >= 0 && y < g_world_height
        && z >= 0 && z < g_world_depth) {
        return &g_world_map[y * g_world_width_depth + z * g_world_width + x];
    }
    return NULL; // out of bound
}

const Block* world_get_visible_block_at(int x, int y, int z) {
    if (x >= 0 && x < g_world_width
        && y >= 0 && y < g_world_height
        && z >= 0 && z < g_world_depth) {
        const Block *block = &g_world_map[y * g_world_width_depth + z * g_world_width + x];
        // TODO check if active
        return block->type == BlockType_Air ? NULL : block;
    }
    return NULL; // out of bound
}

const Block* world_get_visible_active_block_at(int x, int y, int z) {
    if (x >= 0 && x < g_world_width
        && y >= 0 && y < g_world_height
        && z >= 0 && z < g_world_depth) {
        const Block* block = &g_world_map[y * g_world_width_depth + z * g_world_width + x];
        // TODO check if active
        return block->type == BlockType_Air || block->status != BlockStatus_Active ? NULL : block;
    }
    return NULL; // out of bound
}

bool world_get_block_info_at(BlockInfo* block_info, int x, int y, int z) {
    if (x >= 0 && x < g_world_width
        && y >= 0 && y < g_world_height
        && z >= 0 && z < g_world_depth) {
        block_info->block = &g_world_map[y * g_world_width_depth + z * g_world_width + x];
        block_info->block_prototype = &g_world_block_prototypes[block_info->block->type];
        block_info->pos = Vec3{(float)x, (float)y, (float)z};
        return true;
    }
    block_info->block = NULL;
    block_info->block_prototype = NULL;
    return false;
}

bool world_get_visible_block_info_at(BlockInfo* block_info, int x, int y, int z) {
    bool in_boundary = world_get_block_info_at(block_info, x, y, z);
    if (!in_boundary) {
        return false;
    }
    bool visible = block_info->block_prototype->view_type != BlockViewType_Air;
    if (!visible) {
        block_info->block = NULL;
        block_info->block_prototype = NULL;
    }
    return visible;
}

bool world_get_visible_active_block_info_at(BlockInfo* block_info, int x, int y, int z) {
    bool visible = world_get_visible_block_info_at(block_info, x, y, z);
    if (!visible) {
        return false;
    }
    bool active = block_info->block->status == BlockStatus_Active;
    if (!active) {
        block_info->block = NULL;
        block_info->block_prototype = NULL;
    }
    return active;
}

bool world_if_block_passable(int x, int y, int z) {
    const Block* block = world_get_block_at(x, y, z);
    if (!block) {
        return false;
    }
    BlockPrototype proto = g_world_block_prototypes[block->type];
    return proto.interaction_type == BlockInteractionType_Air; // TODO
}

bool world_if_block_ground(int x, int y, int z) {
    const Block* block = world_get_block_at(x, y, z);
    if (!block) {
        return false;
    }
    BlockPrototype proto = g_world_block_prototypes[block->type];
    return proto.interaction_type == BlockInteractionType_Ground; // TODO
}

bool world_if_block_passable(BlockType type) {
    BlockPrototype proto = g_world_block_prototypes[type];
    return proto.interaction_type == BlockInteractionType_Air; // TODO
}

bool world_if_block_ground(BlockType type) {
    BlockPrototype proto = g_world_block_prototypes[type];
    return proto.interaction_type == BlockInteractionType_Ground; // TODO
}

float world_get_y(Vec3 pos) {
    float y = (int)(pos.y);
    while (world_if_block_passable(pos.x, y, pos.z)) {
        --y;
    }
    return y + 1;
}

void world_get_block_info_below(BlockInfo *block_info, Vec3 pos) {
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;
    while (y >= 0 && !world_if_block_ground(x, y, z)) { --y; }
    world_get_block_info_at(block_info, x, y, z);
}

void world_init_fall(int y_offset) {
    for (int y = 0; y < g_world_height; ++y) {
        for (int z = 0; z < g_world_depth; ++z) {
            for (int x = 0; x < g_world_width; ++x) {
                int index = y * g_world_width_depth + z * g_world_width + x;
                if (y == 0) {
                    g_world_map[index].status = BlockStatus_Active;
                }
                else {
                    g_world_map[index].status = BlockStatus_Fall;
                    g_world_map[index].offset_y = y_offset;
                }
            }
        }
    }
}
// public

// utils
// init utils
// define prototypes
void world_init_prototypes() {
    g_world_block_prototypes[BlockType_Air] = BlockPrototype{
        BlockType_Air,
        BlockInteractionType_Air,
        BlockViewType_Air
    };
    g_world_block_prototypes[BlockType_Ground] = BlockPrototype{
        BlockType_Ground,
        BlockInteractionType_Ground,
        BlockViewType_Ground
    };
}

void world_init_create(int height, int width, int depth) {
    g_world_height = height;
    g_world_width = width;
    g_world_depth = depth;
    g_world_width_depth = width * depth;
    g_world_size = g_world_width_depth * height;

    // TODO better boundary
    g_world_boundary.min_x = 0;
    g_world_boundary.min_y = 0;
    g_world_boundary.min_z = 0;
    g_world_boundary.max_x = width;
    g_world_boundary.max_y = height;
    g_world_boundary.max_z = depth;
    // g_world_collision_map = (int*)calloc(g_world_size, sizeof(int));
    g_world_map = (Block*)calloc(g_world_size, sizeof(Block));
    g_world_map_height = (int*)calloc(g_world_width_depth, sizeof(int));

    if (g_world_map == NULL || g_world_map_height == NULL) {
        perror("Failed to create map structures\n");
        return;
    }
}

void world_load_text(const wchar_t* filename) {
    FILE* fp;
    fp = _wfopen(filename, L"r,ccs=UTF-8");

    if (fp == NULL) {
        perror("Failed to open map file\n");
        return;
    }

    const int line_buffer_size = 256;
    wchar_t line[line_buffer_size];
    // read first two line for metadata
    if (!fgetws(line, line_buffer_size, fp)
        || !fgetws(line, line_buffer_size, fp)) {
        perror("Failed to read map file\n");
        return;
    }
    // split second line
    wchar_t seps[] = L" ,\t\n";
    wchar_t* token;
    wchar_t* next_token = NULL;

    // width
    token = wcstok_s(line, seps, &next_token);
    int width = _wtoi(token);
    // height
    token = wcstok_s(NULL, seps, &next_token);
    int height = _wtoi(token);
    // depth
    token = wcstok_s(NULL, seps, &next_token);
    int depth = _wtoi(token);

    // TODO check size
    // init size
    world_init_create(height, width, depth);

    for (int y = 0; y < g_world_height; ++y) {
        for (int z = 0; z < g_world_depth; ++z) {
            if (!fgetws(line, line_buffer_size, fp)) {
                break;
            }
            for (int x = 0; x < g_world_width; ++x) {
                token = wcstok_s(x == 0 ? line : NULL, seps, &next_token);
                if (!token) {
                    break;
                }
                int type = _wtoi(token);
                int index = y * g_world_width_depth + z * g_world_width + x;
                g_world_map[index].type = type;

                g_world_map[index].status = BlockStatus_Active; // TODO
                
                // g_world_map[index].player_stay = 0.0f; // TODO
                // g_world_map[index].attack = NULL; // TODO
                if (type > 0) {
                    g_world_map_height[z * g_world_width + x] = y; // init height map
                }
            }
        }
        // blank line
        fgetws(line, line_buffer_size, fp);
    }
}

void world_config_create(WorldConfig* config, int height, int width, int depth) {
    config->height = height;
    config->width = width;
    config->depth = depth;
    int size = height * width * depth;
    config->map = (BlockType*)calloc(size, sizeof(BlockType));
    if (config->map == NULL) {
        perror("Failed to create map config structures\n");
        return;
    }
}

void world_config_load_text(WorldConfig* config, const wchar_t* filename) {
    FILE* fp;
    fp = _wfopen(filename, L"r,ccs=UTF-8");

    if (fp == NULL) {
        perror("Failed to open map file\n");
        return;
    }

    const int line_buffer_size = 256;
    wchar_t line[line_buffer_size];
    // read first two line for metadata
    if (!fgetws(line, line_buffer_size, fp)
        || !fgetws(line, line_buffer_size, fp)) {
        perror("Failed to read map file\n");
        return;
    }
    // split second line
    wchar_t seps[] = L" ,\t\n";
    wchar_t* token;
    wchar_t* next_token = NULL;

    // width
    token = wcstok_s(line, seps, &next_token);
    int width = _wtoi(token);
    // height
    token = wcstok_s(NULL, seps, &next_token);
    int height = _wtoi(token);
    // depth
    token = wcstok_s(NULL, seps, &next_token);
    int depth = _wtoi(token);

    // TODO check size
    // init size
    world_config_create(config, height, width, depth);
    int width_depth = width * depth;

    for (int y = 0; y < height; ++y) {
        for (int z = 0; z < depth; ++z) {
            if (!fgetws(line, line_buffer_size, fp)) {
                break;
            }
            for (int x = 0; x < width; ++x) {
                token = wcstok_s(x == 0 ? line : NULL, seps, &next_token);
                if (!token) {
                    break;
                }
                int type = _wtoi(token);
                int index = y * width_depth + z * width + x;
                config->map[index] = (BlockType)type;

                //g_world_map[index].status = BlockStatus_Active; // TODO

                //// g_world_map[index].player_stay = 0.0f; // TODO
                //g_world_map[index].attack = NULL; // TODO
                //if (type > 0) {
                //    g_world_map_height[z * width + x] = y; // init height map
                //}
            }
        }
        // blank line
        fgetws(line, line_buffer_size, fp);
    }
}

void world_init_configs() {
    world_config_load_text(&g_world_configs[WorldConfigName_Title], L"assets/maps/map-title.txt");
    world_config_load_text(&g_world_configs[WorldConfigName_Stage0], L"assets/maps/map-stage-0.txt");
    world_config_load_text(&g_world_configs[WorldConfigName_Stage1], L"assets/maps/map-stage-1.txt");
    world_config_load_text(&g_world_configs[WorldConfigName_Stage2], L"assets/maps/map-stage-2.txt");
}
