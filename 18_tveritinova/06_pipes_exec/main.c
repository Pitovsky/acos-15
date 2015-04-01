#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

void process_child(char* s)
{
    fprintf(stderr,"in process child %s\n", s);
    int t = execlp("curl","curl", s, "-s", NULL);
    if (t == -1)
    {
        fprintf(stderr, "can't execlp");
    }
}

void print_href(int pipe)
{
    //printf("in href\n");
    char stream[10];
    char stream_char;
    int count = 0;
    int key = 0;
    char result[PATH_MAX];
    int er_read;
    //printf("hello for\n");
    for (int i=0; i<8; i++)
    {
	//printf("hello!!!\n");
	er_read = read(pipe, &stream_char, 1);
	//printf("er_read = %d   %c\n", er_read, stream_char);
	stream[i] = stream_char;
    }
    er_read = read(pipe, &stream_char, 1);
    //printf("in href   %d\n", er_read);
    while (er_read == 1)
    {
	//printf("%s\n", stream);
        if (strcmp(stream, "<a href=") == 0)
	{
	    key = 1;
	}
	if ((stream_char == '"') && (key == 1))
	{
	    if (count == 1)
	    {
		printf("%s\n\n", result);
	        sprintf (result,"");
		key = 0;
		count = 0;
	    }
	    else
	    {
		count = 1;
	    }
	}
	if ((key == 1) && (stream_char != '"'))
	{
	    sprintf(result, "%s%c", result, stream_char);
	}
	for (int i=0; i<7; i++)
	{
	    stream[i] = stream [i + 1];
	}
	stream[7] = stream_char;
	er_read = read(pipe, &stream_char, 1);
    } 
}

int main(int argc, char** argv)
{
    char* fifo_name = getenv("URLS_SRC");
    //int f = open(argv[
    //if (f == -1)
    //{
    //	fprintf(stderr, "can't open file");
    //}
    if (fifo_name != NULL)
    {
	printf("%s\n", fifo_name);
        int f = open(fifo_name, O_RDONLY);
        if (f == -1)
        {
            fprintf(stderr, "Can't open fifo");
        }
        else
        {
            char s[PATH_MAX];
            char stream;
            int pipes[2];
            pipe(pipes);
            int i = 0;
            int pr;
            int er_read = read(f, &stream, 1);
            while (er_read == 1)
            {
                if (stream == '\n')
                {
                    pr = fork();
                    if (pr == 0)
                    {
			dup2(pipes[1], 1);
                        //printf("hi!\n");
			process_child(s);
			return 0;
                    }
                    s[0] = 0;
                }
                else
                {
                    while (s[i] != 0)
                    {
                        ++i;
                    }
                    s[i] = stream;
                    s[i+1] = 0;
                    i = 0;
                }
                er_read = read(f, &stream ,1);
            }
	    close(pipes[1]);
	    //close(f);
	    print_href(pipes[0]);
        }
    }
    else
    {
        fprintf(stderr, "no fifo_name");
    }
}

