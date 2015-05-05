#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

int sendall(int sfd, const void* buf, int len, int flags)
{
    int total = 0;
    while(total < len)
    {
        int n = send(sfd, buf + total, len - total, flags);
        if(n == -1)
            return -1;
        total += n;
    }

    return total;
}
int recvall(int sfd, void* buf, int len, int flags)
{
    int total = 0;
    while(total < len)
    {
        int n = recv(sfd, buf + total, len - total, flags);
        if(n == -1)
            return -1;
        total += n;
    }
    return total;
}
int sendproc(int sfd, const void* buf, int len, int flags)
{
    int total = 0;
    printf("\n");
    while(total < len)
    {
        int n = send(sfd, buf + total, len - total, flags);
        if(n == -1)
            return -1;
        total += n;
        printf("\r%d%%", total*100/len);
        fflush(stdout);
    }
    printf("\n");
    return total;
}
int recvproc(int sfd, void* buf, int len, int flags)
{
    int total = 0;
    printf("\n");
    while(total < len)
    {
        int n = recv(sfd, buf + total, len - total, flags);
        if(n == -1)
            return -1;
        total += n;
        printf("\r%d%%", total*100/len);
        fflush(stdout);
    }
    printf("\n");
    return total;
}

struct sockaddr_in thisAddr;
int sockfd;
int maxFileSize = 32*1024*1024;

static void *oneUserWork(void* sockfrom)
{
    pthread_t sockfd = *((pthread_t*)sockfrom);
    int namelen = 0;
    char filename[255];
    char com[4];
    int filesize = 0;
    char infile[maxFileSize];
    while(1)
    {
        connect(sockfd, (struct sockaddr *)&thisAddr, sizeof(thisAddr));
        if (recv(sockfd, com, 4, 0) <= 0)
        {
            printf("one user disconnected!\n");
            break;
        }
        if (strcmp(com, "add") == 0)
        {
            recvall(sockfd, (void*)&namelen, sizeof(int), 0);
            recvall(sockfd, filename, namelen, 0);
            filename[namelen] = 0;
            printf("added file: %s\n", filename);
            recvall(sockfd, (void*)&filesize, sizeof(int), 0);
            int i;
            recvproc(sockfd, infile, filesize, 0);
           // for (i = 0; i < filesize; ++i)
          //      while (recv(sockfd, infile + i, 1, 0) != 1);
            printf("(loaded)\n");

            FILE* newFile = fopen(filename, "wb");
            fwrite(infile, sizeof(char), filesize, newFile);
            fclose(newFile);
        }
        else if (strcmp(com, "all") == 0)
        {
            struct dirent** namelist;
            int count = scandir(".", &namelist, NULL, alphasort);
            int i = 0;
            sendall(sockfd, (void*)&count, sizeof(int), 0);
            for (i = 0; i < count; ++i)
            {
                int len = strlen(namelist[i]->d_name);
                sendall(sockfd, (void*)&len, sizeof(int), 0);
                sendall(sockfd, namelist[i]->d_name, len, 0);
            }
        }
        else if (strcmp(com, "get") == 0)
        {
            recvall(sockfd, (void*)&namelen, sizeof(int), 0);
            recvall(sockfd, filename, namelen, 0);
            filename[namelen] = 0;
            printf("get ask for file: %s\n", filename);
            struct stat st;
            stat(filename,&st);
            sendall(sockfd, (void*)&st.st_size, sizeof(int), 0);
            if (st.st_size > 0) //file exist, send it
            {
                FILE* sendFile = fopen(filename, "rb");
                char infile[maxFileSize];
                fread(infile, sizeof(char), st.st_size, sendFile);
                fclose(sendFile);
                sendproc(sockfd, infile, st.st_size, 0);
            }
        }
    }
    pthread_exit(NULL);
}


int main(int argc, char** argv)
{
    if (argc < 3 || (strcmp(argv[1], "client") != 0 && strcmp(argv[1], "server") != 0)
                 || (strcmp(argv[1], "client") == 0 && (argc < 4 || inet_aton(argv[3], &(thisAddr.sin_addr)) == 0)))
    {
        fprintf(stderr, "Usage: %s <client|server> <port> [ip]\n", argv[0]);
        return 1;
    }
    char* mode = argv[1];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    thisAddr.sin_family = AF_INET;
    thisAddr.sin_port = htons(atoi(argv[2]));

    if (strcmp(mode, "server") == 0)
    {
        thisAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        bind(sockfd, (struct sockaddr*)&thisAddr, sizeof(thisAddr));
        listen(sockfd, 10);

        printf("server OK!\n");
        while(1)
        {
            int sockFrom;
            sockFrom = accept(sockfd, NULL, NULL);
            printf("new user connected!\n");

            pthread_t oneGetThr;
            pthread_create(&oneGetThr, NULL, oneUserWork, (void*)&sockFrom);
        }
    }
    else
    {
        //thisAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        char com[4];
        while (1)
        {
            scanf("%s", com);
            if (strcmp(com, "exit") == 0)
                break;
            else if (strcmp(com, "help") == 0)
            {
                printf(" add filename\n");
                printf(" get filename\n");
                printf(" all\n");
                printf(" exit\n");
            }
            else if (strcmp(com, "add") == 0)
            {
                char name[255];
                char infile[maxFileSize];
                scanf("%s", name);
                FILE* sendFile = fopen(name, "rb");
                if (sendFile == NULL)
                {
                    printf("Error: cannot read this file: %s.\n", name);
                    continue;
                }
                int filesize = 0;
                while (filesize < maxFileSize && fread(infile + filesize, sizeof(char), 1, sendFile) == 1)
                    ++filesize;
                if (filesize == maxFileSize)
                    printf("File too big, cutted by %d bytes.\n", filesize);
                fclose(sendFile);

                connect(sockfd, (struct sockaddr*)&thisAddr, sizeof(thisAddr));
                int i = strlen(name) - 1;
                while (i > 0 && name[i] != '/')
                    --i;
                char* shortname = name + i; //for correct filename without path
                int namelen = strlen(shortname);
                sendall(sockfd, com, 4, 0);
                sendall(sockfd, (void*)&namelen, sizeof(int), 0);
                sendall(sockfd, shortname, namelen, 0);
                sendall(sockfd, (void*)&filesize, sizeof(int), 0);
                sendproc(sockfd, infile, filesize, 0);
                printf("file successful sended.\n");
            }
            else if (strcmp(com, "get") == 0)
            {
                char name[255];
                char infile[maxFileSize];
                scanf("%s", name);
                int namelen = strlen(name);
                sendall(sockfd, com, 4, 0);
                sendall(sockfd, (void*)&namelen, sizeof(int), 0);
                sendall(sockfd, name, namelen, 0);
                int filesize = 0;
                recvall(sockfd, (void*)&filesize, sizeof(int), 0);
                if (filesize <= 0)
                {
                    printf("There is not this file: %s\n", name);
                    continue;
                }
                recvproc(sockfd, infile, filesize, 0);
                printf("loaded!\n");
                FILE* newFile = fopen(name, "wb");
                fwrite(infile, sizeof(char), filesize, newFile);
                fclose(newFile);
            }
            else if (strcmp(com, "all") == 0)
            {
                connect(sockfd, (struct sockaddr*)&thisAddr, sizeof(thisAddr));
                sendall(sockfd, com, 4, 0);
                int count;
                recvall(sockfd, (void*)&count, sizeof(int), 0);
                printf("server has %d files:\n", count);
                int i = 0;
                for (i = 0; i < count; ++i)
                {
                    int namelen;
                    char name[255];
                    recvall(sockfd, (void*)&namelen, sizeof(int), 0);
                    recvall(sockfd, name, namelen, 0);
                    name[namelen] = 0;
                    printf("%s\n", name);
                }
            }
            else
                printf("%s: command not found\n", com);
        }
    }

    close(sockfd);

    return 0;
}
