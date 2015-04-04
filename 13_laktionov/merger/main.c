#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, const char * argv[]) {

    char* bashQuery = (char*)malloc(2*sizeof(char));
    FILE** pipes = (FILE**)malloc(sizeof(FILE*));
    int pipesCounter = 0, closedCounter = 0;
    

    for (int i = 1; i < argc; ++i){
        if (argv[i][0] != '-'){
            if (i > 1){
                //конец параметров предыдущей программы ---> запуск
                pipes = realloc(pipes, i*sizeof(FILE*));
                pipes[pipesCounter] = popen(bashQuery, "r");
                ++pipesCounter;
            }
            //path to program
            bashQuery = realloc(bashQuery, strlen(argv[i]) + 1);
            strcpy(bashQuery, argv[i]);
        }else{
            //parameters
            bashQuery = realloc(bashQuery, strlen(bashQuery) + strlen(argv[i]) + 2);
            strcat(bashQuery, " ");
            strcat(bashQuery, argv[i]);
        }
    }
    
    char input[1024];
    while(1){
        //++lineCounter;
        for (int i = 0; i < pipesCounter; ++i){
            if (feof(pipes[i])){
                ++closedCounter;
                strcpy(input, ""); //вывода нет
            }else{
                fscanf(pipes[i], "%s\n", input);
            }
            printf("%s\t", input);
        }
        if (closedCounter == pipesCounter)
            break;
        closedCounter = 0;
    }

    return 0;
}
