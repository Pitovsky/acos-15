#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED

//функция создания таблицы, возвращает её порядковй номер
int table_Creation();

//функция записи данных
void table_Write(int id, char* code);

//поиск элемента
char table_Exist(int id, char* code);

//удаление элемента
char table_Erase(int id, char* code);

//удаление таблицы
void table_Clear(int id);

#endif // TABLE_H_INCLUDED
