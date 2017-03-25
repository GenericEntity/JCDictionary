#include "JCDictionary.h"
#include "MemoryLeak.h"
#include <stdbool.h>
#include "JCHashMap.h"
#include <assert.h>
#include <string.h>


typedef struct JCDictionary {
    HashMap *head;
    int wordCount;
} Dictionary;

Dictionary* JCD_Construct(void) {
    Dictionary *dict = (Dictionary*)malloc(sizeof(Dictionary) * 1);
    if (dict == NULL) { return NULL; }

    dict->head = JCHM_Construct();
    if (dict->head == NULL) {
        free(dict);
        return NULL;
    }

    dict->wordCount = 0;

    return dict;
}

void JCD_Clear(Dictionary *this) {
    assert(this);
    JCHM_ClearAndDestructItems(this->head);
    this->wordCount = 0;
}

void JCD_Destruct(Dictionary *this) {
    assert(this);
    JCHM_DestructWithItems(this->head);
    free(this);
}

bool JCD_AddWord(Dictionary *this, const char *word) {
    assert(this);
    assert(word);

    const char *ch = word;
    HashMap *node = this->head;

    // Calculate the number of nodes to add
    int numToAdd = strlen(word);
    while (*ch != '\0') {
        if (!JCHM_Contains(node, *ch)) { break; }
        node = JCHM_Get(node, *ch);
        assert(node);
        numToAdd--;
        ch++;
    }

    assert(numToAdd >= 0);

    // If all letters of the new word already exist...
    if (numToAdd == 0) {
        // It's new if the last node is not marked as the end.
        bool isNewWord = !JCHM_IsEndOfWord(node);
        if (isNewWord) {
            JCHM_SetIsEndOfWord(node, true);
        }
        // Report if the new word was added successfully.
        return isNewWord;
    }

    // This array of HashMap* is used to track allocated nodes in case
    // we need to abort the operation and free the allocated stuff.
    // calloc because we need NULL to represent unused when freeing
    // number of nodes to add
    HashMap **nodesAlloc = (HashMap**)calloc(numToAdd, sizeof(HashMap*));
    if (nodesAlloc == NULL) { return false; }


    // Keep track of where to put the next allocated node
    HashMap **iterator = nodesAlloc;

    // Keep track of the parent of the nodes to be added.
    HashMap *parent = node;
    // And the key of the direct child of the parent.
    char dirChildKey = *ch;

    bool successful = true;

    // For each character that there isn't a node, make one
    // and add it to its appropriate parent.
    while (*ch != '\0') {
        HashMap *toAdd = JCHM_Construct();
        if (toAdd == NULL || !JCHM_Add(node, *ch, toAdd)) {
            if (toAdd != NULL) { free(toAdd); }
            successful = false;
            break;
        }

        node = toAdd;
        *iterator = toAdd;
        iterator++;
        ch++;
    }

    if (successful) {
        JCHM_SetIsEndOfWord(node, true);
        this->wordCount++;
    } else {
        JCHM_Remove(parent, dirChildKey);
        int i;
        for (i = 0; i < numToAdd; ++i) {
            if (nodesAlloc[i] != NULL)
                JCHM_Destruct(nodesAlloc[i]);
        }
    }

    free(nodesAlloc);
    return successful;
}

bool JCD_RemoveWord(Dictionary *this, const char *word) {
    assert(this);
    assert(word);

    //this->wordCount--;
    return false;
}

bool JCD_ContainsWord(const Dictionary *this, const char *word) {
    assert(this);
    assert(word);

    const char *c = word;
    HashMap *node = this->head;

    // Traverse down the hashmap tree, checking that there is a node
    // for each char in word.
    while (*c != '\0') {
        node = JCHM_Get(node, *c);
        if (node == NULL) { return false; }
        c++;
    }

    // If there's a corresponding node for all letters, then the final
    // node must be marked the end of a word.
    return JCHM_IsEndOfWord(node);
}

int JCD_GetCount(const Dictionary *this) {
    assert(this);
    return this->wordCount;
}



//typedef struct JCDDynamicArray DynArray;
//
//DynArray* JCDDA_ConstructDefault();
//
//DynArray* JCDDA_Construct1(int capacity);
//
//DynArray* JCDDA_Construct2(int capacity, double growthRate);
//
//DynArray* JCDDA_Construct3(int capacity, double growthRate,
//                           double shrinkPercThreshold);
//
//DynArray* JCDDA_Construct4(int capacity, double growthRate,
//                           double shrinkPercThreshold,
//                           double shrinkPercentage);
//
//void JCDDA_Destruct(DynArray *this);
//
//HashMap** JCDDA_GetArray(const DynArray *this);
//
//int JCDDA_GetSize(const DynArray *this);
//
//bool JCDDA_SetSize(DynArray *this, int size);
//
//bool JCDDA_IncrSize(DynArray *this, int amt);
//
//bool JCDDA_DecrSize(DynArray *this, int amt);


//typedef struct JCDDynamicArray {
//    HashMap **values;
//    int size;
//    int capacity;
//    double growthRate;
//    double shrinkPT;
//    double shrinkPerc;
//} DynArray;
//
//DynArray* JCDDA_ConstructDefault() {
//    return JCDDA_Construct4(4, 1.5, 0.0, 0.5);
//}
//
//DynArray* JCDDA_Construct1(int capacity) {
//    return JCDDA_Construct4(capacity, 1.5, 0.0, 0.5);
//}
//
//DynArray* JCDDA_Construct2(int capacity, double growthRate) {
//    return JCDDA_Construct4(capacity, growthRate, 0.0, 0.5);
//}
//
//DynArray* JCDDA_Construct3(int capacity, double growthRate, 
//                           double shrinkPercThreshold) {
//    return JCDDA_Construct4(capacity, 
//                           growthRate, 
//                           shrinkPercThreshold, 
//                           shrinkPercThreshold);
//}
//
//DynArray* JCDDA_Construct4(int capacity, double growthRate, 
//                           double shrinkPercThreshold, 
//                           double shrinkPercentage) {
//    assert(capacity >= 0);
//    assert(growthRate >= 1.0);
//    assert(shrinkPercThreshold >= 0.0 && shrinkPercThreshold <= 1.0);
//    assert(shrinkPercentage >= 0.0 && shrinkPercentage <= 1.0 &&
//           shrinkPercentage >= shrinkPercThreshold);
//
//    DynArray *this = (DynArray*)malloc(sizeof(DynArray) * 1);
//    if (this == NULL) { return NULL; }
//
//    if (capacity == 0) {
//        this->values = NULL;
//    } else {
//        this->values = (HashMap**)malloc(sizeof(HashMap*) * capacity);
//        if (this->values == NULL) {
//            free(this);
//            return NULL;
//        }
//    }
//
//    this->size = 0;
//    this->capacity = capacity;
//    this->growthRate = growthRate;
//    this->shrinkPT = shrinkPercThreshold;
//    this->shrinkPerc = shrinkPercentage;
//
//    return this;
//}
//
//void JCDDA_Destruct(DynArray *this) {
//    assert(this);
//    free(this->values);
//    free(this);
//}
//
//HashMap** JCDDA_GetArray(const DynArray *this) {
//    assert(this);
//    return this->values;
//}
//
//int JCDDA_GetSize(const DynArray *this) {
//    assert(this);
//    return this->size;
//}
//
//bool JCDDA_SetSize(DynArray *this, int size) {
//    assert(this);
//    assert(size >= 0);
//
//    if (size > this->capacity) {
//        // Try to expand
//        int newCapacity = (int)(this->capacity * this->growthRate);
//        if (newCapacity == this->capacity) { newCapacity = this->capacity + 1; }
//
//        HashMap **newArray = (HashMap**) realloc(
//            this->values, sizeof(HashMap*) * newCapacity);
//
//        if (newArray == NULL) { return false; }
//
//        this->capacity = newCapacity;
//        this->values = newArray;
//    } else if (size < (int)(this->capacity * this->shrinkPT)) {
//        // Try to contract if the new size is less than threshold of capacity
//        // It's okay if it (somehow) fails to shrink, we can still use it.
//        int newCapacity = (int)(this->capacity * this->shrinkPT);
//        HashMap **newArray = (HashMap**) realloc(
//            this->values, sizeof(HashMap*) * newCapacity);
//
//        if (newArray != NULL) {
//            this->capacity = newCapacity;
//            this->values = newArray;
//        }
//    }
//
//    this->size = size;
//
//    return true;
//}
//
//bool JCDDA_IncrSize(DynArray *this, int amt) {
//    assert(this);
//    return JCDDA_SetSize(this, this->size + amt);
//}
//
//bool JCDDA_DecrSize(DynArray *this, int amt) {
//    assert(this);
//    return JCDDA_SetSize(this, this->size - amt);
//}

