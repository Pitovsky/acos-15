#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>

#define MAX_PATH 256

int CharInWord(char t)
{
	if (t == '_') return 1;
	if (t >= 'a' && t <='z') return 1;
	if (t >='A' && t <= 'Z') return 1;
	return 0;
}

int words(FILE* in)
{
	int count = 0, PrevInWord = 0;
	char t;
	while (!feof(in))
	{
		t = getc(in);
		if(CharInWord(t))
		{
			PrevInWord = 1;
			continue;
		}
		if (PrevInWord)			//Если символ разделитель и перед ним стоит слово
		{
			count++;			//Счетчик слов
			PrevInWord = 0;		//перед следующим слово не стоит
		}
	}
	return count;
}

void run(char* dir, int depth, int limit, int CheckLinks)
{
	printf("RUN DEPTH=%d STARTED OK\n", depth);
	printf("dir : %s\n", dir);
	if (limit != 0)
	{
		if (depth > limit)
		{
			printf("Depth == %d > Limit == %d\n", depth, limit);
			return;
		}
	}
	
	DIR *CurrDir = opendir(dir);
	struct dirent *element;	
	element = readdir(CurrDir); 
	
	while(element != NULL)	//Если объект есть, то есть считался
	{           
//Если имя . или .., то пропустить .d_name      
		if (strcmp(element->d_name, ".") == 0)
		{
			element = readdir(CurrDir);
			continue;
		}
		if (strcmp(element->d_name, "..") == 0)
		{
			element = readdir(CurrDir);
			continue;
		}		
		
		printf("Element name: %s\n", element->d_name);
		
	
//Склеивание полного пути к файлу
		char filepath[MAX_PATH];
		strcpy(filepath, dir);
		strcat(filepath, "/");
		strcat(filepath, element->d_name);
//Склеивание
		
//Информацию об element dirent запишем в fileinfo
		struct stat fileinfo;
		int check = lstat(filepath, &fileinfo);
			
		if (check != -1)
		{
			if(S_ISREG(fileinfo.st_mode)) //Если обычный файл
			{
				//Посчитать слова
				FILE *in = fopen(filepath, "r"); 
				printf("WORDS in motherfuckingfile %s\n------------>%d.\n", filepath, words(in));
				fclose(in);
			}
			if(S_ISDIR(fileinfo.st_mode)) //Если директория
			{
				run(filepath, depth+1, limit, CheckLinks); //Рекурсивно опускаемся ниже
				printf("dir %s checked.\n", dir);
			}
			if(S_ISLNK(fileinfo.st_mode) && CheckLinks) //Если символьная ссылка
			{
				int n = readlink(filepath, filepath, sizeof(filepath) - 1);
				filepath[n] = '\0';
				FILE* in = fopen(filepath, "r");
				printf("WORDS in motherfuckingfile %s--->%d.\n", filepath, words(in));
				fclose(in);
			}
		}
		element = readdir(CurrDir);
	}
}

int main(int argc, char**argv)
{
	if (argc == 1) printf("argc == 1");
	if (argc < 2)
	{
		printf("Wrong input.\n");
		return 1;
	}
	int i = 3, CheckLinks = 1, depth;
	
	while(i < argc)
	{
		if (!strcmp(argv[i], "-r") && i+1 < argc)
		{
			if (i+1 >= argc) 
			{
				printf("Wrong input.\n");
				return 1;
			}
			depth = atoi(argv[i+1]);
		}
	
		if (!strcmp(argv[i], "-s")) CheckLinks = 0;
		i++;
	}

	run(argv[1], 1, depth, CheckLinks);

return 0;
}
