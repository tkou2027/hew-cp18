#include "application.h"

int main()
{
	applicationInit(80, 25, L"HAL Event Week 2024");
	applicationRun();
	applicationDestroy();
	return 0;
}