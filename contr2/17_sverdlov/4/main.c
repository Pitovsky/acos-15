#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//похоже, вы файлы перепутали
int file_size;
char *file_name;
char *file;

int n_threads;

int *pos;
int *starts;
int *words;
int *lines;
pthread_t *threads;

int is_word(char c)
{
	return isdigit(c) || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

void *process(void* idp)
{
	int id = *((int*)idp);

	int ended = 1;
	char c;
	while (pos[id] < file_size && 
	       ((id == n_threads - 1) || (id != n_threads - 1 && pos[id] != starts[id + 1])) && 
	       (c = file[pos[id]])) 
	{
		printf("LETTER, %c\n", c);
		printf("id: %d, pos: %d\n", id, pos[id]);
		if (is_word(c)) {
			if (ended) {
				++words[id];
				printf("DELETE, id: %d\n", id);
			}
			ended = 0;
		} else {
			ended = 1;
		}

		if (c == '\n')
			++lines[id];
		++pos[id];
	}
	pthread_exit(NULL);
}

int *args_;

int main(int argc, char **argv)
{
	if (argc < 3) {
		printf("Too few arguments\n");
		return 0;
	}

	file_name = argv[1];
	sscanf(argv[2], "%d", &n_threads);


	pos = (int*) malloc(sizeof(int) * n_threads);
	lines = (int*) malloc(sizeof(int) * n_threads);
	starts = (int*) malloc(sizeof(int) * n_threads);
	threads = (pthread_t*) malloc (sizeof(pthread_t) * n_threads);
	words = (int*) malloc(sizeof(int) * n_threads);
	args_ = (int*) malloc(sizeof(int) * n_threads);

	int fd = open(file_name, O_RDWR);
	if (fd < 0) {
		printf("Can't open file\n");
		return 0;
	}
	
	struct stat stats;
	stat(file_name, &stats);
	file_size = stats.st_size;
	printf("file size: %d\n", file_size);
	

	file = (char*) mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (file == MAP_FAILED) {
		printf("mmap error\n");
		return 0;
	}


	int all_lines = 0, all_words = 0;
	
	int i;
	for (i = 0; i < n_threads; ++i) {
		starts[i] = i * file_size / n_threads;
		pos[i] = starts[i];

		/* решаем проблемку с разрезанным словом */
		if (i && is_word(file[pos[i]]) && is_word(file[pos[i] - 1]))
			--all_words;

		args_[i] = i;
		pthread_create(&threads[i], 0, process, &args_[i]);
	}

	/* ждем пока они сдохнут */
	for (i = 0; i < n_threads; ++i) {
		pthread_join(threads[i], 0);
		all_words += words[i];
		all_lines += lines[i];
	}

	printf("words: %d, lines: %d", all_words, all_lines);

	return 0;
}
