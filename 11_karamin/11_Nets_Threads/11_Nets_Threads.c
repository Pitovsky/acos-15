#include <sys/socket.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <stdlib.h>
#include <assert.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define THREADS_NUMBER 2
#define MAX_MESSAGE_SIZE MAX_NAME_LENGTH + MAX_NAME_LENGTH
#define MAX_NAME_LENGTH 1024
#define MAX_CONTENT_LENGTH 1024*29

typedef int
#define true 1
#define false 0
bool;

#define PORT 4444

#define PERM_FILE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int server_task(int sock);



typedef struct data
{
	char name[MAX_NAME_LENGTH];
	char content[MAX_CONTENT_LENGTH];
} data;
/*MUTEX!*/
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
void* thread_processing(void *our_pipe)
{	
//Здесь описано ожидание потоком момента, пока в пайп
//не будут записаны данные, и соответственно чтение этих данных
	int* temp = (int*)our_pipe;
	int read_fd = temp[0];

	while(true)
	{
		int client_socket;
		int nbytes;

		pthread_mutex_lock(&mtx);								
		while (sizeof(int) != (nbytes = read(read_fd, &client_socket, sizeof(int))))
			if (nbytes == -1)
			{
				printf("Ошибка чтения в потоке : %d\n", errno);
				exit(EXIT_FAILURE);
			}				
		pthread_mutex_unlock(&mtx);
		server_task(client_socket);
	}

	return our_pipe;
}

int* run_threads(int threads_number)
{
	assert(threads_number > 0);
	 
	int i; int* our_pipe = (int*)malloc(2*sizeof(int));
	pipe(our_pipe);
	
	pthread_t thread_id;
		
	for (i = 0; i < threads_number; ++i)
	{
		pthread_create(&thread_id, NULL, thread_processing, (void*)our_pipe);
		printf("Поток id%ld запущен...\n", thread_id);
		fflush(stdin);
	}
		
	return our_pipe;
 }
int server_task(int sock)
{
	bool finish = false;
	while (!finish)
	{
		data message;
		int nbytes;
		if ((nbytes = read(sock, &message, sizeof(data))) < 0)
		{
			printf("Ошибка получения сообщения %d...\n", errno);
			fflush(stdin);
			break;
		}
		if (nbytes == 0)
		{
			printf("Клиент отключился???...\n");
			break;
		}
		if (!strcmp("stop", message.name) || !strcmp("stop", message.content))
		{
			finish = true;
			break;
		}
		if (!strcmp(message.name, "") || !strcmp(message.content, ""))
			continue;
		printf("Сообщение с именем %s подобрано, и будет записано в файл...\n", message.name);
		message.name[nbytes] = '\0';
		int file_fd = open(message.name, O_RDWR | O_CREAT | O_TRUNC, PERM_FILE);
		write(file_fd, message.content, strlen(message.content));
		close(file_fd);
	}
	return 0;
}
int run_server(struct sockaddr_in sa)
{
	int server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);				//Creating socket
	bind(server_socket, (struct sockaddr *)&sa, sizeof(struct sockaddr));		//Binding - assotiation server_socket_fd with sa?
	listen(server_socket, 10);												//listening with maximum 10 requests in queue
	printf("Сервер запущен...\n");
	fflush(stdin);

	return server_socket;
}
int server_procesing(int sock)
{
	int* our_pipe = run_threads(THREADS_NUMBER);
	while (0 == 0)
	{
		int client = accept(sock, NULL, 0);
		assert(client > 0);
		printf("Client (sock_fd = %d) connected...\n", client);
		write(our_pipe[1], &client, sizeof(int));
		//сунули клиента в очередь, подбирать будут потоки
	}
	return 0;
}


int run_client(struct sockaddr_in sa)
{
	int client_socket = socket(AF_INET, SOCK_STREAM, 0);
/*	int flags = fcntl(client_socket, F_GETFL);
	fcntl(client_socket, F_SETFL, flags | O_NONBLOCK);
*/	
	while(connect(client_socket, (struct sockaddr *)&sa, sizeof(sa)) == -1)
	if (errno == EINPROGRESS)
	{
		printf("Ожидание ответа сервера\n");
		fflush(stdin);
		sleep(1);
		continue;
	}
	else if (errno == EALREADY)
	{
		sleep(1);
		printf(".");
		fflush(stdin);
	}
	else
	{
		printf("Ошибка подключения %d. Повтор через 2 секунды...\n", errno);
		sleep(2);
	}
	
	
	return client_socket;
}
int client_task(int sock)
{
		pthread_mutex_lock(&mtx);	
		data message;
		printf("FileName -->");
		scanf("%s", message.name);
		if (!strcmp("stop", message.name))
			return -1;
		printf("FileContent -->");
		scanf("%s", message.content);
		if (!strcmp("stop", message.content))
			return -1;
		
		send(sock, &message, sizeof(message), 0);
		printf("Данные отправлены...\n");
		fflush(stdin);
		pthread_mutex_unlock(&mtx);
	return 0;
}
int client_processing(int sock)
{

	while (0 == 0)
		if(client_task(sock))
		{
			printf("Клиент завершил свою работу...\n");
			exit(EXIT_SUCCESS);
		}
	fflush(stdin);
	return 0;
}

int main(int argc, char** argv)
{
	if (argc < 2) 
		exit(EXIT_FAILURE);
	
	struct sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(PORT);
	
	if (!strcmp("server", argv[1]))
	{
		sa.sin_addr.s_addr = htonl(INADDR_ANY);
		int sock = run_server(sa);
		server_procesing(sock);
	}
	else if(!strcmp("client", argv[1]))
	{
		if (argc != 3) 
			exit(EXIT_FAILURE);
		sa.sin_addr.s_addr = inet_addr(argv[2]);
		int sock = run_client(sa);
		client_processing(sock);
	}
	else
	{
		printf("Ошибка входного параметра: client/server.\n");
		fflush(stdin);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_FAILURE);
	return 0;
}
