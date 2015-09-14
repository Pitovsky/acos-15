#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>

extern int parser();//обработчик URL (см. файл parser.cpp)

int main(int argc, char **argv, char **envp)
{
    /*получение значения необходимой переменной окружения URLS_SRC
    (она должна содержать имя pipe-канала*/
    char *urls_src;
    char cwd[PATH_MAX];
    getcwd(cwd,PATH_MAX);//Найдем имя текущего каталога

    //проверим, определена ли переменная URLS_SRC
    if((urls_src = getenv("URLS_SRC")) == NULL) {
        //если переменная не определена, получим от пльзователя её значение и поместим переменную в окружение программы
        printf("Argument URLS_SRC don't exists, enter its value: ");

        setenv("URLS_SRC", strcat(cwd,"/1.fifo"), 1); //установка значения переменной окружения (для переносимости кода используем текущий каталог пользователя)
        urls_src = getenv("URLS_SRC");
    } else {
        printf("URLS_SRC = %s\n", urls_src);
    }
    printf("%s\n", getenv("URLS_SRC"));

    /*откроем необходимый pipe-канал*/
    int fd;

    int retCode = mkfifo(urls_src, 0666);//создаём fifo
    if(retCode)
        switch(errno)
        {
        case EACCES:
            fprintf(stderr,"\nmkfifo: Have no permissions to run in a directory path\n");
            return -1;
        case EEXIST:
            fprintf(stderr,"\nmkfifo: WARNING The file (or symbolic link already exsists\n");
            break;
        case ENOENT:
            fprintf(stderr,"\nmkfifo: There is no directory mentioned in %s, or dangling link\n",urls_src);
            return -1;
        case ENOSPC:
            fprintf(stderr,"\nmkfifo: no plase for creating new file\n");
            return -1;
        case ENOTDIR:
            fprintf(stderr,"\nmkfifo: One of directories mentioned in %s, actually is not such\n", urls_src);
            return -1;
        case EROFS:
            fprintf(stderr,"\nmkfifo: attempt to create FIFO file on the file system \"read-only\"\n");
            return -1;
        default:
            fprintf(stderr,"\nmkfifo: Error\n");
            return -1;
        }
    /*
     * Теперь следует "распараллелить текущий процесс. В родительском процессе следует прочитеть список URL, открыть pipe на запись
     * и построчно записать туда прочитанные URL...
     * В дочернем процессе, одновременно с родительским следует открыть pipe на чтение, считать построчно URL и обработать их в
     * соответствии с заданием. В качестве дочернего процесса можно использовать код в файле parser.cpp (подпрограмма int parser())
     */
    int PID;
    if(PID=fork()>0) {//дублируем текущий процесс...
    					// PID>0 - Это родительский процесс!!!
        printf("\n\nParent process PID=%d\n", PID);

        fd = open(urls_src, O_WRONLY);//открываем канал только для записи
        //обработка возможных ошибок
            if(fd < 0) {
                perror("Error with opening fifo\n");
                exit(-1);
        }

        char u[PATH_MAX];//Определим буфер для URL
/*
 * Поместим URL в буфер... (сделано для простоты. А можно организовать ввод с клавиатуры, как вариант...)
 * В нашем случае это будет ссылка на файл local.html в текущем каталоге вида:
 *      file://PATH_TO_CURRENT_DIR/local.html
 */
        strcat(strcat(strcpy(u,"file://"),getcwd(cwd,PATH_MAX)), "/local.html");
        write(1, u, strlen(u));//выведем это на экран для отладки...

        write(fd, u, strlen(u));//Передадим дочернему процессу через pipe...

        close(fd);
    } else {
        printf("\n\nMessage from child process PID=%d\n", PID);
        parser();
    }
    return 0;
}
