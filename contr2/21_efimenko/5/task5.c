#include<assert.h>
#include<dirent.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
//тоже слишком далеко
const char* joinpath(const char* dirname, const char* filename){
	assert(dirname != NULL && filename != NULL);
	char* result = malloc(strlen(dirname)+strlen(filename)+2);
	if (result == NULL)
		return NULL;
	strcpy(result, dirname);
	strcat(result, "/");
	strcat(result, filename);
	return result;			
}

int op_dir(const char* path, char* c){
	int res = 0;
	DIR* d = opendir(path);
	assert(d!=NULL);
	struct dirent* entry;
	for(entry = readdir(d); entry != NULL; entry = readdir(d)){
		assert(entry != NULL);
		const char* filepath = joinpath(path, entry->d_name);
		struct stat buf;
		lstat(filepath, &buf);
		if(S_ISDIR(buf.st_mode)){
			res+=op_dir(filepath, c);
		} else if (c = "co"){
			res+=1;
		} else{
			res+=buf.st_size;
		}
	}
	return res;
}

	
	
int main(){
	char command[2];
	char* path = (char*)malloc(sizeof(char)*256);
	scanf("%s %s", &command, &path);
	int res = op_dir(path, command);
	

}
