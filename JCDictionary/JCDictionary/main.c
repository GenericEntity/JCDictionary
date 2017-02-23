#include <stdio.h>
#include "JCHashMap.h"
#include "MemoryLeak.h"

int main(void)
{
    HashMap *nodes[7];
    int i;
    for (i = 0; i < _countof(nodes); ++i) {
        nodes[i] = JCHM_Construct();
    }

    JCHM_Add(nodes[0], 'o', nodes[1]);
    JCHM_Add(nodes[0], 'r', nodes[2]);

    JCHM_Add(nodes[1], 'g', nodes[3]);
    JCHM_Add(nodes[1], 't', nodes[4]);

    JCHM_Add(nodes[2], 'a', nodes[5]);
    JCHM_Add(nodes[5], 'g', nodes[6]);


	char buf[100];
	printf("\nPress ENTER to continue... ");
	gets_s(buf, sizeof buf);

    for (i = 0; i < _countof(nodes); ++i) {
        JCHM_Destruct(nodes[i]);
    }

	_CrtDumpMemoryLeaks();

	return 0;
}


