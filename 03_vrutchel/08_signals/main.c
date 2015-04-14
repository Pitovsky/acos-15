#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>

//Вычисление факториала
double factorial (int n)
{
  return (n < 2) ? 1 : n * factorial (n - 1);
}

//Вычисление cos
double my_cos(double x, int deep)
// x - аргумент функции cos, deep - степень ряда Тейлора
{
	double sum = 0;
    int n, i;
	for(i = 0; i <= deep; i++)
	{
		n = i*2;
		sum += ((i % 2) ? (-1) : 1) * pow(x, (double)n) / factorial(n);
	}
	return sum;
}

//Вычисление sin
double my_sin(double x, int deep)
{
	double sum = 0;
	int n, i;

	for(i = 0; i <= deep; i++)
	{
		n = i*2+1;
		sum += ((i % 2) ? (-1) : (1)) * pow(x, (double)n) / factorial(n);
	}
	return sum;

}

//Вычисление ln
double my_ln(double x, int deep)
{
    int i;
	double sum = 0;
	for(i = 1; i <= deep; i++)
		sum += ((i % 2) ? 1 : (-1)) * pow(x - 1, (double)i) / i;
}

//Вычисление exp
double my_exp(double x, int deep)
{
	double sum = 1;
	int i;
	for(i = deep; i > 0; i--)
		sum = 1 + (x / i)*sum;		//По схеме Горнера
	return sum;
}

int is_stop;

//Программа-обработчик сигнала SIGINT
void signHandler(int i) {
    is_stop = 1;
}

//Программа-калькулятор
void calc(char *info) {

    char *cmd, *s;//указатели на команду и вспомогательный, для разбора строки
    double start, step;//переменные для начальной точки и шага
    int deep;//степень

    cmd = info;
    s = cmd;
    while(*s!=' ' && *s!='\t' && *s)//до тех пор пока символ входной строки не равен пробелу, табуляции и не достигнут конец строки
    {
        *s = tolower(*s);
        s++;
    }
    if(!*s)//если введён набор символов без пробелов и табуляций, необходимо сообщить об ошибке
    {
        printf("\nInvalid input '%s'\n", cmd);
        return;
    }

    *s = 0;//пробельный символ заменим на 0 (окончание строки) для корректного выполнения строкового сравнения
    s++;

    sscanf(s,"%lf %lf %d",&start,&step,&deep);

    if(!strcmp(cmd, "cos"))
        while(1)
        {
            if(is_stop) break;
            printf("cos(%lf) = %lf\n", start, my_cos(start, deep));
            start += step;
        }
    if(!strcmp(cmd, "sin"))
        while(1)
        {
            if(is_stop) break;
            printf("sin(%lf) = %lf\n", start, my_sin(start, deep));
            start += step;
        }
    if(!strcmp(cmd, "exp"))
        while(1)
        {
            if(is_stop) break;
            printf("exp(%lf) = %lf\n", start, my_exp(start, deep));
            start += step;
        }
    if(!strcmp(cmd, "ln"))
        while(1)
        {
            if(is_stop) break;
            if(start <= 0) {
                printf("ln(%lf) Wrong domain!\n", start);//Несоответствие области определения
            } else printf("ln(%lf) = %lf\n", start, my_ln(start, deep));
            start += step;
        }
    if(is_stop) return;
    printf("\nThe command '%s' is invalid\n", cmd);
    return;
}

int main(int argc, char **argv)
{

    is_stop = 0;
    struct sigaction sa;//описатель правил установки обработчика сигнала для нашей программы
    struct sigaction so;//описатель правил установки обработчика сигнала по умолчанию (системного обработчика)
    memset(&sa, 0, sizeof(struct sigaction));//обнуление содержимого sa

    sa.sa_handler = signHandler;//установка в вектрое функции обработки signHandler?????????
    sigemptyset(&(sa.sa_mask));	//обнулим маску сигналов
    //sa.sa_flags = SA_RESTART;		//установим необходимость перезапуска медленных системных вызовов (read, write ...)
					// (в случае их прерывания они вернут код ошмбки EINTR - надо не забыть это проверить и обработать)

    sigaction(SIGINT, &sa, &so);//перехват сигнала (а не установка ли это новой версии обработчика для сигнала SIGINT?)

    char buffer[512], *p_buf;

    while(1)  {
        p_buf = buffer;
        printf("\nEnter the task in format <func> <start> <step> <deep> or exit to finish the work\n: ");
        fgets(buffer, 512, stdin);

        while((*p_buf == ' ') || (*p_buf=='\t')) p_buf++;//проигнорируем пробелы и табуляции вначале строки...

        if(strlen(p_buf) < 4)		// грубая проверка корректности ввода по длине прочитанной строки
		      continue;		// Строк_а слишком короткая - повторим ввод...

		if(tolower(p_buf[0])=='e' && tolower(p_buf[1])=='x' && tolower(p_buf[2])=='i' && tolower(p_buf[3])=='t')//exit
            break;

        printf("\n");
        calc(p_buf);//вызываем калькулятор...
        is_stop = 0;
    }

    sigaction(SIGINT, &so, NULL);//восстановление обработчика сигнала по умолчанию (системного)


    return 0;
}
