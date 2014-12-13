#include <stdio.h>
#include "world.h"


int main()
{
	World ourWorld;
	InitializeWorld(&ourWorld, 256, 256);
	printf("Hello World!");

	SetCell(&ourWorld, 26, 26, true);
	SetCell(&ourWorld, 26, 27, true);
	SetCell(&ourWorld, 26, 28, true);
	SetCell(&ourWorld, 27, 27, true);

	for(int test = 0; test < 101; ++test)
	{
		Update(&ourWorld);
	}
	return 0;
}
