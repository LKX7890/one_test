
#include <cstdio>  
#include <cstdlib>  
#include <iostream>
#include "network.hpp"

#pragma comment(lib, "ws2_32.lib")


int main()
{
	NetWork::GetNetWorkInstall()->NetWorkInit();
	system("pause");

	return 0;
}