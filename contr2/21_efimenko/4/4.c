#include<stdio.h>
#define MAX_PATH_SIZE 256
//понятно, что засчитывать тут нечего.
int count_Word(FILE* input)
{
	int c, count;
	count = 0;

	while ( (c = getc(input)) != EOF){
		while ( (c = getc(input)) != EOF){
			if (((c >= 'a')&&(c <= 'z' ))||(( c >= 'A')&&(c <= 'Z'))||(c == '_')||((c >= '0')&&(c <= '9'))){
				continue;
			}
			else break;
		}
		if (c != EOF){
			count++;
		}

		while ( (c = getc(input)) != EOF){
			if (((c < 'a')||(c > 'z' ))&&(( c < 'A')||(c > 'Z'))&&(c != '_')&&((c < '0')||(c > '9'))){
				continue;
			}else break;
		}
	}	

	return count;
}


void thread_processing


int main(){
	char* path = (char*)malloc(MAX_PATH_SIZE * sizeof(char));
	int th_num;
	scanf("%s %d", path, th_num);
	int i;
	pthread_t thread_id; 
	for (i = 0; i < th_num; ++i)
	pthread_create(&thread_id, NULL, thread_processing, (void*)our_pipe);


	return 0;
}
