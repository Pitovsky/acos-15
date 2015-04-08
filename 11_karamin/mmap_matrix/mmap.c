#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

#define MAX_COMMAND_SIZE 30

int main(int argc, char** argv)
{
//Элементарная проверка на корректность входных данных.
	if (argc < 3)
	{
		printf("Wrong input\n");
		return 1;
	}

//Открываем файл для чтения/записи с матрицей в бинарном виде
	int infd;
	if ((infd = open(argv[1], O_RDWR)) < 0)
	{
		printf("Can't open %s for read/write\n", argv[1]);
		return 1;
	}

//Сбор сведений о файле с матрицей в бинарном виде
	struct stat fileinfo;
	if (stat(argv[1], &fileinfo) < 0)
	{
		printf("Stat error.\n");
		return 1;
	}
	
//Здесь вычисляется количество байт в файле,
//которые должны быть отображены в адресное пространство программы.
	int newfilesize;
	int stlb = atoi(argv[2]), strk;
	if (argc >= 4)
		strk = atoi(argv[3]);
	else
		strk = fileinfo.st_size / (sizeof(double)*stlb);
	newfilesize = strk*stlb*sizeof(double);
	
//Здесь проверка на то, как размер входного файла соответствует запрошенным
//пользователем данным. Если пользователь запросил больше, чем записана матрица
//в файле, то файл дополнить необходимым количеством байт и заполнить нулями.
//newfilesize - это как раз количество байт, которые будут отображены в
//адресное пространство программы.
	double *matrix;
	if (newfilesize <= fileinfo.st_size)	//Если в файле достаточно данных, запрошенных пользователем.
	{
		if((matrix = mmap(0, newfilesize, PROT_READ | PROT_WRITE, MAP_SHARED, infd, 0)) == MAP_FAILED)
		{
			printf("mmap failed.\n");
			return 1;
		}
	}
	else	//Если в файле недостаточно данных, до приписать в конец сколько нужно и отобразить
	{
		FILE *temp; int i, firstnewpos = fileinfo.st_size;
		temp = fopen(argv[1], "a");
//Добавляем необходимое количество байт в конец файла.		
		for (i = 0; i < newfilesize - fileinfo.st_size; ++i)
			fprintf(temp, "%c", '0');
		fclose(temp);
//Опять собираем информацию о файле
		stat(argv[1], &fileinfo);
//И только теперь отображаем этот файл в память (matrix.txt)
		if((matrix = mmap(0, newfilesize, PROT_READ | PROT_WRITE, MAP_SHARED, infd, 0)) == MAP_FAILED)
		{
			printf("mmap failed.\n");
			return 1;
		}
//Инициализация по умолчанию нулями дополненного пространства файла
		for (i = firstnewpos; i < strk*stlb; ++i)
			matrix[i] = 0;
	}
		
//Отсюда начинается реализация самих команд	
	char *command = (char*)malloc(MAX_COMMAND_SIZE*sizeof(char));
	scanf("%s", command);
	while (strcmp(command, "exit"))
	{
		if (!strcmp(command, "getinfo"))
		{
			printf("Number of rows = %d\n", strk);
			printf("Number of columns = %d\n", stlb);
		}
		if (!strcmp(command, "get"))
		{
			int x, y;
			scanf("%d%d", &x, &y);
			if (x < strk && y < stlb)
				printf("Matrix[%d][%d] = %lf\n", x, y, matrix[x*stlb + y]);
		}
		if (!strcmp(command, "set"))
		{
			int x, y; double New;
			scanf("%d%d%lf", &x, &y, &New);
			if (x < strk && y < stlb)
				matrix[x*stlb + y] = New;
		}
		if (!strcmp(command, "sum"))
		{
			char *sumcommand = (char*)malloc(MAX_COMMAND_SIZE*sizeof(char));
			int coord;
			scanf("%s%d", sumcommand, &coord);
			if (!strcmp(sumcommand, "row") && coord < strk)
			{
				int i; double sum = 0;
				for (i = 0; i < stlb; ++i)
					sum += matrix[coord*stlb + i];
				printf("Sum row %d = %lf\n", coord, sum);
			}
			if (!strcmp(sumcommand, "col") && coord < stlb)
			{
				int i; double sum = 0;
				for (i = 0; i < strk; ++i)
					sum += matrix[i*stlb + coord];
				printf("Sum col %d = %lf\n", coord, sum);
			}
			free(sumcommand);
		}
		if (!strcmp(command, "swap"))
		{
			int x, y, i;
			scanf("%d%d", &x, &y);
			if (x < strk && y < strk && x != y)
			{
				double temp;
				for (i = 0; i < stlb; ++i)
				{
					temp = matrix[x*stlb + i];
					matrix[x*stlb + i] = matrix[y*stlb + i];
					matrix[y*stlb + i] = temp;
				}
			}
		}
		if (!strcmp(command, "printmatrix"))
		{
			int i, j;
			for (i = 0; i < strk; ++i)
			{
				for (j = 0; j < stlb; ++j)
				{
					printf("%lf ", matrix[i*stlb + j]);
				}
				printf("\n");
			}
		}
		scanf("%s", command);
	}
	
/*
● exit

● getinfo ­­­ вывести число строк и столбцов

● get x y  ­­­ узнать значение в соотв. ячейке

● set x y  ­­­ записать значение в соотв. ячейке

● sum row x ­­­ вывести сумму по соотв. строке

● sum col y  ­­­ вывести сумму по соотв. столбцу

● swap x y ­­­ переставить соотв. строки
*/
	return 0;
}
