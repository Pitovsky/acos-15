#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char buf[1024], s_matrixfile[1024];
int n_columns, n_rows, fd;
double *matrix;

void swap(double* a, double* b)
{
	double t = *a;
	*a = *b;
	*b = t;
}

void parse_input_and_open_file(int argc, char **argv)
{
	if (argc < 3) {
		printf("Usage: ./main [file_name] [n_columns] ([n_rows])");
		exit(0);
	}
	if (sscanf(argv[1], "%s", s_matrixfile) != 1) {
		printf("file_name must be string");
		exit(0);
	}
	if (sscanf(argv[2], "%d", &n_columns) != 1 || n_columns < 0) {
		printf("n_columns must be a non-negative number");
		exit(0);
	}
	if (sscanf(argv[2], "%d", &n_rows) != 1 || n_rows < 0) {
		printf("n_rows must be a non-negative number");
		exit(0);
	}

	fd = open(s_matrixfile, O_RDWR | O_CREAT, 0666);
	if (fd < 0) {
		printf("Error opening file %s", s_matrixfile);
		exit(0);
	}

	struct stat file_info;
	lstat(s_matrixfile, &file_info);

	int file_size = file_info.st_size;
	if (argc == 4 && file_size < n_rows * n_columns) {
		file_size = sizeof(double) * n_rows * n_columns;
	} else {
		n_rows = file_size / (sizeof(double) * n_columns);
	}

	fprintf(stderr, "n_rows: %d, n_columns: %d\n", n_rows, n_columns);

	matrix = (double*)mmap(NULL, 
			 file_size,
			 PROT_READ | PROT_WRITE,
			 MAP_SHARED,
			 fd,
			 0);
}

void exit_handler()
{
	exit(0);
}

void getinfo_handler()
{
	printf("n_columns: %d, n_rows: %d\n", n_columns, n_rows);
}

void get_handler()
{
	int x, y;
	if (scanf("%d%d", &x, &y) != 2) {
		printf("x, y must be integers");
		return;
	}
	if (!(0 <= x && x <= n_rows)) {
		printf("x must be in [0, %d]\n", n_rows);
		return;
	}
	if (!(0 <= y && y <= n_columns)) {
		printf("y must be in [0, %d]\n", n_columns);
		return;
	}
	printf("matrix[%d][%d]: %lf\n", x, y, matrix[x * n_columns + y]);
}

void set_handler()
{
	int x, y;
	double value;
	if (scanf("%d%d%lf", &x, &y, &value) != 3) {
		printf("x, y, value must be integers");
		return;
	}
	if (!(0 <= x && x <= n_rows)) {
		printf("x must be in [0, %d]\n", n_rows);
		return;
	}
	if (!(0 <= y && y <= n_columns)) {
		printf("y must be in [0, %d]\n", n_columns);
		return;
	}
	matrix[x * n_columns + y] = value;
}

void sumrow_handler()
{
	int x, y;
	if (scanf("%d", &x) != 1) {
		printf("x must be integer");
		return;
	}
	double sum = 0;
	for (y = 0; y < n_columns; ++y)
		sum += matrix[x * n_columns + y];
	printf("sum of row %d: %lf\n", x, sum);
}

void sumcol_handler()
{
	int y, x;
	if (scanf("%d", &y) != 1) {
		printf("y must be integer");
		return;
	}
	double sum = 0;
	for (x = 0; x < n_columns; ++x)
		sum += matrix[x * n_columns + y];
	printf("sum of column %d: %lf\n", y, sum);
}

void swap_handler()
{
	int x, y, i;
	if (scanf("%d%d", &x, &y) != 2) {
		printf("x, y must be integers\n");
		return;
	}
	if (!(0 <= x && x <= n_columns)) {
		printf("x must be in [0, %d]\n", n_columns);
		return;
	}
	if (!(0 <= y && y <= n_columns)) {
		printf("y must be in [0, %d]\n", n_columns);
		return;
	}
	for (i = 0; i < n_columns; ++i) {
		swap(&matrix[x * n_columns + i], &matrix[y * n_columns + i]);
	}
}

int main(int argc, char **argv)
{
	parse_input_and_open_file(argc, argv);

	while (1) {
		scanf("%s", buf);
		if (strcmp(buf, "exit") == 0) {
			exit_handler();
		} else if (strcmp(buf, "getinfo") == 0) {
			printf("getinfo\n");
			getinfo_handler();
		} else if (strcmp(buf, "get") == 0) {
			printf("get\n");
			get_handler();
		} else if (strcmp(buf, "set") == 0) {
			printf("set\n");
			set_handler();
		} else if (strcmp(buf, "sum row") == 0) {
			printf("sum row\n");
			sumrow_handler();
		} else if (strcmp(buf, "sum col") == 0) {
			printf("sum cow\n");
			sumcol_handler();
		} else if (strcmp(buf, "swap") == 0) {
			printf("swap\n");
			swap_handler();
		}
	}

	return 0;
}
