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

#define MAX_PATH 512

int WordsInFile(FILE* f){
	int count = 0, prev, curr = '.';
	while (!feof(f)){
		prev = curr;
		curr = getc(f);
		if ( ((curr >= 'a' && curr <='z') || (curr >= 'A' && curr <= 'Z') || 
				(curr >= '0' && curr <= '9') || curr == '_') &&
			!((prev >= 'a' && prev <= 'z') || (prev >= 'A' && prev <= 'Z') ||
				(prev >= '0' && prev <= '9') || prev == '_')){
			count++;
		}
	}
	return count;
}


void Process(char* dir, int depth, int limit, char links){
	if (limit != 0 && depth >= limit){
		return;
	}
	DIR* CDir = opendir(dir);
	if (CDir == NULL){
		printf("Couldn't open this directory\n");
		return;
	}
	struct dirent* celement = readdir(CDir);
	char filepath[MAX_PATH];
	char checkerlink = 0;

	while (celement != NULL){
		if (strcmp(celement->d_name, ".") == 0 || strcmp(celement->d_name, "..") == 0){
			celement = readdir(CDir);
		}else{
			if (checkerlink == 0){
				strcpy(filepath, dir);
				strcat(filepath, "/");
				strcat(filepath, celement->d_name);
			}
			checkerlink = 0;
			struct stat cfile;
			char checker = lstat(filepath, &cfile);
			if (checker == 0){
				if (S_ISLNK(cfile.st_mode)){
					if (links == 1){
						int n = readlink(filepath, filepath, sizeof(filepath) - 1);
						filepath[n] = '\0';
						checkerlink = 1;
					}else{
						celement = readdir(CDir);
					}
				}
				if (S_ISDIR(cfile.st_mode)){
					Process(filepath, depth+1, limit, links);
					celement = readdir(CDir);
				}
				if (S_ISREG(cfile.st_mode)){
					FILE* f = fopen(filepath, "r");
					if  (f != NULL){
						int words = WordsInFile(f);
						printf("Words in file %s: %d\n", filepath, words);
						fclose(f);
					}else{
						printf("Couldn't open file %s\n", filepath);
					}
					celement = readdir(CDir);
				}
			}
		}
	}
}

int main(int argc, char* argv[]){
	if (argc != 4 && argc != 5){
		printf ("Wrong Input!!!\n");
		return 1;
	}
	char linker = 1;	
	if (argc == 5){
		linker = 0;
	}
	int limit = atoi(argv[3]);
	Process(argv[1], 0, limit, linker);
	return 0;
}














