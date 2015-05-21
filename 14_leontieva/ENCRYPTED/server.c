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
void sendtoall( TList* list, char* message ) {
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

void sendfile(int readsock, int sendsock){
    char buf[1024];
    int readed = 0;
    int name_length, content_length;

    // 1. получение длины имени файла
    readed = read(readsock, buf, sizeof(int));
    if(readed != sizeof(int)) {
        perror("The read of namelenght is wrong");
        exit(1);
    }
    send(sendsock,buf, sizeof(int) +1, 0);
    name_length = *((int *) buf);

    // 2. получаение имени файла
    readed = read(readsock, buf, name_length);
    if(readed != name_length) {
        perror("The read of filename is wrong");
        exit(1);
    }
    buf[name_length] = '\0';
    send(sendsock,buf, sizeof(int) +1, 0);

    // 3. получение длины содержимого файла
    readed = read(readsock, buf, sizeof(int));
    if(readed != sizeof(int)) {
        perror("The read of filelength is wrong");
        exit(1);
    }
    content_length = *((int *) buf);
    send(sendsock, buf, sizeof(int) + 1, 0);

    // 4. получение содержимого файла
    readed = 0;
    int internal_readed = 0;
    do {
        internal_readed = read(readsock, buf, 1023);
        send(sendsock, buf, 1023, 0);
        buf[internal_readed] = '\0';
        readed += internal_readed;
    } while(readed < content_length);
}

int main(int argc, char** argv) {
    if (argc != 3){
        perror("Incorrect input data.");
        exit(1);
    }

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
    addr.sin_port = htons(atoi(argv[1]));
    addr.sin_addr.s_addr = inet_addr(argv[2]);

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
			printf ("%s joined\n", tmp1->nickname);
			char msg[265];
			strcpy(msg, tmp1->nickname);
			strcat(msg," joined\n");
			sendtoall(&clients, msg);
		}

		tmp = clients.begin;
		while (tmp != NULL){
			if (FD_ISSET(tmp->sock, &foread)){
				bytes_read = recv(tmp->sock, buf, 1024, 0);
				if (bytes_read <= 0){//client left
					client* tmp1 = tmp->next;
					char msg[40];
					strcpy(msg, tmp->nickname);
					strcat(msg, " left");
					printf("%s\n", msg);
					sendtoall(&clients, msg);
					close(tmp->sock);
					list_delete(&clients, tmp);
					tmp = tmp1;
					continue;
				}
				if (buf[0] != '/'){//not a command
                    printf("%s\n", buf);
					chat(&clients, buf, tmp);
				} else if( strncmp( buf, "/to ", 4 ) == 0 ) {
					char* msg_recv = strchr(buf + 4, ' ');
					*msg_recv = '\0';//там где заканчивается никнейм
					msg_recv++;
					client* tmp1 = clients.begin;
					int flag = 0;

					while (tmp1 != NULL) {//поиск адресата в списке
						if (strcmp(buf + 4, tmp1->nickname) == 0){//отправка
							char msg[1060];
							strcpy(msg, tmp->nickname);
							strcat(msg, " (to me): ");
							strcat(msg, msg_recv);
							send(tmp1->sock, msg, strlen(msg)+1, 0);
							flag = 1;
							break;
						}
						tmp1 = tmp1->next;
					}
					if (flag == 0) {
						send(tmp->sock, "Name wasn't found.\n", 20, 0);
					}
				} else if (strncmp( buf, "/encrypted ", 11 ) == 0 ) {
					char* msg_recv = strchr(buf + 11, ' ');
					*msg_recv = '\0';//там где заканчивается никнейм
                    msg_recv++;
					client* tmp1 = clients.begin;
					int flag = 0;

					while (tmp1 != NULL) {//поиск адресата в списке
						if (strcmp(buf + 11, tmp1->nickname) == 0){//отправкa
							char rsp[120];
							strcpy(rsp, tmp->nickname);
							strcat(rsp, " requested you public key.\n");
							send(tmp1->sock, rsp,strlen(rsp) + 1, 0);
							flag = 1;
							sendfile(tmp->sock, tmp1->sock);
							break;
						}
						tmp1 = tmp1->next;
					}
					if (flag == 0) {
						send(tmp->sock, "Name wasn't found.\n", 36, 0);
					}
                }
			}
			tmp = tmp->next;
		}
	}
	return 0;
}
