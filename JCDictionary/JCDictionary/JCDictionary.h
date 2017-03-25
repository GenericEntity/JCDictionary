#pragma once
#include <stdbool.h>

typedef struct JCDictionary Dictionary;

Dictionary* JCD_Construct(void);

void JCD_Clear(Dictionary *this);

void JCD_Destruct(Dictionary *this);

bool JCD_AddWord(Dictionary *this, const char *word);

bool JCD_RemoveWord(Dictionary *this, const char *word);

bool JCD_ContainsWord(const Dictionary *this, const char *word);

int JCD_GetCount(const Dictionary *this);

