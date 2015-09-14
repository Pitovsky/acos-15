#ifndef ALLOC_INT_H_INCLUDED
#define ALLOC_INT_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "alloc.h"

// буффер
typedef struct SBuffer {
    void *address;
    unsigned int size;
    char isFree;
} SBuffer;

// кусочек памяти (1 Мб, например)
typedef struct SPiece {
    SBuffer buffers[16];
    char used;
} SPiece;

// создание куска памяти
void piece_create();

// переформирование буфферов
void buffers_reform();

// очистка пустых кусков памяти
void piece_autoclean();

// сравнение двух буфферов по адресу
int buffer_compare(const void* a, const void* b);

// число кусков
unsigned int piece_count;

// указатель на первый кусок
void * piece_first;

#endif // LIST_H_INCLUDED
