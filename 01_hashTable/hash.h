//
//  hash.h
//  hash
//
//  Created by Danil Tulin on 3/1/15.
//  Copyright (c) 2015 Danil Tulin. All rights reserved.
//

#ifndef __hash__hash__
#define __hash__hash__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int NSInteger;
typedef unsigned int NSUInteger;

struct element_s
{
    char *key;
    char *value;
    struct element_s *next;
};
typedef struct element_s Element;

struct hashtable_s
{
    NSUInteger size;
    struct element_s **table;
};
typedef struct hashtable_s Hashtable;

Hashtable *createHashtable(NSUInteger size);
void setValue(Hashtable *hashtable, char *key, char *value);
char *getValue(Hashtable *hashtable, char *key);
void deleteValue(Hashtable *hashtable, char *key);
void releaseHashtable(Hashtable *hashtable);

#endif 
