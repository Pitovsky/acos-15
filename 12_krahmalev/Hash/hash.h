#include <stdlib.h>

#include <stdio.h>

#include <string.h>

int hash(char* v);

struct list* Add(struct list* ,char*, int);

void deleteElement(struct list** , char* );

void deleteTable(struct list** );

int* Search(char*, struct list**);
