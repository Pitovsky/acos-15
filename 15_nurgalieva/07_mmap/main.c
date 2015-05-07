 #include "header.h" 
 
 double get(int col, int str, double* file_pointer)
 {
	 return *(file_pointer + col * str - 1 + col - 1);
 }
 
 void set(int col, int str, double new_val, double* file_pointer)
 {
	 *(file_pointer + col * str - 1 + col - 1) = new_val;
 }
 
 double sum_row(int str, int col, double* file_pointer)
 {
	double sum = 0;
	int i;
	
	for ( i = 0; i < col; ++i)
	{
		sum += *(file_pointer + col * str - 1 + i);
	}
	return sum;
}

int main(int argc, char** argv)
{
	int fd = open(argv[1],
			   O_RDWR|O_CREAT,
			   S_IRWXU);
			   
	struct stat file_info;
	
	fstat(fd, &file_info);
	
	if (argc < 3)
	{
		fprintf(stdout, "Incorrect data");
		exit(0);
	}
	
	int num_colmn = atoi(argv[2]);
	int num_str = 1;
	
	if (argc == 4)
	{
		num_str = atoi(argv[3]);
		if (ftruncate(fd, num_str * num_colmn * sizeof(double)) != 0)
		{
			fprintf(stdout, "error\n");
			exit(0);
		}
	}
	else
	{
		fstat(fd, &file_info);
		num_str = ((file_info.st_size) / (sizeof(double))) / num_colmn;
		//printf("there is no error\n");
	}
		
	
	double * file_pointer = mmap(0, file_info.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0); 
	
	char command[20] = "";
	
	while (strcmp(command, "exit"))
	{
		fscanf(stdin,"%s", command);
		if (!strcmp(command, "get"))
		{ 
			int col, str;
			fscanf(stdin,"%d", &col);
			fscanf(stdin, "%d", &str);
			fprintf(stdout, "%lf \n", get(col, str, file_pointer));
		}
		if (!strcmp(command, "getinfo"))
		{
			fprintf(stdout, "There are %d str and %d colmn\n", num_str, num_colmn);
		}	
		if (!strcmp(command, "set"))
		{
			int col, str;
			double new_val;
			fscanf(stdin,"%d", &col);
			fscanf(stdin, "%d", &str);
			fscanf(stdin,"%lf", &new_val);
			set(col, str, new_val, file_pointer);
		}
		if (!strcmp(command, "sum"))
		{
			fscanf(stdin,"%s", command);
			if (!strcmp(command, "row"))
			{
				int str;
				fscanf(stdin, "%d", &str);
				fprintf(stdout, "sum row: %lf\n", sum_row(str, num_colmn, file_pointer));
			}
		}
	}
	
	return 0;
}
