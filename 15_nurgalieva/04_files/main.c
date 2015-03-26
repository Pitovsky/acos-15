#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <time.h>

int count_Word(FILE* input)
{
	int c, count;
	count = 0;
	
	  while  ( (c = getc(input)) != EOF)
	  {
		   while  ( (c = getc(input)) != EOF)
			{
			if (((c >= 'a')&&(c <= 'z' ))||(( c >= 'A')&&(c <= 'Z'))||(c == '_')||((c >= '0')&&(c <= '9')))
				{
					continue;
				}
			else break;
			}
			if (c != EOF) 
			{
				count++;
			}
			
			while  ( (c = getc(input)) != EOF)
			{
			if (((c < 'a')||(c > 'z' ))&&(( c < 'A')||(c > 'Z'))&&(c != '_')&&((c < '0')||(c > '9')))
				{
					continue;
				}
			else break;
			}
     }
	
	return count;
}

void Start(char* path, int NowDeep, int Lim_deep, int flag)
{
	//printf("start started");
	if (NowDeep == Lim_deep + 1)
	{
		return;
	}

	DIR* d;
	int result;
	char str[256];
	
	d = opendir(path); //возвращает указатель на dir
	struct dirent* point; //хранит имя и номер узла
	
	while ( (point = readdir(d)) != NULL) //возвращает указатель на объект типа dirent
	{
		if (strcmp(point->d_name, ".") == 0)
		{
			point = readdir(d);
			continue;
		}
		if (strcmp(point->d_name, "..") == 0)
		{
			point = readdir(d);
			continue;
		}	
		
		
		struct stat status; //там есть st_mode, он знает, директория это, файл или ссылка (есть флаги)

		

		strcpy(str, path);
		strcat(str, "/");
		strcat(str, point->d_name);
		
		//printf("element -->> %s\n", str);
		
		if ((result = lstat(str, &status)) == 0)//статистика получена
		{
			if (S_ISDIR(status.st_mode))//если директория
			{
				Start(str, NowDeep + 1, Lim_deep, flag);
			}
			if (S_ISREG(status.st_mode))
			{
				FILE* f = fopen(str, "r");
				printf("Name: %s  count: %d \n", point->d_name, count_Word(f));

			}
			if (S_ISLNK(status.st_mode) && flag)
			{
				char buf [256];
				int j;
				
				j = readlink(str, buf, sizeof(buf) - 1);
				buf[j] = '\0';
				
				FILE* f = fopen(buf, "r");
				printf( "Name: %s  count: %d \t", point->d_name, count_Word(f));
				fclose(f);
			}
			
		}
	}
}

int main(int argc, char * argv[])
{
	int deep, flag, i;
	deep = 0;
	flag = 0;
	
	if (argc < 2)
	{
		printf("Not enough arguments\n");
		return 0;
	}
	
	for (i = 2; i < argc; ++i)
	{
		if (strcmp(argv[i],"-r") == 0)
		{
			deep = atoi(argv[i + 1]);
		}
		if (strcmp(argv[i], "-s") == 0)
		{
			flag++;
		}
		
	}
	
	Start(argv[1], 1, deep, flag);	
	
	return 0;
}


