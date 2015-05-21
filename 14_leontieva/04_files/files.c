#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <string.h>

int wordsInF(FILE* f){

	int flag = 1, count = 0;
	char c[2] = {0};
	while (!feof(f)){
		fgets (c, 2, f);
		if (isalnum(c[0]) || c[0] == 95)
			flag = 0;
		else if (flag == 0 ){
			count++;
			flag = 1;
		}
		c[0] = 0;
	}
	fclose(f);
	return count;
}
int wordsdir1(char *path){
//	int fd = open(path, O_RDONLY);
	DIR* d = opendir(path);
	if (d == NULL) { perror("can't open directory");}
	int count = 0;
	struct dirent *entry;
	char str[256];
	while ((entry = readdir(d)) != NULL) {
		if (strcmp(entry->d_name, ".") == 0){
			entry = readdir(d);
			continue;
		}
		if (strcmp(entry->d_name, "..") == 0){
			entry = readdir(d);
			continue;
		}
		strcpy(str, path);
		//strcat(str, "/");
		strcat(str,entry->d_name);
		FILE *f = fopen(str, "r");
		count+=wordsInF(f);
		printf("file %s - current number of words %d\n", str,count);
	}
	closedir(d);
	return count;
}

int wordsdir2(char* path){
	DIR* d = opendir(path);
        if (d == NULL) { perror("can't open directory");}
	struct dirent *entry;
	struct stat *dstat;
	char str[256];
	int count = wordsdir1(path);
        while ((entry = readdir(d)) != NULL) {
		if (stat(entry->d_name, dstat) != 0){ perror("can't get stat"); continue;}
		if (S_IFDIR == dstat->st_mode){
			printf("%s", entry->d_name);
			//strcpy(str, path);
	                //strcat(str, "/");
        	        //strcat(str,entry->d_name);
			//count += wordsdir1(str);
		}
	}
	return count;
}

int main(int argc, char** argv){

if (argc < 2) {
	perror("too few arguments");
	exit(EXIT_FAILURE);
}
//FILE *f = fopen(argv[1], "r");
//printf ("Number of words in file %d\n:", wordsInF(f));
 wordsdir2(argv[1]);
return 0;
}

