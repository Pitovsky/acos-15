#include <stdio.h>
#include "hash.h"

int main(int argc, char **argv)
{
    
    Hashtable *hashtable = createHashtable(65536);
    
    setValue(hashtable, "key1", "inky");
    setValue(hashtable, "key2", "pinky");
    setValue(hashtable, "key3", "blinky");
    setValue(hashtable, "key4", "floyd");
    
    printf("%s\n", getValue(hashtable, "key1"));
    printf("%s\n", getValue(hashtable, "key2"));
    printf("%s\n", getValue(hashtable, "key3"));
    printf("%s\n", getValue(hashtable, "key4"));
    
    return 0;
}