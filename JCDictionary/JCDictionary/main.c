#include <stdio.h>
#include "JCDictionary.h"
#include "MemoryLeak.h"

int main(void)
{
    Dictionary *dict = JCD_Construct();

    printf("Contains Hello: %d\n", JCD_ContainsWord(dict, "Hello"));
    printf("Contains Hey: %d\n", JCD_ContainsWord(dict, "Hey"));
    printf("Contains He: %d\n", JCD_ContainsWord(dict, "He"));
    printf("Contains Hell: %d\n", JCD_ContainsWord(dict, "Hell"));

    printf("Adding Hello: %d\n", JCD_AddWord(dict, "Hello"));
    printf("Adding Hey: %d\n", JCD_AddWord(dict, "Hey"));
    printf("Adding He: %d\n", JCD_AddWord(dict, "He"));
    printf("Adding He: %d\n", JCD_AddWord(dict, "He"));
    printf("Adding He: %d\n", JCD_AddWord(dict, "He"));
    printf("Adding Hey: %d\n", JCD_AddWord(dict, "Hey"));
    printf("Adding Hell: %d\n", JCD_AddWord(dict, "Hell"));
    
    printf("Contains Hello: %d\n", JCD_ContainsWord(dict, "Hello"));
    printf("Contains Hey: %d\n", JCD_ContainsWord(dict, "Hey"));
    printf("Contains He: %d\n", JCD_ContainsWord(dict, "He"));
    printf("Contains Hell: %d\n", JCD_ContainsWord(dict, "Hell"));

	printf("Remove Hello: %d\n", JCD_RemoveWord(dict, "Hello"));
	printf("Contains Hello: %d\n", JCD_ContainsWord(dict, "Hello"));
	printf("Contains Hey: %d\n", JCD_ContainsWord(dict, "Hey"));
	printf("Contains He: %d\n", JCD_ContainsWord(dict, "He"));
	printf("Contains Hell: %d\n", JCD_ContainsWord(dict, "Hell"));
	printf("Remove Hello: %d\n", JCD_RemoveWord(dict, "Hello"));
	printf("Contains Hello: %d\n", JCD_ContainsWord(dict, "Hello"));
	printf("Contains Hey: %d\n", JCD_ContainsWord(dict, "Hey"));
	printf("Contains He: %d\n", JCD_ContainsWord(dict, "He"));
	printf("Contains Hell: %d\n", JCD_ContainsWord(dict, "Hell"));
	printf("Adding Hello: %d\n", JCD_AddWord(dict, "Hello"));
	printf("Contains Hello: %d\n", JCD_ContainsWord(dict, "Hello"));
	printf("Contains Hey: %d\n", JCD_ContainsWord(dict, "Hey"));
	printf("Contains He: %d\n", JCD_ContainsWord(dict, "He"));
	printf("Contains Hell: %d\n", JCD_ContainsWord(dict, "Hell"));

    JCD_Clear(dict);
    printf("Cleared dictionary.\n");

    printf("Contains Hello: %d\n", JCD_ContainsWord(dict, "Hello"));
    printf("Contains Hey: %d\n", JCD_ContainsWord(dict, "Hey"));
    printf("Contains He: %d\n", JCD_ContainsWord(dict, "He"));
    printf("Contains Hell: %d\n", JCD_ContainsWord(dict, "Hell"));

    JCD_Destruct(dict);
    printf("Destroyed dictionary.\n");

	char buf[100];
	printf("\nPress ENTER to continue... ");
	gets_s(buf, sizeof buf);

	_CrtDumpMemoryLeaks();

	return 0;
}


