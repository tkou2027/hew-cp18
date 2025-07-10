#pragma once
#include "vec.h"
#include "world.h"

// #define ITEM_NUM 20
#define ITEM_SPECIAL_NUM 7

enum ItemType {
    ItemType_None,
    ItemType_Test,
    ItemType_Special,
    // total
    ItemType_Num
};

//struct ItemPrototype {
//    ItemType type;
//    Sprite* sprite;
//};
//
//struct ItemConfig {
//    ItemType type;
//    int index;
//    Vec3 pos;
//};

enum ItemStatus {
    ItemStatus_Idle,
    ItemStatus_Generated,
    ItemStaus_Gained,
    ItemStatus_Dead
};

enum ItemSpotStatus {
    ItemSpotStatus_Idle,
    ItemSpotStatus_Spotted,
    ItemSpotStatus_WaitSpotted,
    ItemSpotStatus_Failed
};

struct ItemSpecialInfo {
    int index;
    Color color;
};

struct Item {
    ItemType type;
    int index;
    // small
    bool is_small;
    int offset_forward;
    int offset_right;
    // status
    ItemStatus status;
    Vec3 pos;
    BlockInfo block_standing;
	// combo
	bool next_expected;
	bool combo_start;
	float combo_countdown;
    // spot me
    ItemSpotStatus spot_status;
    float spot_countdown;
    bool spotted;
};

void item_init();
// map (stage)
void item_map_init();
void item_map_init_special_items(int num, const Coord2* offsets);
void item_map_update();
void item_map_draw();
void item_map_destroy();
// map manipulations
void item_map_set_item(Item* item, int x, int y, int z);
void item_map_set_item_special(int index, int x, int y, int z);
Item* item_map_get_item(int x, int y, int z);
bool item_map_gain_item(int x, int y, int z);
// single item
void item_draw(Item* item);
void item_ui_draw();
Color item_get_color(Item* item, bool base_color_only = false);
Pixel item_get_pixel(Item* item);
// ui and game logic
void item_ui_draw();
bool item_if_done();
ItemSpecialInfo* item_get_item_special_info();
Item* item_get_item_special_list();


// ending score
void item_ui_ending_init();
void item_ui_ending_update();
bool item_ui_ending_if_done();
bool item_ui_ending_if_retry();

void item_update_spot(Item* item);