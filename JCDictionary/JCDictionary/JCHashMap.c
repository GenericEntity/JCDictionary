#include "JCHashMap.h"
#include "MemoryLeak.h"
#include <assert.h>

typedef struct JCHashMapKeyValuePair KeyValuePair;
typedef struct JCHashMap HashMap;

typedef struct JCLinkedList List;
bool JCLL_Add(List *list, KeyValuePair *item);
void JCLL_Clear(List *list);
List* JCLL_Construct(void);
bool JCLL_Contains(const List *list, KeyValuePair *item);
bool JCLL_ContainsKey(const List *list, char key);
void JCLL_Destruct(List *list);
void JCLL_Foreach(const List *list, void(*action)(KeyValuePair *item));
KeyValuePair *JCLL_GetByKey(const List *list, char key);
int JCLL_GetCount(const List *list);
bool JCLL_IsEmpty(const List *list);
bool JCLL_RemoveByKey(List *list, char key);


typedef struct JCHashMapKeyValuePair {
    char key;
    HashMap *value;
} KeyValuePair;

typedef struct JCHashMap {
    List *buckets[4];
    int(*hashFn)(char);
} HashMap;

int _JCHM_GetHashCode(char c) {
    return (int)c;
}

void _JCHM_DestructKVP(KeyValuePair *kvp) {
    free(kvp);
}

int _JCHM_GetIndex(const HashMap *map, char key) {
    return map->hashFn(key) % _countof(map->buckets);
}

bool JCHM_Add(HashMap *map, char key, HashMap *value) {
    assert(map);
    
    KeyValuePair *kvp = (KeyValuePair*)malloc(sizeof(KeyValuePair) * 1);
    if (kvp == NULL) { return false; }
    kvp->key = key;
    kvp->value = value;

    return JCLL_Add(map->buckets[map->hashFn(key) % _countof(map->buckets)], kvp);
}

void JCHM_Clear(HashMap *map) {
    assert(map);

    // Destroy all entries and empty all buckets
    int i;
    for (i = 0; i < _countof(map->buckets); ++i) {
        JCLL_Foreach(map->buckets[i], _JCHM_DestructKVP);
        JCLL_Clear(map->buckets[i]);
    }
}

HashMap* JCHM_Construct(void) {
	HashMap *map = (HashMap*)malloc(sizeof(HashMap) * 1);
	if (map == NULL) { return NULL; }

    // Initialize hash function
    map->hashFn = _JCHM_GetHashCode;

	// Create buckets
	int i;
	for (i = 0; i < _countof(map->buckets); ++i) {
		map->buckets[i] = JCLL_Construct();

		// If any bucket fails to create, destroy all the buckets before it (backwards).
		if (map->buckets[i] == NULL) {
			for (i = i-1; i >= 0; --i) {
				JCLL_Destruct(map->buckets[i]);
			}
			free(map);
			return NULL;
		}
	}

	return map;
}

bool JCHM_Contains(const HashMap *map, char key) {
    assert(map);
    int index = _JCHM_GetIndex(map, key);
    return JCLL_ContainsKey(map->buckets[index], key);
}

void JCHM_Destruct(HashMap *map) {
    JCHM_Clear(map);

	// Destroy all buckets
	int i;
	for (i = 0; i < _countof(map->buckets); ++i) {
		JCLL_Destruct(map->buckets[i]);
	}

	free(map);
}

HashMap* JCHM_Get(const HashMap *map, char key) {
    assert(map);

    int index = _JCHM_GetIndex(map, key);
    KeyValuePair *kvp = JCLL_GetByKey(map->buckets[index], key);
    if (kvp == NULL) { return NULL; }
    return kvp->value;
}

int JCHM_GetCount(const HashMap *map) {
    assert(map);

    int count = 0;
    int i;

    // Sum up all the buckets' counts.
    for (i = 0; i < _countof(map->buckets); ++i) {
        count += JCLL_GetCount(map->buckets[i]);
    }

    return count;
}

bool JCHM_Remove(HashMap *map, char key) {
    assert(map);

    int index = _JCHM_GetIndex(map, key);
    return JCLL_RemoveByKey(map->buckets[index], key);
}



/*****************************************
* Hashmap internal bucket implementation *
*****************************************/

typedef struct JCLLNode Node;

typedef struct JCLLNode {
    KeyValuePair *value;
    Node *next;
} Node;

typedef struct JCLinkedList {
    Node *head;
    int count;
} List;

List* JCLL_Construct(void) {
    List *list = (List*)malloc(sizeof(List) * 1);
    if (list == NULL) { return NULL; }

    list->head = NULL;
    list->count = 0;
    return list;
}

bool JCLL_Contains(const List *list, KeyValuePair *item) {
    assert(list);

    Node *node = list->head;
    while (node != NULL) {
        if (node->value == item) {
            return true;
        }
        node = node->next;
    }

    return false;
}

bool JCLL_ContainsKey(const List *list, char key) {
    assert(list);

    Node *node = list->head;
    while (node!=NULL) {
        if (node->value->key == key) {
            return true;
        }
        node = node->next;
    }

    return false;
}

void JCLL_Destruct(List *list) {
    JCLL_Clear(list);
    free(list);
}

bool JCLL_Add(List *list, KeyValuePair *item) {
    assert(list);

    Node *newNode = (Node*)malloc(sizeof(Node) * 1);
    newNode->value = item;
    newNode->next = NULL;
    if (newNode == NULL) { return false; }

    if (list->head == NULL) {
        list->head = newNode;
    } else {
        // Find the end of the list
        Node *node = list->head;
        while (node->next != NULL) { node = node->next; }
        // Attach new node to the end
        node->next = newNode;
    }

    list->count++;
    return true;
}

bool JCLL_RemoveByKey(List *list, char key) {
    assert(list);

    Node *node = list->head;
    Node *prev = NULL;

    // Walk through the list, looking for a match
    while (node != NULL) {
        // Found it! Remove and report success.
        if (node->value->key == key) {
            Node *next = node->next;
            if (prev != NULL) {
                prev->next = next;
            }
            free(node);
            list->count--;

            return true;
        }

        prev = node;
        node = node->next;
    }

    // If we got here, we failed to find a match.
    return false;
}

void JCLL_Clear(List *list) {
    assert(list);

    Node *node = list->head;
    Node *next;

    while (node != NULL) {
        // Keep a handle on the next node
        next = node->next;
        // Destroy the node (replace with function if more complicated)
        free(node);
        // Move on to the next node
        node = next;
    }

    list->head = NULL;
    list->count = 0;
}

int JCLL_GetCount(const List *list) {
    assert(list);
    return list->count;
}

bool JCLL_IsEmpty(const List *list) {
    assert(list);
    return list->head == NULL;
}

KeyValuePair *JCLL_GetByKey(const List *list, char key) {
    assert(list);

    Node *node = list->head;

    while (node != NULL) {
        if (node->value->key == key) {
            return node->value;
        }
        node = node->next;
    }

    return NULL;
}

void JCLL_Foreach(const List *list, void(*action)(KeyValuePair *item)) {
    assert(list);
    assert(action);
    if (list->head == NULL) { return; }

    Node *node = list->head;
    while (node != NULL) {
        action(node->value);
        node = node->next;
    }
}

#undef T