#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(int argc, char** argv){
	int i,j, co; char* str="  ";
		FILE *myfile;
		myfile=fopen("test.txt", "r");  
	str=(char *)malloc(255* sizeof(char));
	if((argc<3)||(strlen(argv[1])!=strlen(argv[2]))){printf("error"); return(0);}else{
		while(!feof(myfile)){
			if(str!="  "){
			printf("%s\n", str);}
			fscanf(myfile,"%s",str);
			for(i=0;i<strlen(str);i++){				
				co=0;
				for(j=0;j<strlen(argv[1]);j++)
					if((str[i]==argv[1][j])&&(co==0)){str[i]=argv[2][j];co=1;}
				}
			
			}
		};
	return(0);
} 
