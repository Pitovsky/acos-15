#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef unsigned long long uint64;

//хэш-функция
uint64 hash(char* code)
{
    uint64 h = 0;
    char *i;
    for (i = code; *i; i++) {
        h = h * 37 + *i - 'a' + 1;
    }
    return h;
}

typedef struct List {
    char *code;//ключ - строка
    int len;//длина строки-ключа
    uint64 h;//значение хеш-функции
    struct List *ne;//указатель на следующий элемент в цепочке
} List;

typedef struct Table {
    int N, M;//N - количество элементов в таблице, M - размер таблицы
    List** t;//массив указателей на первые элементы цепочек
} Table;

Table* T;
char T_empty = 1;//флаг, который показывает, что не была создана ещё ни одна таблица, изначально равен 1 (истина)
int T_cur = 0;//последняя заполненная позиция (количество имеющихся табличек)
int T_sz = 0; //размер выделенной памяти (под базу данных табличек)

//функция создания новой таблички в базе
void table_init(Table *table) {
    table->N = 0;
    table->M = 1;
    table->t = malloc(sizeof(List*));
    table->t[0] = NULL;
}

//функция расширения таблички
void rebuild_table(Table *ta) {
    printf("The rebuilding\n");//отладочный вывод
    List **li;//вспомогательный массив указателей на первые элементы списков
    li = malloc(ta->N * sizeof (List*));
    int i, li_cur = 0;
    for (i = 0; i < ta->M; i++) {//в цикле проходим по исходному массиву указателей на первые элементы списков
        List *it = ta->t[i];
        while (it != NULL) {
            li[li_cur++] = it;
            it = it->ne;
            li[li_cur - 1]->ne = NULL;//в результате получаем массив, в котором хранятся все элементы исходной таблицы
        }
    }
    ta->M *= 2;//увеличиваем размер таблицы в два раза
    ta->t = realloc(ta->t, ta->M * sizeof(List*));
    for(i = 0; i < ta->M; i++) {
        ta->t[i] = NULL;
    }
    for (i = 0; i < ta->N; i++) {//в цикле перезаполняем новую табличку
        li[i]->ne = ta->t[li[i]->h % ta->M];
        ta->t[li[i]->h % ta->M] = li[i];
    }
    free(li);
}

//функция записи данных в конкретную выбранную табличку
void write_in_table(Table *ta, char *code) {//ta - указатель на табличку, в которую производится запись
    if (ta->N == ta->M)
        rebuild_table(ta);//здесь происходит расширение таблички, число элементов в которой совпало с её размром
    uint64 h = hash(code);//считаем хэш от ключа
    List *it;
    it = malloc(sizeof(List));
    it->h = h;
    it->len = strlen(code);
    char *code_copy;
    code_copy = malloc(it->len + 1);
    memcpy(code_copy, code, it->len + 1);
    it->code = code_copy;
    it->ne = ta->t[h % ta->M];
    ta->t[h % ta->M] = it;
}

//функция поиска элемента в конкретном списке по полученному хэшу
char exist_in_list(List *it, uint64 h, char *code) {
    while (it) {
        if (it->h == h && strcmp(it->code, code) == 0)
            return 1;
        it = it->ne;
    }
    return 0;
}

//функция удаления элемента в конкретном списке по полученному хэшу
char erase_in_list(List **prev, List *it, uint64 h, char *code) {
    while (it) {
        if (it->h == h && strcmp(it->code, code) == 0) {
            *prev = it->ne;
            free(it);
            return 1;
        }
        prev = &it->ne;
        it = it->ne;
    }
    return 0;
}

//функция поиска элемента в конкретной выбранной табличке
char exist_in_table(Table *ta, char *code) {
    uint64 h = hash(code);
    return exist_in_list(ta->t[h % ta->M], h, code);//функция поиска элемента в конкретном списке по полученному хэшу
}

//функция которая удаляет элемент из конкретной выбранной таблички
char erase_in_table(Table *ta, char *code) {
    uint64 h = hash(code);
    return erase_in_list(&ta->t[h % ta->M], ta->t[h % ta->M], h, code);//функция удаления элемента в конкретном списке по полученному хэшу
}

void clear_list(List* it) {//функция, которая осуществляет очистку каждого конкретного списка
    if (it) {
        clear_list(it->ne);
        free(it);
    }
}

//функция, которая осуществляет удаление таблицы по порядковому номеру
void clear_table(Table* ta) {
    int i;
    for (i = 0; i < ta->M; i++)
        clear_list(ta->t[i]);//функция, которая осуществляет очистку каждого конкретного списка
    free(ta->t);
}

//функция создания таблицы, возвращает её порядковй номер
int table_Creation() {
    if (T_empty) {
        T = malloc(sizeof(Table));
        T_sz = 1;
        T_empty = 0;
    }
    if (T_cur == T_sz) {
        T_sz = T_sz * 2;
        T = realloc(T, T_sz * sizeof(Table));
    }
    table_init(&T[T_cur]);//функция создания новой таблички в базе
    return T_cur++;
}

//функция записи данных
void table_Write(int id, char* code) { //входные данные: в какую таблицу (id) и что записать (code)
    assert(0 <= id && id < T_cur);
    write_in_table(&T[id], code);//функция записи данных в конкретную выбранную табличку
}

//поиск элемента
char table_Exist(int id, char* code) {
    assert(0 <= id && id < T_cur);
    return exist_in_table(&T[id], code);//функция поиска элемента в конкретной выбранной табличке
}

//удаление элемента
char table_Erase(int id, char* code){
    assert(0 <= id && id < T_cur);
    return erase_in_table(&T[id], code);//функция которая удаляет элемент из конкретной выбранной таблички
}

//удаление таблицы
void table_Clear(int id) {
    assert(0 <= id && id < T_cur);
    clear_table(&T[id]);//функция, которая осуществляет удаление таблицы по порядковому номеру
}
