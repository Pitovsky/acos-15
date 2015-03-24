#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(int argc, char** argv){
	int i,j=1,k,l,m, co;
		sscanf(argv[argc-1],"%d",&co);
	for(i=0;i<co;i++){
		j=fork();
		if(j==0){
			for(k=i+1;k<argc;k+=co){
				FILE *myfile;
					myfile=fopen(argv[k], "r");
				char c='a';
				int so=0;
				int st=0;
					while(!feof(myfile)){  
						scanf("%c", &c);
						 if(((c<='z')&&(c>='a'))||((c<='Z')&&(c>='A'))||((c<='9')&&(c>='0'))||(c=='_')||(c=='-')){st=0;}else
							if(st==0){so++;st=1;};
					}
				printf("%s %d", argv[k], so);
				
				}
		};
		if(j==0)
			break;
	}	
	return(0);
}		
