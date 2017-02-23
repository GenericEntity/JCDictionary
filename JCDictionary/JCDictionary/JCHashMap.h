#pragma once
#include <stdbool.h>

typedef struct JCHashMap HashMap;

bool JCHM_Add(HashMap *map, char key, HashMap *value);

void JCHM_Clear(HashMap *map);

HashMap* JCHM_Construct(void);

bool JCHM_Contains(const HashMap *map, char key);

void JCHM_Destruct(HashMap *map);

HashMap *JCHM_Get(const HashMap *map, char key);

int JCHM_GetCount(const HashMap *map);

bool JCHM_Remove(HashMap *map, char key);
