//
//  hash.c
//  hash
//
//  Created by Danil Tulin on 3/1/15.
//  Copyright (c) 2015 Danil Tulin. All rights reserved.
//

#include "hash.h"

static const int MEM_ALLOCATION_ERROR = 101;

void throwException(int code)
{
    switch (code)
    {
        case MEM_ALLOCATION_ERROR:
            printf("\n---------ERROR---------\nMemory allocarion problem\n---------ERROR---------\n");
            break;
            
        default:
            break;
    }
    
    exit(EXIT_FAILURE);
}

Hashtable *createHashtable(NSUInteger size)
{
    Hashtable *_hashtable = NULL;
    
    if(!size)
    {
        return NULL;
    }
    
    if(!(_hashtable = malloc(sizeof(Hashtable))))
    {
        throwException(MEM_ALLOCATION_ERROR);
    }
    
    if(!(_hashtable->table = malloc(sizeof(Element *) * size)))
    {
        throwException(MEM_ALLOCATION_ERROR);
    }
    
    for(NSInteger i = 0; i < size; i++ )
    {
        _hashtable->table[i] = NULL;
    }
    
    _hashtable->size = size;
    return _hashtable;
}

NSUInteger hash(Hashtable *hashtable, char *key)
{
    return (NSUInteger)strlen(key);
}

Element *newElement(char *key, char *value)
{
    Element *newpair;
    
    if(!(newpair = malloc(sizeof(Element))))
    {
        throwException(MEM_ALLOCATION_ERROR);
    }
    
    if(!(newpair->key = strdup(key)))
    {
        throwException(MEM_ALLOCATION_ERROR);
    }
    
    if(!(newpair->value = strdup(value)))
    {
        throwException(MEM_ALLOCATION_ERROR);
    }
    
    newpair->next = NULL;
    return newpair;
}

void setValue(Hashtable *hashtable, char *key, char *value)
{
    Element *_element = NULL;
    Element *next = NULL;
    Element *last = NULL;
    
    NSInteger hashKey = hash(hashtable, key);
    
    next = hashtable->table[hashKey];
    
    while(next && next->key && strcmp(key, next->key) > 0)
    {
        last = next;
        next = next->next;
    }
    
    if(next && next->key && strcmp(key, next->key) == 0)
    {
        free(next->value);
        next->value = strdup(value);
    }
    else
    {
        _element = newElement(key, value);
        if(next == hashtable->table[hashKey])
        {
            _element->next = next;
            hashtable->table[hashKey] = _element;
        }
        else if (!next)
        {
            last->next = _element;
        }
        else
        {
            _element->next = next;
            last->next = _element;
        }
    }
}

Element *search(Hashtable *hashtable, char *key)
{
    Element *_element;
    
    NSUInteger hashKey = hash(hashtable, key);
    
    _element = hashtable->table[hashKey];
    
    while(_element && _element->key && strcmp(key, _element->key) > 0)
    {
        _element = _element->next;
    }
    
    if(!_element || _element->key == NULL || strcmp(key, _element->key))
    {
        return NULL;
    }
    else
    {
        return _element;
    }
}

void deleteValue(Hashtable *hashtable, char *key)
{
    Element *_element;
    Element *prevElement = NULL;
    
    NSUInteger hashKey = hash(hashtable, key);
    
    _element = hashtable->table[hashKey];
    
    while(_element && _element->key && strcmp(key, _element->key) > 0)
    {
        prevElement = _element;
        _element = _element->next;
    }
    
    if(!_element || _element->key == NULL || strcmp(key, _element->key))
    {
        return;
    }
    
    if (prevElement)
    {
        prevElement->next = _element->next;
    }
    
    free(_element);
}

char *getValue(Hashtable *hashtable, char *key)
{
    Element *_element = search(hashtable, key);
    if (_element)
        return _element->value;
    else
        return NULL;
}

void releaseHashtable(Hashtable *hashtable)
{
    for (NSInteger i = 0; i<hashtable->size; i++)
    {
        Element *ptr = *(hashtable->table + i);
        if (ptr)
        {
            Element *prevElement = NULL;
            do
            {
                prevElement = ptr;
                ptr = ptr->next;
                free(prevElement);
            } while (ptr);
            free(ptr);
        }
    }
}