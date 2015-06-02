#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int weak_check = 0;
int limit_static = 1024;
int limit_dynamic = 8192;
char read_ok[] = "Read %s OK\n";
char owerflow[] = "Owerflow\n";
char env_name[] = "MAX_BUF";
char weak_flag[] = "-weak";
char open_mode[] = "r";
char file_name[] = "input.txt";
char new_line = '\n';
char line_end = '\0';

void check_pal(char* input)
{
	int len = strlen(input); 
	int i = 0;
	int j = len - 1;
pal:;
		if (i > j) goto pal_ok;
		if (input[i] != input[j]) 
			if (weak_check) goto check_weak;
			else goto not_pal;
		i++;
		j--;
	goto pal;
pal_end:;
	
check_weak:;
	if(input[i] == ' ') { i++; goto pal;}
	if(input[j] == ' ') { j--; goto pal;}
	if(input[i] == '.') { i++; goto pal;}
	if(input[j] == '.') { j--; goto pal;}
	if(input[i] == ',') { i++; goto pal;}
	if(input[j] == ',') { j--; goto pal;}
	goto not_pal;
weak_end:;

pal_ok:;
	printf("pal OK\n");
	goto check_end;

not_pal:;
	printf("Not pal\n");

check_end:;
}

char* get(FILE* stream)
{
	char current_string[limit_static + 1];
	char c; int i = 0;
	char* newstr = current_string;
	c = getc(stream);
loop_begin:;
	while (1)
	{
		if (c == new_line) goto loop_end;
		if (c == line_end) goto loop_end;
		if (c == -1) goto loop_end;
		if (i == limit_static)
		{
			newstr = (char*)malloc(limit_dynamic + 1);
			memcpy(newstr, current_string, limit_static);
		}
		if (i == (limit_dynamic))
		{
			printf("%s", owerflow);
			exit(1);
		}

		newstr[i] = c;
		newstr[i+1] = line_end;

		i++;
		
		c = getc(stream);
		if (feof(stream)) goto loop_end;
		
	}
loop_end:;
	return newstr;
}
int main(int argc, char** argv)
{
	char* max_buf = getenv(env_name);
	if (max_buf != NULL) limit_dynamic = atoi(max_buf);
	
	if (argc == 2)
		if (!strcmp(weak_flag, argv[1]))
			weak_check = 1;
			
	int i = 0; char c;
	FILE* input = fopen(file_name, open_mode);
	
	while (1)
	{	
		char* string;
		string = get(input);
		if (feof(input)) goto finish;
		printf(read_ok, string);
		check_pal(string);
	}

finish:;
	fclose(input);
	return 0;
}

