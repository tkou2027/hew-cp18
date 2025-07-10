#include "stage_config.h"
#include "vec.h"

void stage_config_init_stage0(Stage* stage);
void stage_config_init_actions_stage0(Stage* stage);
void stage_config_init_stage1(Stage* stage);
void stage_config_init_actions_stage1(Stage* stage);
void stage_config_init_stage2(Stage* stage);
void stage_config_init_actions_stage2(Stage* stage);

void stage_config_init(Stage* stage) {
	switch (stage->name) {
	case StageName_Stage0: {
		stage_config_init_stage0(stage);
		break;
	}
	case StageName_Stage1: {
		stage_config_init_stage1(stage);
		break;
	}
	case StageName_Stage2: {
		stage_config_init_stage2(stage);
		break;
	}
	}
}
void stage_config_init_actions(Stage* stage) {
	switch (stage->name) {
	case StageName_Stage0: {
		stage_config_init_actions_stage0(stage);
		break;
	}
	case StageName_Stage1: {
		stage_config_init_actions_stage1(stage);
		break;
	}
	case StageName_Stage2: {
		stage_config_init_actions_stage2(stage);
		break;
	}
	}
}

// stage 0
// stage 0 - config
void stage_config_init_stage0(Stage* stage) {
	Vec3 camera_look_at{ 3.5f, 2.0f, 3.5f };
	Vec3 player_pos_init{ 3.5f, 7.0f, 3.5f };

	stage->world_config = WorldConfigName_Stage0;
	stage->player_pos_init_in[0] = player_pos_init;
	stage->camera_init_in = camera_look_at;
	stage->player_pos_init_play[0] = player_pos_init;
	stage->camera_init_play = camera_look_at;

	stage->ending_pos = Vec3{ 5.0f, 0, 5.0f };
	stage->next = StageName_Stage1;
}

// stage 0 - actions
void stage_config_init_actions_stage0(Stage* stage) {
	// items
	int special_item_num = 7;
	Coord2 offsets[] = {
		{ -1, 1 },
		{ 1, 1 },
		{ 1, -1 },
		{ 1, 1 },
		{ -1, -1 },
		{ -1, -1 },
		{ -1, 1 }
	};
	item_map_init_special_items(special_item_num, offsets);
	// attacks
	int attack_num = 10;
	// init items
	AttackFace attacks[] = {
		// init items
		{
			0, 0,
			0, 0,
			0,
			true,
			0,
			4, 2,
			false, -1,
			// respawn
			true, 3, 3

		},
		{
			0, 0,
			0, 0,
			1,
			true,
			1,
			5, 5
		},
		{
			0, 0,
			0, 0,
			1,
			true,
			2,
			2, 4
		},
		// attacks square top left
		{
			4, 4,
			5, 5,
			4,
			false, 0, 0, 0,
			true, 2 // wait
		},
		{
			3, 4,
			4, 5,
			2,
			true,
			3,
			3, 4
		},
		{
			4, 3,
			5, 4,
			2,
			false
		},
		{
			3, 3,
			4, 4,
			2,
			false
		},
		// attacks square large
		// lower left
		{
			3, 1,
			5, 3,
			4,
			true,
			4,
			4, 2
		},
		// top right
		{
			1, 3,
			3, 5,
			4,
			true,
			5,
			2, 3
		},
		// lwer right
		{
			1, 1,
			3, 3,
			2,
			true,
			6,
			2, 2,
			true, 5
		}
	};
	attack_queue_init_config(attack_num, attacks);
	attack_queue_init();
}


// stage 1
// stage 1 - config
void stage_config_init_stage1(Stage* stage) {
	Vec3 camera_look_at{ 3.5f, 2.0f, 3.5f };
	Vec3 player_pos_init{ 2.5f, 7.0f, 1.5f };

	stage->world_config = WorldConfigName_Stage1;
	stage->player_pos_init_in[0] = player_pos_init;
	stage->camera_init_in = camera_look_at;
	stage->player_pos_init_play[0] = player_pos_init;
	stage->camera_init_play = camera_look_at;

	stage->ending_pos = Vec3{ 3, 0, 4 };
	stage->next = StageName_Stage2;
}
// stage 1 - actions
void stage_config_init_actions_stage1(Stage* stage) {
	// items
	int special_item_num = 7;
	Coord2 offsets[] = {
		{ -1, 1 },
		{ -1, 1 },
		{ -1, 1 },
		{ -1, -1 },
		{ 1, 1 },
		{ 1, -1 },
		{ 1, 1 },
	};
	item_map_init_special_items(special_item_num, offsets);

	// attacks
	// attacks
	int attack_num = 16;
	AttackFace attacks[] = {
		// init item
		{
			0, 0,
			0, 0,
			0,
			true,
			0,
			5, 2,
			false, -1,
			true, 1, 1
		},
		// init item
		{
			0, 0,
			0, 0,
			0,
			true,
			1,
			1, 3
		},
		// attack floor
		{
			4, 1,
			6, 3,
			1,
			false, 0, 0, 0,
			true, 0
		},
		{
			2, 1,
			4, 3,
			2,
			false
		},
		{
			1, 1,
			2, 3,
			2,
			false, 0, 0, 0,
			false, -1,
			true, 1, 3
		},
		// attack stair
		// 1
		{
			5, 5,
			6, 6,
			2,
			false
		},
		{
			2, 3,
			3, 4,
			0,
			false
		},
		// 2
		{
			4, 5,
			6, 6,
			3,
			false
		},
		{
			3, 3,
			4, 4,
			0,
			true,
			2,
			3, 3
		},
		// 3
		{
			3, 5,
			6, 6,
			3,
			false
		},
		{
			4, 3,
			5, 4,
			0,
			false
		},
		// 4
		{
			2, 5,
			6, 6,
			3,
			true,
			3,
			5, 5
		},
		// left
		{
			1, 3,
			6, 4,
			0,
			true,
			4,
			2, 3,
			true, 3
		},
		// front
		{
			1, 5,
			6, 6,
			0,
			true,
			5,
			1, 5,
			true, 4
		},
		// back
		{
			1, 4,
			6, 5,
			2,
			true,
			6,
			5, 4
		}
		// center
		//{
		//	1, 4,
		//	6, 5,
		//	2,
		//	false,
		//	4,
		//	4, 3
		//},
	};
	attack_queue_init_config(attack_num, attacks);
	attack_queue_init();
}

// stage 2
// stage 2 - config
void stage_config_init_stage2(Stage* stage) {
	Vec3 camera_look_at{ 3.5f, 2.0f, 3.5f };
	Vec3 player_pos_init{ 3.5f, 9.0f, 3.5f };

	stage->world_config = WorldConfigName_Stage2;
	stage->player_pos_init_in[0] = player_pos_init;
	stage->camera_init_in = camera_look_at;
	stage->player_pos_init_play[0] = player_pos_init;
	stage->camera_init_play = camera_look_at;

	stage->ending_pos = Vec3{ 3, 0, 3 };
	stage->next = StageName_None;
}
// stage 2 - actions
void stage_config_init_actions_stage2(Stage* stage) {
	// items
	int special_item_num = 7;
	Coord2 offsets[] = {
		{ -1, -1 },
		{ 1, 1 },
		{ -1, 1 },
		{ 1, -1 },
		{ -1, 1 },
		{ 1, -1 },
		{ -1, 1 }
	};
	item_map_init_special_items(special_item_num, offsets);

	// init attacks
	AttackFace attacks[] = {
		{
			6, 5,
			7, 6,
			2,
			true,
			0,
			6, 5
		},
		{
			0, 1,
			1, 2,
			0,
			true,
			1,
			0, 1
		},
		// attack 2
		{
			5, 5,
			7, 6,
			2,
			false
		},
		{
			0, 1,
			2, 2,
			0,
			false
		},
		// attack 3
		{
			4, 5,
			7, 6,
			2,
			false
		},
		{
			0, 1,
			3, 2,
			0,
			false
		},
		// attack 4
		{
			3, 5,
			7, 6,
			2,
			false
		},
		{
			0, 1,
			4, 2,
			0,
			false
		},
		// attack 5
		{
			2, 5,
			7, 6,
			2,
			false
		},
		{
			0, 1,
			5, 2,
			0,
			false
		},
		// center
		{
			1, 2,
			6, 5,
			2,
			false, 0, 0, 0,
			true, 0
		},
		// SIDE
		{
			1, 1,
			6, 2,
			2,
			true,
			2,
			5, 1
		},
		{
			1, 5,
			6, 6,
			0,
			false
		},
		// center
		{
			1, 2,
			6, 5,
			2,
			true,
			3,
			5, 2,
			true, 2
		},
		{
			1, 2,
			6, 5,
			2,
			false,
		},
		{
			1, 2,
			3, 5,
			2,
			true,
			4,
			1, 4,
		},
		{
			4, 2,
			6, 5,
			0,
			false, 0, 0, 0,
			true, 4
		},
		{
			1, 2,
			3, 5,
			2,
			true,
			5,
			2, 2
			
		},
		{
			4, 2,
			6, 5,
			2,
			true,
			6,
			4, 4
			
		}
	};
	// attacks
	int attack_num = 19;
	attack_queue_init_config(attack_num, attacks);
	attack_queue_init();
}