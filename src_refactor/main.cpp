#include <iostream>
#include <memory>

#include "application.h"
int main(void)
{
	std::unique_ptr<hoge::Application> app = std::make_unique<hoge::Application>();

	app->Initialize(L"title");
	app->Run();
	app->Finalize();

	std::cin.seekg(0);
	std::cin.get();

	return 0;
}