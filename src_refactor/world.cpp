#define _CRT_SECURE_NO_WARNINGS // file reading
#include "world.h"

namespace hoge
{
	void World::Initialize()
	{
		// define block typess
		InitializeBlockPrototypes();
		// load maps from file
		InitializeConfigs();
	}

	void World::Finalize() {}

	void World::SetConfig(WorldConfigName name)
	{
		const auto& config = m_world_configs[static_cast<size_t>(name)];
		m_height = config.m_height;
		m_width = config.m_width;
		m_depth = config.m_depth;
		m_width_depth = m_width * m_depth;
		// TODO: better boundary for larger maps
		m_boundary = {
			0, 0, 0,
			m_width, m_height, m_depth
		};
		// allocate memory
		m_map = std::make_unique<Block[]>(m_width_depth * m_height);
		m_map_height = std::make_unique<int[]>(m_width_depth);
		// init map data
		for (int y = 0; y < m_height; ++y) {
			for (int z = 0; z < m_width; ++z) {
				for (int x = 0; x < m_depth; ++x) {
					int index = y * m_width_depth + z * m_width + x;
					BlockType type = config.m_map[index];
					m_map[index].type = type;
					m_map[index].status = BlockStatus::ACTIVE; // TODO
					m_map[index].is_attacked = false; // TODO
					m_map[index].is_ending = false; // TODO
					if (type != BlockType::AIR) {
						// init height map
						m_map_height[z * m_width + x] = y;
					}
				}
			}
		}
	}

	bool World::GetBlockInfoAt(int x, int y, int z, BlockInfo& block_info) const
	{
		if (x >= 0 && x < m_width
			&& y >= 0 && y < m_height
			&& z >= 0 && z < m_depth)
		{
			block_info.block = m_map.get() + (y * m_width_depth + z * m_width + x);
			block_info.block_prototype = m_block_prototypes[static_cast<size_t>(block_info.block->type)];
			block_info.pos = Vec3{ x, y, z };
			return true;
		}
		block_info.block = nullptr;
		return false;
	}

	bool World::GetVisibleBlockInfoAt(int x, int y, int z, BlockInfo& block_info)  const
	{
		bool in_boundary = GetBlockInfoAt(x, y, z, block_info);
		if (!in_boundary) {
			return false;
		}
		bool visible = block_info.block_prototype.view_type != BlockViewType::AIR;
		if (!visible) {
			block_info.block = nullptr;
		}
		return visible;
	}

	bool World::GetActiveBlockInfoAt(int x, int y, int z, BlockInfo& block_info)  const
	{
		bool in_boundary = GetBlockInfoAt(x, y, z, block_info);
		if (!in_boundary) {
			return false;
		}
		bool active = block_info.block->status == BlockStatus::ACTIVE;
		if (!active) {
			block_info.block = nullptr;
		}
		return active;
	}

	// Initialize Utils ========
	void World::InitializeBlockPrototypes()
	{
		m_block_prototypes[static_cast<size_t>(BlockType::AIR)] = BlockPrototype{
			BlockType::AIR,
			BlockInteractionType::AIR,
			BlockViewType::AIR
		};
		m_block_prototypes[static_cast<size_t>(BlockType::GROUND)] = BlockPrototype{
			BlockType::GROUND,
			BlockInteractionType::GROUND,
			BlockViewType::GROUND
		};
	}
	void World::InitializeConfigs()
	{
		// Load all txt files at once
		m_world_configs[static_cast<size_t>(WorldConfigName::TITLE)].Load(L"assets/maps/map-title.txt");
		m_world_configs[static_cast<size_t>(WorldConfigName::STAGE_0)].Load(L"assets/maps/map-stage-0.txt");
		m_world_configs[static_cast<size_t>(WorldConfigName::STAGE_1)].Load(L"assets/maps/map-stage-1.txt");
		m_world_configs[static_cast<size_t>(WorldConfigName::STAGE_2)].Load(L"assets/maps/map-stage-2.txt");
	}

	// load from text file
	void WorldConfig::Load(const std::wstring& filename)
	{
		// TODO: file reading needs to be udpdated to C++ style
		FILE* fp;
		fp = _wfopen(filename.c_str(), L"r,ccs=UTF-8");

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

		// allocate memory
		m_height = height;
		m_width = width;
		m_depth = depth;
		m_map = std::make_unique<BlockType[]>(height * width * depth);

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
					m_map[index] = static_cast<BlockType>(type);
				}
			}
			// blank line
			fgetws(line, line_buffer_size, fp);
		}
	}
}