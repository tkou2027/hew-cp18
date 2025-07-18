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
			NUM
		};
		struct WorldBoundary {
			int min_x{ 0 };
			int min_y{ 0 };
			int min_z{ 0 };
			int max_x{ 0 };
			int max_y{ 0 };
			int max_z{ 0 };
		};
		void Initialize();
		void Finalize();
		void SetConfig(WorldConfigName name);

		// getters
		float GetHeight() const { return m_height; }
		float GetWidth() const { return m_width; }
		float GetDepth() const { return m_depth; }
		WorldBoundary GetBoundary() const { return m_boundary; }

		// get block info
		bool GetBlockInfoAt(int x, int y, int z, BlockInfo& block_info) const;
		bool GetVisibleBlockInfoAt(int x, int y, int z, BlockInfo& block_info) const;
		bool GetActiveBlockInfoAt(int x, int y, int z, BlockInfo& block_info) const;
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
		WorldBoundary m_boundary;

		std::array<BlockPrototype, static_cast<size_t>(BlockType::NUM)> m_block_prototypes;
		std::array<WorldConfig, static_cast<size_t>(WorldConfigName::NUM)> m_world_configs;
	};
}