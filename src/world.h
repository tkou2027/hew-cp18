#pragma once
#include "vec.h"
#include "attack.h"

enum BlockType {
    BlockType_Air,
    BlockType_Ground,
    // count
    BlockType_Num
};

enum BlockInteractionType {
    BlockInteractionType_Air,
    BlockInteractionType_Ground
};

enum BlockViewType {
    BlockViewType_Air,
    BlockViewType_Ground
};

enum BlockStatus {
    BlockStatus_Active,
    BlockStatus_Fall,
    BlockStatus_Melt,
};

struct BlockPrototype {
    BlockType type;
    BlockInteractionType interaction_type;
    BlockViewType view_type;
};

struct Block {
    int type;
    int chunk;
    BlockStatus status;
    // status
    // melt
    float melt_countdown;
	// fall
	float offset_y;
    // melt
    bool is_attacked;
    Pixel* attack_pixel;
    // ending block
    bool is_ending;
    Pixel* ending_pixel;
};

// for returning all info of a block
struct BlockInfo {
    Block* block;
    BlockPrototype* block_prototype;
    Vec3 pos;
};

enum WorldConfigName {
    WorldConfigName_Title,
    WorldConfigName_Stage0,
    WorldConfigName_Stage1,
    WorldConfigName_Stage2,

    WorldConfigName_Num // total
};

void world_init();
void world_set_config(WorldConfigName name);
void world_destroy();
const Block* world_get_map();
const Block* world_get_block_at(int x, int y, int z);
const Block* world_get_visible_block_at(int x, int y, int z); // for drawing
const Block* world_get_visible_active_block_at(int x, int y, int z); // for drawing
bool world_get_block_info_at(BlockInfo* block, int x, int y, int z);
bool world_get_visible_block_info_at(BlockInfo* block_info, int x, int y, int z); // for drawing
bool world_get_visible_active_block_info_at(BlockInfo* block_info, int x, int y, int z); // for drawing
int world_get_height();
int world_get_width();
int world_get_depth();
int* world_get_map_height();
int world_get_map_height_at(int x, int z);
Vec3IntBoundary world_get_boundary();

void world_remove_block_at(int x, int y, int z);
void world_init_fall(int y_offset);

float world_get_y(Vec3 pos);
void world_get_block_info_below(BlockInfo* block_info, Vec3 pos);

bool world_if_block_passable(int x, int y, int z);
bool world_if_block_ground(int x, int y, int z);