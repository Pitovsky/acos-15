/*При линковке использовать флаг -lcrypto*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "openssl/rsa.h"
#include "openssl/pem.h"

//Имена ключевых файлов
#define PRIVAT "./privat.key"
#define PUBLIC "./public.key"

int main(int argc, char **argv)
{
    OpenSSL_add_all_algorithms();

    RSA *privKey = NULL;
    RSA *prpr = NULL;
    FILE *priv_key_file;
    unsigned char *ptext = NULL;
    unsigned char *ctext = NULL;

    //Открываем входной и создаём выходной файл
    int inf, outf;
    inf = open("./rsa.file", O_RDWR);
    if(inf < 0) {
        perror("Problems with opening file");
        exit(errno);
    }
    outf = open("./test.file", O_CREAT|O_TRUNC|O_RDWR, 0600);
    if(outf < 0) {
        perror("Problems with opening file");
        exit(errno);
    }

    //Открываем ключевой файл и считываем секретный ключ
    priv_key_file = fopen(PRIVAT, "rwb");
    if (priv_key_file== NULL) perror("fopen!");
    privKey = PEM_read_RSAPrivateKey(priv_key_file, &prpr, NULL, "hello");
    if (privKey == NULL) perror("xxxxxx!");
    if (prpr == NULL) perror("xxxxxx2!");

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

    return 0;
}
