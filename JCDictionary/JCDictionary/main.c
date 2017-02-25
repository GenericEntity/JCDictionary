#include <stdio.h>
#include "JCDictionary.h"
#include "MemoryLeak.h"

int main(void)
{
    Dictionary *dict = JCD_Construct();

    JCD_AddWord(dict, "Hello");
    JCD_AddWord(dict, "Hey");
    
    JCD_Destruct(dict);

	char buf[100];
	printf("\nPress ENTER to continue... ");
	gets_s(buf, sizeof buf);

	_CrtDumpMemoryLeaks();

	return 0;
}


