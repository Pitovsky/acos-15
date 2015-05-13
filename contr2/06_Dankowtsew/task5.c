#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <dirent.h>
#include <linux/limits.h>

const size_t THREAD_NUMBER = 5;

pthread_mutex_t input_mutex;
pthread_mutex_t output_mutex;


void process_catalog(char *path, int operation, long* rez);

void analise_entry(char* path, char* entry_name, int operation, long* rez)
{
	struct stat entry_info;
    char entry_path[PATH_MAX + 1];

    sprintf(entry_path, "%s/%s", path, entry_name);
    if (lstat(entry_path, &entry_info) != 0)
    {
        //fprintf(stderr, "Can't open file: %s\n", entry_path);
        return;
    }

    if (S_ISDIR(entry_info.st_mode))
    {
		process_catalog(entry_path, operation, rez);
    }

    if (S_ISREG(entry_info.st_mode) || S_ISLNK(entry_info.st_mode))
    {
    	if (operation == 1)
    	{
    		//printf("file %s\n", entry_path);
    		(*rez)++;
    	}
    	else
    	{
    		//printf("file %s (size :%d)\n", entry_path, (int)entry_info.st_size);
    		(*rez) += entry_info.st_size;
    	}
    }
}

void process_catalog(char *path, int operation, long* rez)
{
    DIR *current_dir = opendir(path);

    if (current_dir == NULL)
    {
        fprintf(stderr, "Can't open catalog.\n");
        return;
    }

    struct dirent *entry;

    while (1)
    {
        entry = readdir(current_dir);

        if (entry == NULL)
        {
            break;
        }

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        analise_entry(path, entry->d_name, operation, rez);
    }

    closedir(current_dir);
}

void* run_thread(void* arg)
{
	while (1)
	{
		char comand[10];
		char path[PATH_MAX + 1];
		pthread_mutex_lock(&input_mutex);
		int code = scanf("%s%s", comand, path);
		//printf("%d\n", code);
		pthread_mutex_unlock(&input_mutex);
		if (code != 2 || code == EOF)
		{
			break;
		}
		long result = 0;
		if (strcmp(comand, "co") == 0)
		{
			process_catalog(path, 0, &result);
		}
		else
		{
			if (strcmp(comand, "cs") == 0)
			{
				process_catalog(path, 1, &result);
			}
			else
			{
				fprintf(stderr, "Wrong input.\n");
				exit(EXIT_FAILURE);
			}
		}
		
		pthread_mutex_lock(&output_mutex);
		printf("%p: %s %s\t%ld\n", arg, comand, path, result);
		pthread_mutex_unlock(&output_mutex);
		
	}
	return NULL;
}

int main(int argc, char** argv)
{
	int i;
	pthread_t threads[THREAD_NUMBER];
	pthread_mutex_init(&input_mutex, NULL);
	pthread_mutex_init(&output_mutex, NULL);
	for (i = 0; i < THREAD_NUMBER; ++i)
	{
		if (pthread_create(&threads[i], NULL, &run_thread, &threads[i]) == -1)
        {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
	}
	for (i = 0; i < THREAD_NUMBER; ++i)
	{
		pthread_join(threads[i], NULL);
	}
	pthread_mutex_destroy(&input_mutex);
	pthread_mutex_destroy(&output_mutex);
	return 0;
}