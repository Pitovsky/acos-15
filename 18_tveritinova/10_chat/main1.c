#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <unistd.h>

int send_message(char* buf, int socket, int len)
{
    int current = 0;
    while (current < len)
    {
        int code = send(socket, buf + current, len - current, 0);
        if (code == -1)
        {
            return -1;
        }
        current += code;
    }
}

void process_with_server(int port)
{
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener == -1)
    {
        perror("socket");
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int er_bind = bind(listener, &addr, sizeof(addr));
    if (er_bind == -1)
    {
        perror("bind");
    }
    listen(listener, 2);
    int con[500];
    char buf[1024];
    while(1)
    {
        fd_set read_set;
        FD_ZERO(&read_set);
        FD_SET(listener, &read_set);
        int max = listener;
        for (int i = 0; i < 500; i++)
        {
            if (con[i] != 0)
            {
                FD_SET(con[i], &read_set);
                if (con[i] > max)
                {
                    max = con[i];
                }
            }
        }
        int er_select = select(max + 1, &read_set, NULL, NULL, NULL);
        if (er_select == -1)
        {
            perror("select");
        }
        if (FD_ISSET(listener, &read_set))
        {
            int connector = accept(listener, NULL, NULL);
            if (connector == -1)
            {
                perror("accept");
            }
            int find_key = 0;
            for (int i = 0; i < 500; i++)
            {
                if (con[i] == 0)
                {
                    con[i] = connector;
                    find_key = 1;
                    break;
                }
            }
            if (find_key == 0)
            {
                fprintf(stderr, "too many clients\n");
            }
        }
        for (int i = 0; i < 500; i++)
        {
            if ((con[i] != 0) && (FD_ISSET(con[i], &read_set)))
            {
                int cnt = recv(con[i], buf, 1024, 0);
                if (cnt <= 0)
                {
                    printf("disconnected\n");
                    close(con[i]);
                    con[i] = 0;
                    continue;
                }
                printf("server got: %s\n", buf);
                for (int j = 0; j < 500; j++)
                {
                    if ((con[j] != 0) && (j != i))
                    {
                        send_message(buf, con[j], strlen(buf) + 1);
                        printf("resended to %d\n", i);
                    }
                }
            }
        }
    }
    close(listener);
}

void process_with_client(int port, char* ip)
{
    printf("Welcome!\n");  
    int write = socket(AF_INET, SOCK_STREAM, 0);
    if(write < 0)
    {
        perror("socket");                                
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    if(connect(write, &addr, sizeof(addr)) < 0)
    {
        perror("Connect");
    }
    int read_code = 0;
    char buf[1024];
    char* line = (char*) malloc(1024);

    while (1)
    {
        printf("Enter message:");
        fd_set read_set;
        FD_ZERO(&read_set);
        FD_SET(0, &read_set);
        FD_SET(write, &read_set);
        int code = select(write + 1, &read_set, NULL, NULL, NULL);
        if (code == -1)
        {
            perror("Select error.");
        }
        if (FD_ISSET(0, &read_set))
        {
            int len;
            read_code = getline(&line, &len, stdin);
            if (read_code <= 0)
            {
                break;
            }
            line[read_code - 1] = '\0';
            if (strlen(line) != 0)
            {
                send_message(line, write, strlen(line) + 1);
            }
        }
        if (FD_ISSET(write, &read_set))
        {
            code = recv(write, buf, 1024, 0);
            if (code <= 0)
            {
                free(line);
            }
            printf("\rServer told: %s\n", buf);
        }
    }

    free(line);
    close(write);
    printf("Goodbye!\n");
}

int main(int argc, char** argv)
{
    int mode = atoi(argv[1]); //0 - сервер, 1 - клиент
    int port;
    sscanf(argv[2], "%d", &port);
    if (mode == 0)
    {
        process_with_server(port);
    }
    if (mode == 1)
    {
        char* ip = argv[3];
        process_with_client(port, ip);
    }
    if ((mode != 0) && (mode != 1))
    {
        printf("incorrect mode\n");
    }
    return 0;
}
