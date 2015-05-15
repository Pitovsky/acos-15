#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>

typedef uint64_t Hash_t;
typedef struct HashTable_t HashTable_t;
Hash_t hashString(const char* string);
HashTable_t* createHashTable(size_t nBins);
void killHashTable(HashTable_t* table);
void insertToHashTable(HashTable_t* table, const char* key, void* value);
void* findInHashTable(HashTable_t* table, const char* key);
void eraseFromHashTable(HashTable_t* table, const char* key);
size_t hashTableSize(HashTable_t* table);
