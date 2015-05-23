#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>
#include <sys/select.h>

// двусвязный список клиентов
typedef struct client {
	struct client* prev;
	int sock;
	char nickname[30];
	struct client* next;
} client;

typedef struct TList {
	client* begin;
	client* end;
	int size;
} TList;

void list_push_back( TList* list, client* new_client ) {
	if( list->size != 0 ) {
		list->end->next = new_client;
		new_client->prev = list -> end;
		list->end = new_client;
		new_client->next = NULL;
	} else {
		list->begin = new_client;
		list->end = new_client;
		new_client->next = NULL;
		new_client->prev = NULL;
	}
	(list->size)++;
}

void list_delete( TList* list, client* id) {
	close(id->sock);
	client* left;
	client* right;
	left = id->prev;
	right = id->next;
	free(id);
	if( left != NULL && right != NULL ) {
		left->next = right;
		right->prev = left;
	} else if (left != NULL) {
		list->end = left;
		left->next = NULL;
	} else if (right != NULL) {
		right->prev = NULL;
		list->begin = right;
	} else {
		list->begin = NULL;
		list->end = NULL;
	}
	(list->size)--;
}

void list_erase( TList* list ) {
	while( list -> size > 0 ) {
		list_delete( list, list->begin );
	}
}

//сообщение от сервера всем
void sendtoall( TList* list, char* message) {
	client* temp = list -> begin;
	while (temp != NULL ) {
        send( temp->sock, message, strlen(message) + 1, 0 );
		temp = temp->next;
	}
}
//сообщение от клиента всем
void chat(TList* list, char* message, client* id){
    char buf[1280];
	strcpy(buf, id->nickname);
	strcat(buf, ": ");
	strcat(buf, message);
	sendtoall(list, buf);
}

int main(int argc, char** argv) {
if (argc < 3) {
    perror("few arguments");
    exit(1);
}
if (strcmp(argv[1], "server") == 0){
	int listener;
	struct sockaddr_in addr;
	int bytes_read;
	char buf[1024];

	listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener < 0){
		perror("socket");
		exit(1);
	}
	addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listener,(struct sockaddr*)&addr, sizeof(addr)) < 0){
        perror("bind");
        exit(1);
    }
    printf ("Server %s is ready\n", (char*)inet_ntoa(addr.sin_addr.s_addr));
    listen(listener, 3);

    TList clients;
    clients.size = 0;
    clients.begin = NULL;
    clients.end = NULL;

	while(1) {
		fd_set foread;
		FD_ZERO(&foread);
		FD_SET(listener, &foread);
		client* tmp = clients.begin;
		int max = listener;
		while(tmp != NULL) {
			FD_SET(tmp->sock, &foread);
			if (max < tmp->sock) max = tmp->sock;
			tmp = tmp->next;
		}
		if (select(max + 1, &foread, NULL, NULL, NULL) < 0 ){
			perror("select");
			exit(1);
		}
		if (FD_ISSET(listener, &foread)){
			int sock = accept(listener, NULL, NULL);
			if (sock < 0) {
				perror("accept");
				exit(1);
			}
			client* tmp1 = (client*)malloc(sizeof(client));
			tmp1->sock = sock;

			char name[30];
			memset(&name, 0,30);
			bytes_read = recv(tmp1->sock, name,30, 0);
			name[strlen(name)] = 0;
			memset(&tmp1->nickname, 0, 30);
			strcpy(tmp1->nickname, name);
			list_push_back(&clients, tmp1);
			char msg[265];
			strcpy(msg, tmp1->nickname);
			strcat(msg," joined\n");
			sendtoall(&clients, msg);
			printf ("%s", msg);
		}

		tmp = clients.begin;
		while (tmp != NULL){
			if (FD_ISSET(tmp->sock, &foread)){
				bytes_read = recv(tmp->sock, buf, 1024, 0);
				if (bytes_read <= 0){//client left
					client* tmp1 = tmp->next;
					char msg[40];
					strcpy(msg, tmp->nickname);
					strcat(msg, " left\n");
					printf("%s", msg);
					sendtoall(&clients, msg);
					close(tmp->sock);
					list_delete(&clients, tmp);
					tmp = tmp1;
					continue;
				}
				printf("%s\n", buf);
                chat(&clients, buf, tmp);
			}
			tmp = tmp->next;
		}
	}
}
if (strcmp(argv[1],"client") == 0){
int sock;
	struct sockaddr_in addr;
	int bytes_read;
	char buf[1024];
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0) {
		perror("socket");
		exit(1);
	}

	addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi (argv[2]) );
    addr.sin_addr.s_addr = inet_addr(argv[3]);

	if(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0 ) {
        perror( "connect" );
        exit(2);
    }

	printf ("Connected to server, please click Enter\n");
    char nickname[30];
    strcpy(nickname, argv[4]);
    memset(nickname + strlen(argv[4]), 0, 30 - strlen(argv[3]));//на всякий случай
    int flag = 0;//0 - сообщение первое
	while(1) {
		fd_set foread;
		FD_ZERO(&foread);
		FD_SET(sock, &foread);
		FD_SET(0, &foread);// 0 - stdin

		if (select(sock + 1, &foread, NULL, NULL, NULL) < 0){
			perror("select");
			exit(3);
		}

		if (FD_ISSET(sock, &foread)) {
			bytes_read = recv(sock, buf, 1024, 0);
			if(bytes_read <= 0){
				close(sock);
				printf("Server closed.\n");
				return 0;
			}
			printf("%s", buf);
		} else if(FD_ISSET(0, &foread)){
			if (fgets(buf, 1024, stdin) == NULL){
				perror("fgets");
				exit(4);
			}
			// Отправка сообщений
            if (!flag){//если оно первое - послать имя
                send(sock,nickname, strlen(nickname), 0);
                flag = 1;
            }
            else send(sock, buf, strlen(buf) + 1, 0);
		}
	}
	close(sock);
}
return 0;
}
