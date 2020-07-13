#include <stdio.h>
#include "httpHost.h"

int main(int argc, const char* argv[])
{
	if (argc < 2) {

		connectionLoop("80");
	}
	else {
		connectionLoop(argv[1]);
	}
	system("pause");
}