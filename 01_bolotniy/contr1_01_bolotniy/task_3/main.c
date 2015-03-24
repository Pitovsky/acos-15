#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

void process (int readpipe, int id){
	while(1){
		char *str;
		int code  = read(readpipe,&str,sizeof(str));
		if (code == EOF){
			return;
		}

		printf("%s by %d",str,id);
		FILE *cur = fopen(str,"r");
		if (cur != NULL){
			int f=0;
			int ch =fgetc(cur);
			int num = 0;
			int l;
			while(ch != EOF){
				l = (((ch>='a')&&(ch<='z'))||((ch>='A')&&(ch<='Z'))||(ch=='-')||(ch=='_'));
				if((f==0)&&l){
					f=1;
					num++;
				}else {
				if ((!l)&&(f==1)){f = 0;}
				}
			ch=fgetc(cur);
			}
			printf("%s has  %d words",str,num);
		}
	}
}

int main (int argc, char **argv){
int i;

int n;
sscanf(argv[argc-1],"%d",&n);

int task[2];
pipe(task);
for(i=1;i<argc-1;i++){
	write(task[1],&argv[i],sizeof(char **));
}
printf("hey i am doing something\n");
int child;
for(i = 0; i < n; i++){
	printf("hey i entered the cycle\n");
	child = fork();
	if (child == 0){
		printf("hey i am child");
		process(task[0],i);
	}
}
return 0;
}
