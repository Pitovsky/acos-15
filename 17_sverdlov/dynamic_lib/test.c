#include "hashtable.h"
#include <time.h>

const char* rand_string = "asfqwettoiuyaasdf;lkj";

void test_insert_erase_get(Table** tbl) {
    table_clear(*tbl);
    table_insert(tbl, rand_string, 8);

    assert(table_get(tbl, rand_string) == 8);
    assert((*tbl)->filled == 1);

    table_erase(tbl, rand_string);

    assert((*tbl)->filled == 0);
    printf("test_insert_erase_get(): OK\n");
}

void test_insert(Table** tbl) {
    table_clear(*tbl);
    size_t i;
    const int ACTIONS = 1 + rand() % 1024;
    int last = -1;
    for (i = 0; i < ACTIONS; ++i) {
        int rand_int = rand();
        table_insert(tbl, rand_string, rand_int);
        last = rand_int;
    }

    assert((*tbl)->filled == 1);
    assert(table_get(tbl, rand_string) == last);
    printf("test_insert(): OK\n");
}

void run_tests(Table** tbl) {
    test_insert_erase_get(tbl);
    test_insert(tbl);

    table_insert(tbl, "asd", 8);
    table_insert(tbl, "qwer", 2);
    table_insert(tbl, "a", 3);
    table_insert(tbl, "q", 5);
    table_insert(tbl, "aksd", 8);
    table_insert(tbl, "qkwer", 2);
    table_insert(tbl, "ka", 3);
    table_insert(tbl, "kq", 5);
    printf("insertions: OK\n");

    printf("tbl->size: %d\n", (*tbl)->size);

}

int main() {
    Table* tbl;
    table_create(&tbl, 2);

    run_tests(&tbl);

    table_delete(tbl);

    return 0;
}
