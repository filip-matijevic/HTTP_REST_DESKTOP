#include <stdio.h>
#include "httpHost.h"

int main(int argc, const char* argv[])
{
	connectionLoop(argv[1]);
	system("pause");
}