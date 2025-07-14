#pragma once

namespace hoge
{
	class Application
	{
	public:
		void Initialize(const wchar_t* title);
		void Run();
		void Finalize();
	private:
		void Update();
		void Draw();
	};
}