#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    int output_fd1 = open("output1.txt", O_RDWR | O_CREAT);
    int output_fd2 = open("output2.txt", O_RDWR | O_CREAT);
    
    pid_t pid = fork();
    if (pid)
    {
        if (output_fd1 < 0)
        {
            perror("Can't open a file\n");
        }
        dup2(output_fd1, STDOUT_FILENO);
        execl(argv[1], NULL);
    }
    
    pid = fork();
    if (pid)
    {
        if (output_fd2 < 0)
        {
            perror("Can't open a file\n");
        }
        dup2(output_fd2, STDOUT_FILENO);
        execl(argv[2], NULL);
    }
    
    wait(NULL);
    wait(NULL);
    
    
    
    return 0;
}
