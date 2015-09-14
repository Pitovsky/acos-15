#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <libgen.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

// проверяет символ на допустимый
int isWordCharacter(char c) {
    return ((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z')) || ((c >= '0') && (c <= '9')) || (c == '_');
}

// ищет число строк в файле
int findWordsInFile(char * file) {
    int fd; // дескриптор файла
    char buf; // буффер, куда сливаются символы
    int wCount = 0; // число слов в файле
    int lastChar = 0; // предыдущий символ - часть слова

    // открываем файл
    fd = open(file, O_RDONLY);
    if(fd == -1) {
        return -1;
    }

    // считываем посимвольно
    while(read(fd, &buf, 1) > 0) {
        if(!isWordCharacter(buf) && lastChar) {
            ++wCount;
        }
        lastChar = isWordCharacter(buf);
    }
    // если информация в файле кончилась, но последний символ принадлежал слову
    if(lastChar) {
        ++wCount;
    }

    // закрываем файл
    close(fd);

    // возвращаем результат
    return wCount;
}

// печатает информацию о файлах в директории (шаг рекурсии)
void printDirectoryInfoLevel(char * d_name, int level, int maxLevel, int ignoreLinks)
{
    // проверка на глубину рекурсии (глубину вхождений)
    if((maxLevel > 0) && (level >= maxLevel))
        return;

    // открытие каталога
    DIR *dir;
    dir = opendir(d_name);
    if(dir == NULL)
        return;

    // переменные для работы с элементом директории
    struct dirent *directoryEntry;
    char directoryEntryName[NAME_MAX+1];
    struct stat entryStat; // информация о текущем файле/папке (любой записи в директории)

    // прохождение по списку элементов в директории
    do {
        directoryEntry = readdir(dir);

        // если мы смогли прочитать информацию об элементе директории
        if(directoryEntry != NULL) {
            // если название начинается не с "."
            if(directoryEntry->d_name[0] != '.') {
                memset(directoryEntryName, 0, NAME_MAX + 1); // очистка переменной от старого значения
                sprintf(directoryEntryName, "%s/%s", d_name, directoryEntry->d_name);
                lstat(directoryEntryName, &entryStat);

                if(ignoreLinks && S_ISLNK(entryStat.st_mode)) {
                    // игнорируем вывод ссылки, если он необходим
                } else {
                    // пишем информацию о элементе директории
                    printf("[");
                    printf((S_ISDIR(entryStat.st_mode)) ? "d" : "-"); // пишем d, если это директория
                    printf((S_ISLNK(entryStat.st_mode)) ? "l" : "-"); // пишем l, если это ссылка
                    printf("] %s: %d wrd\n", directoryEntryName, findWordsInFile(directoryEntryName));

                    // recursion
                    printDirectoryInfoLevel(directoryEntryName, 1+level, maxLevel, ignoreLinks);
                }
            }
        }
    } while(directoryEntry != NULL);

    closedir(dir);
}

// функция, запускающая механизм рекурсии
void printDirectoryInfo(char * directoryName, int maxLevel, int ignoreLinks)
{
    printDirectoryInfoLevel(directoryName, 0, maxLevel, ignoreLinks);
}

// вывод справки по использованию программы
void printHelp(char * pName) {
    printf("USAGE: %s dir [-r <uint>] [-s]\n", basename(pName));
    printf("Выводит список слов в файлах внутри директории.\n");
    printf("\tdir - директория\n");
    printf("\t-r <uint> - число вхождений в поддиректории. 0 - не ограничено.\n");
    printf("\t-s - отключает переход по символьным ссылкам\n");
    printf("\n");
}

// точка входа в программу
int main(int argc, char ** argv)
{
    // если мы не ввели даже название директории, то показать справку
    if (argc < 2) {
        printHelp(argv[0]);
        return 1;
    }

    // объявляем переменные для аргументов
    int ignoreLinks = 0;
    int maxLevel = 0;

    // поиск необязательных аргументов
    int i;
    for(i = 2; i < argc; i++) {
        if(strcmp(argv[i], "-r") == 0) {
            // если аргумент -r <uint>

            // если за -r не следует число
            if(1+i >= argc) {
                printHelp(argv[0]);
                return 1;
            }

            // если следующий аргумент на -r "-s"
            if(strcmp(argv[1+i], "-s") == 0) {
                printHelp(argv[0]);
                return 1;
            }

            // получаем максимальное число вхождений
            maxLevel = atoi(argv[1+i]);
        } else if(strcmp(argv[i], "-s") == 0) {
            // если аргумент -s
            ignoreLinks = 1;
        }
    }

    // запуск механизма
    printDirectoryInfo(argv[1], maxLevel, ignoreLinks);

    // возврат успешного выполнения
    return 0;
}
