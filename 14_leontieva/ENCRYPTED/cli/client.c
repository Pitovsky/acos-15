#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define PRIVAT "./privat.key"

void chifer(char* tmp_path, char* pub_key_path){
    //Структура для хранения открытого ключа
    RSA *pubKey = NULL;
    unsigned char *ptext, *ctext;
    FILE *pub_key_file = NULL;

    //Открываем входной и создаём выходной файлы
    int inf = open(tmp_path, O_RDWR);
    int outf = open("./rsa.file", O_CREAT|O_TRUNC|O_RDWR, 0600);
    //Считываем открытый ключ
    pub_key_file = fopen(pub_key_path, "rb");
    pubKey = PEM_read_RSAPublicKey(pub_key_file, NULL, NULL, NULL);

    //Определяем длину ключа
    int key_size = RSA_size(pubKey);
    ptext = malloc(key_size);
    ctext = malloc(key_size);

    //Шифруем содержимое входного файла
    int intlen, outlen;
    while(1) {
        intlen = read(inf, ptext, key_size - 11);
        if(intlen <= 0)
            break;
        outlen = RSA_public_encrypt(intlen, ptext, ctext, pubKey, RSA_PKCS1_PADDING);
        if(outlen != RSA_size(pubKey))
            exit(-1);

        write(outf, ctext, outlen);
    }
    //return pubKey;
}
//Функция, которая по образу и подобию функции выше расшифровывает содержимое файла rsa_file_path ключом priv_key_path
void dechifer(char* rsa_file_path, char* priv_key_path){
    RSA *privKey = NULL;
    RSA *prpr = NULL;
    FILE *priv_key_file;
    unsigned char *ptext = NULL;
    unsigned char *ctext = NULL;

    //Открываем входной и создаём выходной файл
    int inf, outf;
    inf = open(rsa_file_path, O_RDWR);
    if(inf < 0) {
        perror("Problems with opening rsa file");
        exit(errno);
    }
    outf = open("./result.file", O_CREAT|O_TRUNC|O_RDWR, 0600);
    if(outf < 0) {
        perror("Problems with opening file");
        exit(errno);
    }

    //Открываем ключевой файл и считываем секретный ключ
    priv_key_file = fopen(priv_key_path, "rwb");
    if (priv_key_file== NULL) perror("fopen!");
    privKey = PEM_read_RSAPrivateKey(priv_key_file, &prpr, NULL, "hello");
    if (privKey == NULL) perror("Priv key is NULL");
    if (prpr == NULL) perror("prpr is NULL");

    //Определяем размер ключа
    size_t key_size;
    key_size = RSA_size(privKey);
    ctext = malloc(key_size);
    ptext = malloc(key_size);

    //Дешифруем файл
    int intlen, outlen;
    while(1) {
        intlen = read(inf, ctext, 2048);
        if(intlen <= 0) {
            break;
        }
        outlen = RSA_private_decrypt(intlen, ctext, ptext, privKey, RSA_PKCS1_PADDING);
        if(outlen < 0) {
            exit(0);
        }

        write(outf, ptext, outlen);
    }

    close(outf);
    close(inf);
}
void send_file(char* path, int sock){//works
    int fd = open(path, O_RDONLY);
    if(fd == -1) {
        perror("Problems with opening file");
        exit(errno);
    }
    struct stat info_file;
    if (fstat(fd, &info_file) == -1) {
        perror("fstat");
        exit(errno);
    }
    int info_length = info_file.st_size;//длина содержимого
    int name_length = fileNameLengthByPath(path);
    //получаем имя файла
    char name[name_length];
    memcpy(name, path + strlen(path) - name_length, name_length);
    //получаем содержимое файла
    char info[info_length];
    int res = read(fd, info, info_length);
    if(res < 0) {
        perror("Problems with reading from file");
        exit(errno);
    }
    res = send(sock, (char*)(&name_length), sizeof(int), 0);
    if(res == -1) {
        perror("Problems with sending the length of file-name");
        exit(errno);
    }
    res = send(sock, name, name_length, 0);
    if(res == -1) {
        perror("Problems with sending the name of file");
        exit(errno);
    }
    res = send(sock, (char*)(&info_length), sizeof(int), 0);
    if(res == -1) {
        perror("Problems with sending the length of information");
        exit(errno);
    }
    res = send(sock, info, info_length, 0);
    if(res == -1) {
        perror("Problems with sending the information");
        exit(errno);
    }
    close(fd);
}

int recv_file(int sock){//works
    char buf[1024];

    // считывание данных от клиента
    int readed = 0;
    int name_length, content_length;
    int filefd;

    // 1. получение длины имени файла
    readed = read(sock, buf, sizeof(int));
    if(readed != sizeof(int)) {
        perror("The first read is wrong");
        exit(1);
    }
    name_length = *((int *) buf);

    // 2. получаение имени файла
    readed = read(sock, buf, name_length);
    if(readed != name_length) {
        perror("The second read is wrong");
        exit(1);
    }

    buf[name_length] = '\0';
    filefd = open(buf, O_WRONLY | O_CREAT, 0775);
    if(filefd < 0) {
        perror("Can't open file");
        exit(1);
    }

    // 3. получение длины содержимого файла
    readed = read(sock, buf, sizeof(int));
    if(readed != sizeof(int)) {
        perror("The third read is wrong");
        exit(1);
    }
    content_length = *((int *) buf);

    // 4. получение содержимого файла
    readed = 0;
    int internal_readed = 0;
    do {
        internal_readed = read(sock, buf, 1023);
        write(filefd, buf, internal_readed);
        buf[internal_readed] = '\0';
        readed += internal_readed;
    } while(readed < content_length);

    close(filefd);
    return 0;
}

// возвращает длину файла по его пути (или тупо имени файла)
int fileNameLengthByPath(const char * filePath) {
    if(filePath == NULL)
        return 0;

    if(strrchr(filePath, '/') == NULL)
        return strlen(filePath);

    int lastSlashPosition = (int)(strrchr(filePath, '/') - filePath);
    return strlen(filePath) - lastSlashPosition - 1;
}


int main(int argc, char** argv) {
    if (argc != 4) {
        perror("Incorrect input data");
        exit(1);
    }

    /*Сгенерируем необходимые для работы ключи*/
    //указатель на структуру для хранения ключей
    RSA *rsa = NULL;
    unsigned long bits = 2048;
    FILE *priv_key_file = NULL, *pub_key_file = NULL;

    //контекст алгоритма шифрования
    const EVP_CIPHER *cipher = NULL;
    priv_key_file = fopen(PRIVAT, "wb");
    char PUBLIC[120];
    strcpy(PUBLIC, argv[3]);
    strcat(PUBLIC, ".key");
    pub_key_file = fopen(PUBLIC, "wb");

    //генерируем ключи
    rsa = RSA_generate_key(bits, RSA_F4, NULL, NULL);

    //формируем контекст алгоритма шифрования
    OpenSSL_add_all_ciphers();
    cipher = EVP_get_cipherbyname("bf-ofb");

    //получаем из структуры RSA открытый и секретный ключи и сохраняем в файлах
    //секретный ключ шифруем с помощью парольной фразы "hello"
    PEM_write_RSAPrivateKey(priv_key_file, rsa, cipher, NULL, 0, NULL, "hello");
    PEM_write_RSAPublicKey(pub_key_file, rsa);
		fclose(pub_key_file);
		fclose(priv_key_file);
    /*Выполнение подключений*/

	int sock;
	struct sockaddr_in addr;
	int bytes_read;
	char buf[1024];
	sock = socket( AF_INET, SOCK_STREAM, 0 );
	if(sock < 0) {
		perror("socket");
		exit(1);
	}

	addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi (argv[1]) );
    addr.sin_addr.s_addr = inet_addr(argv[2]);

	if( connect( sock, ( struct sockaddr* )&addr, sizeof(addr) ) < 0 ) {
        perror( "connect" );
        exit(2);
    }

	printf ("connected to server\n");
    char nickname[30];
    strcpy(nickname, argv[3]);
    memset(nickname + strlen(argv[3]), 0, 30 - strlen(argv[3]));//на всякий случай
    int flag = 0;
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
			if (strncmp(buf, "Your pubkey was requsted by ",28) == 0){
                if(recv_file(sock)){
                    perror("recv(rsa.file)");
                    exit(1);
                };
                dechifer("./rsa.file", PRIVAT);
                int resfd = open("./result.file", O_RDONLY, 00666);
                if (resfd < 0){
                    perror("open(result.file");
                    exit(1);
                }
                if(read(resfd, buf, 1024) < 0){
                    perror("read(resul.file)");
                    exit(1);
                }
                printf("%s", buf);
                close(resfd);
			}
		} else if(FD_ISSET(0, &foread)){
			if (fgets(buf, 1024, stdin) == NULL){
				perror("fgets");
				exit(4);
			}
			// Отправка сообщений
			if (buf[0] != '/'){
                if (!flag){//если оно первое - послать имя и ключик
                    send(sock,nickname, strlen(nickname), 0);
                    send_file(PUBLIC, sock);
                    flag = 1;
                }
                else send(sock, buf, strlen(buf) + 1, 0);
			} else if(strncmp(buf, "/to ", 4) == 0){
				if(strchr(buf + 4, ' ') != NULL) {
					send(sock,buf,strlen(buf) + 1, 0);
				} else {//значит там либо нет имени,либо только имя и больше ничего
					printf( "Incorrect input format.\n" );
				}
			} else if(strncmp(buf, "/encrypted ",11) == 0){
                char* pos;
                if((pos = strchr(buf + 11, ' ')) != NULL) {
                    //Запись шифруемого сообщения в файл
                    char msg[255];
                    strcpy(msg,argv[3]);
                    strcat(msg, " said: ");
                    strcat(msg, pos);
                    int msgfd = open("msg.txt", O_WRONLY | O_CREAT | O_TRUNC, 00666);
                    if (msgfd < 0) {
                        perror("open msg.txt");
                        exit(1);
                    }
                    if(write(msgfd, msg, strlen(msg)) < strlen(msg)){
                        perror("write msg to file");
                        exit(1);
                    }
                    send(sock, buf, strlen(buf) +1, 0);
                    recv_file(sock);
                    char name [40]; //имя публичного ключа собеседника
                    strncpy(name, buf+11, pos - buf-11);
                    strcat(name, ".key");
                    chifer("./msg.txt", name);
                    //printf("began to send rsa file\n");
                    send_file("rsa.file",sock);
                    //printf("success(rsa.file)\n");
				} else {//либо нет имени,либо только имя и больше ничего
					printf( "Incorrect input format.\n" );
				}
			}else {
				printf( "Incorrect command\n" );
			}
		}
	}
	close( sock );
	/*Освобождение памяти*/
    RSA_free(rsa);
	return 0;
}
