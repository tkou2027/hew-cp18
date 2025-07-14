#include "application.h"

#include <iostream>
int main(void)
{
	hoge::Application* app = new hoge::Application();

	app->Initialize(L"title");
	app->Run();
	app->Finalize();

	delete app;

	std::cin.seekg(0);
	std::cin.get();

	return 0;
}