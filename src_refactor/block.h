#pragma once

#include <memory>
#include "pixel.h"
#include "vec3.h"

namespace hoge
{
	enum class BlockType {
		AIR,
		GROUND,
		// count
		NUM
	};
	enum class BlockInteractionType {
		AIR,
		GROUND
	};

	enum class BlockViewType {
		AIR,
		GROUND
	};

	enum class BlockStatus {
		ACTIVE,
		FALL,
		MELT,
	};

	struct BlockPrototype {
		BlockType type{ BlockType::AIR };
		BlockInteractionType interaction_type{ BlockInteractionType::AIR };
		BlockViewType view_type{ BlockViewType::AIR };
	};

	struct Block {
		BlockType type{ BlockType::AIR };
		BlockStatus status{ BlockStatus::ACTIVE };
		// status ====
		// melt
		float melt_countdown{ 0.0f };
		// fall
		float offset_y{ 0.0f };
		// melt
		bool is_attacked{ false };
		Pixel* attack_pixel{};
		// ending block
		bool is_ending{ false };
		Pixel* ending_pixel{};
	};

	// for returning all info of a block
	struct BlockInfo {
		std::shared_ptr<Block> block;
		std::shared_ptr<BlockPrototype> block_prototype;
		Vec3 pos;
	};
};