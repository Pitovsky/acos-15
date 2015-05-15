#include "hashset.h"
#include <assert.h>

#define v(x) ((void*)(size_t)(x))

int main()
{
    HashTable_t* table = createHashTable(1);

    insertToHashTable(table, "Stepan", v(15));
    eraseFromHashTable(table, "Stepan");
    assert(0 == findInHashTable(table, "Danila"));
    assert(0 == findInHashTable(table, "Stepan"));

    srand(2);
    for (int i = 0; i < 100000; i++)
    {
        char string[10] = "";
        for (int k = 0; k < 9; k++)
            string[k] = (char)(rand() % 254 + 1);
        if (!findInHashTable(table, string))
           insertToHashTable(table, string, v(i + 42));
    }

    srand(2);
    for (int i = 0; i < 100000; i++)
    {
        char string[10] = "";
        for (int k = 0; k < 9; k++)
            string[k] = (char)(rand() % 254 + 1);
        void* searchResult = findInHashTable(table, string);
        assert(0 == searchResult || v(i + 42) == searchResult);
        if (v(i + 42) == searchResult)
            eraseFromHashTable(table, string);
    }

    assert(0 == hashTableSize(table));

    killHashTable(table);
}
