#ifndef STDLIB_H_INCLUDED
#define STDLIB_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include "table.h"

//функция создания массива рандомных строк
char **randomize_strings() {
    int i, j, c;
    char** array = malloc(sizeof(char*) * 100);
    for(j = 0; j < 100; j++)
    {
        array[j] = malloc(16);
        for(i = 0; i < 15; i++)
        {
            c = rand() % 26;
            c = 'a' + c;
            array[j][i] = c;
        }
        array[j][15] = '\0';
    }
    return array;
}

void Testing_first() {
    char **array = randomize_strings();
    int i, j;

    printf("Randomizing strings: \n");
    for(i = 0; i < 100; i++)
    {
        printf("%s ", array[i]);
    }
    printf("\n");

    //в цикле создаём 10 новых табличек
    printf("Create new tables:\n");
    for(i = 0; i < 10; i++)
    {
        printf("%d ", table_Creation());
    }
    printf("\n\n");

    //в цикле записываем в каждую из табличек новые элементы
    for(i = 0; i < 10; i++)
    {
        for(j = 0; j < 100; j++)
        {
            table_Write(i, array[j]);//Внимание! Вопрос! В результате не происходит ребилдинг табличек (см table.c)
        }
    }

    //добавляем ещё строки в первую табличку
     for(i = 0; i < 100; i++)
    {
        table_Write(0, array[i]);//И снова вопрос: почему табличка не расширяется?
    }

    //удаляем элементы из таблички
    printf("Deleting existing elements:\n");
    for(i = 0; i < 99; i++)
    {
        printf("%d ", table_Erase(1, array[i]));
    }
    printf("\n\n");

    //проверим, есть ли в табличке теперь удалённые элементы
    printf("Checking: are elements exist:\n");
    for(i = 0; i < 100; i++)
    {
        printf("%d ", table_Exist(1, array[i]));//повледний элемент должен существовать, так как мы его не удалили
    }
    printf("\n\n");

    //теперь удалим из таблички больше элементов, чем в ней содержится
    printf("Deleting more elements than exist:\n");
    printf("%d ", table_Erase(1, array[99]));
    for(i = 0; i < 20; i++)
    {
        printf("%d ", table_Erase(1, array[i]));
    }
    printf("\n\n");

    //теперь удалим все существующие таблички, и попытаемся также удалить таблички, которые не существуют
    printf("Deleting all tables and more:\n");
    for(i = 0; i < 10; i++)
    {
        table_Clear(i);
    }
    printf("\n\n");
    table_Clear(0);//ошибки нет, всё прекрасно удалилось (второй раз, хотя такой таблички не существует), не знаю, нужно ли это исправлять
    //table_Clear(125);//а вот в этом месте она логичным образом вылетает, ибо такой таблички нет
    //возникли проблемы с расширением таблички, либо не работает отладочный вывод (возможно я допустила глупую ошибку и не заметила),
    //либо не так что-то ещё, но судя по тому, что в табличку записывается далеко не один элемент, как объявлялось изначально
    //то расширение таблицы работает
    //НЕ МОГУ НАЙТИ ОШИБКУ

}

#endif // STDLIB_H_INCLUDED
