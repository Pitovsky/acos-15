#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

int thrCount = 2;
int maxComs = 10;

struct ask
{
    int com;
    char path[255];//маловато будет, есть специальная константа
} coms[10];

int showDir(const char* path, int com)
{
    int ans;
    struct dirent** namelist;
    int inDirCount = scandir(path, &namelist, NULL, alphasort);
    int i = 0;
    for (i = 0; i < inDirCount; ++i)
    {
        struct stat st;
        lstat(namelist[i]->d_name, &st);
        if (S_ISDIR(st.st_mode))
        {
            char deeppath[255];
            sprintf(deeppath, "%s/%s", path, namelist[i]->d_name);
            ans = ans + showDir(deeppath, com);
        }
        else
        {
            if (com == 1)
                ++ans;
            else
                ans = ans + st.st_size;
        }
    }
    return ans;
}

static void* oneCom(void* args)
{
    while (1)//грете процессор за зря
    {
        int i = 0;
        while (i < maxComs && coms[i].com <= 0)
            ++i;
        if (i == maxComs)
            continue;
        //ясно, что тут не корректно дела обстоят
        //up mutex
        int thiscom = coms[i].com;
        char thispath[255];
        strcpy(thispath, coms[i].path);
        coms[i].com = -1;
        //down mutex
        int ans = showDir(coms[i].path, thiscom);
        printf("%d %s\t%d\n", thiscom, thispath, ans);
    }
    pthread_exit(NULL);
}


int main(int argc, char** argv)
{
    if (argc > 1)
        thrCount = atoi(argv[1]);

    int i = 0;
    for (i = 0; i < maxComs; ++i)
        coms[i].com = -1;

    pthread_t thr;
    for (i = 0; i < thrCount; ++i)
        pthread_create(&thr, NULL, oneCom, NULL);

    char com[5];
    char path[255];
    while (1)
    {
        scanf("%s", com);
        if (strcmp(com, "exit") == 0)//не корректно
            break;
        else if (strcmp(com, "co") == 0 || strcmp(com, "cs") == 0)
        {
            int thisAsk = 0;
            while (thisAsk < maxComs && coms[thisAsk].com != -1)
                ++thisAsk;
            if (thisAsk == maxComs)//нет. почему бы просто не подождать тогда
            {
                printf("too many comands, please wait...\n");
                continue;
            }
            scanf("%s", coms[thisAsk].path);
            if (strcmp(com, "co") == 0)
                coms[thisAsk].com = 1;
            else
                coms[thisAsk].com = 2;
        }

    }
}
