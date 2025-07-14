#pragma once

#include <string>
#include <array>
#include <memory>

#include "block.h"

namespace hoge
{
	// map for a stage
	class WorldConfig {
	public:
		// world map (block type only)
		int m_height{ 0 };
		int m_width{ 0 };
		int m_depth{ 0 };
		std::unique_ptr<BlockType[]> m_map;
		void Load(const std::wstring& filename);
	};

	class World
	{
	public:
		enum class WorldConfigName {
			TITLE,
			STAGE_0,
			STAGE_1,
			STAGE_2,
			NUM // total
		};
		void Initialize();
		void Finalize();
		void SetConfig(WorldConfigName name);
	private:
		void InitializeBlockPrototypes();
		void InitializeConfigs();

		// world map (each block has its state)
		int m_height{ 0 };
		int m_width{ 0 };
		int m_depth{ 0 };
		int m_width_depth{ 0 };
		std::unique_ptr<Block[]> m_map;
		std::unique_ptr<int[]> m_map_height;

		std::array<BlockPrototype, static_cast<size_t>(BlockType::NUM)> m_block_prototypes;
		std::array<WorldConfig, static_cast<size_t>(WorldConfigName::NUM)> m_world_configs;
	};
}