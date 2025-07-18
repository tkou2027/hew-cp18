#include "world_view.h"

namespace hoge
{
	void WorldView::Initialize()
	{
		m_config = WorldViewConfig{};
	}

	void WorldView::Update()
	{
	}

	void WorldView::Draw()
	{
	}


	void WorldView::SetFocus(Vec3 look_at)
	{
	}
	void WorldView::GetForward()
	{
	}
	void WorldView::GetRight()
	{
	}
	bool WorldView::IfRotating()
	{
		return false;
	}
	bool WorldView::StartRotation(int direction)
	{
		return false;
	}
}