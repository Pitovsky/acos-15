/*При компиляции писать флаг -lpthread*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

char* stringsWork(char *into, int *len) {
    int i = 0, j = 0;
    char *tmp = NULL;

    while(into[i] != '\n'&& into[i] != EOF && into[i] != '\0') {//проходим по строке, пока не конец
        if(into[i] >= 'A' && into[i] <= 'Z') j++;//Считаем число заглавных букв в строке
        i++;
    }

    tmp = (char*)malloc(i + j + 1);//Выделяем память под новую строку, где i - длина исходной строки
    //j - число заглавных букв и 1 - символ '\0' в конце строки
    //потом нужно не забыть удалить tmp

    j = 0;//счётчик символов в новой строке
    i = 0;
    while(into[i] != '\n' && into[i] != EOF && into[i] != '\0') {
        if(into[i] >= 'a' && into[i] <= 'z') {//каждая строчная буква заменяется на заглавную копию
            tmp[j] = (char)toupper(into[i]);
        } else if(into[i] >='A' && into[i] <= 'Z') {
            tmp[j] = (char)tolower(into[i]);
            tmp[j + 1] = tmp[j];//Следующему символу строки присваиваем то же значение
            j++;
        } else {//это не буква
            tmp[j] = into[i];
        }
        i++; j++;
    }
    tmp[j] = '\0';
    *len = j;//Возможно, не нужно будет возвращать длину строки
    return tmp;
}

//Структура, которая будет использоваться в массиве
struct beg_end
{
	unsigned int beg;
	unsigned int end;
};

struct beg_end *fillingArray(char *into, int size, int *c_str) {
//array		- массив, который необходимо заполнить (изначально обнулён),
//into		- база (входной массив)
//size		- размер массива (into) не должен быть нулевым(Нужно проверить перед вызовом функции)
//c_str		- количество строк (возвращается через указатель)

    int i, j, k;
	struct beg_end *array;

	*c_str = 0; //Обнуляем счётчик строк
	for(i = 0; i < size; i++)	//Проходим по into, считаем количество строк
	{
		if(into[i] == '\n' || !into[i])
			(*c_str)++;
	}
	if(into[i - 1] != '\n' && into[i - 1]) //Массив заканчивается не 0 и не '\n'
		(*c_str)++;

	array = (struct beg_end*)malloc((*c_str) * sizeof(struct beg_end));//в массиве будет столько элементов, сколько строк в файле (into)

	if(size == 1)
	{
		array[0].beg = 0;
		array[0].end = 0;
		return array;
	}

	j = 0;//Счетчик элементов в массиве array
	for(i = 0; i < size; i++)	//Проходим по into и находим начала строк
	{
		if(into[i] == '\n' || !into[i]) //Конец строки
			if(i)	//не начало файла
			{
				array[j].end = i; //Записываем индекс конца строки
				k = i - 1;
				while(k && into[k] != '\n' && into[k]) k--;
				array[j].beg = k ? k + 1 : 0; //Записываем индекс начала строки
				j++;
			}
			else //В начале файла '\n', т.е. нулевая строка
			{
				array[j].beg = 0; //Записываем индекс начала строки
				array[j].end = 0; //Записываем индекс конца строки
				j++;
			}
	}
	if(i && j < (*c_str)) //Массив заканчивается не 0 и не '\n'
	{
		array[j].end = i; //Записываем индекс конца строки
		k = i - 1;
		while(into[k] != '\n') k--;
		array[j].beg = k + 1; //Записываем индекс начала строки
	}

	return array;
}

int main(int argc, char **argv)
{
    int len = 0;
    char *str;

    printf("%s\n", "AfsawHHooo43mOwmOw");
    str = stringsWork("AfsawHHooo43mOwmOw", &len);
    printf("%s %d\n", str, len);
    if(str) free(str);


    /*Нужно создать три области разделяемой памяти*/

    int fd = open("file.txt", O_RDWR);//fd - будет файловым дескриптором нового открытого файла
    //здесь определим размер открытого файла
    struct stat info_file;
    fstat(fd, &info_file);
    int size = info_file.st_size;

    printf("Size of file: %d\n", size);

    char *pointer1;//указатель на первую область разделяемой памяти
    struct beg_end *pointer2;
    char *pointer3;
    int shmid1;//дескриптор для первой области разделяемой памяти
    int shmid2, shmid3;
    key_t key1;//ключ для первой области разделяемой памяти
    key_t key2, key3;
    char *pathname1 = "1.c";//Имя файла, используется для генерации ключа
    char *pathname2 = "2.c"; char *pathname3 = "3.c";

     //Сначала создадим область, где будет отражение файла на память
    if((key1 = ftok(pathname1, 1)) == -1) {
        perror("Error with getting key1\n");
        exit(errno);
    }
    if((shmid1 = shmget(key1, size, 0666|IPC_EXCL|IPC_CREAT)) == -1) {//получение дескриптора 1 области разделяемой памяти
        perror("Error with opening shared memory1\n");
        exit(errno);
    }
    pointer1 = (char*)shmat(shmid1, NULL, 0);//включение области разделяемой памяти в адресное пространство текущего процесса
    //здесь теперь отражаем открытый файл на первую область разделяемой памяти
    void *file_addr = mmap((void*)pointer1, //при удачном выполнении mmap возвращает указатель на область с отражёнными данными
                            size,
                            PROT_READ | PROT_WRITE,
                            MAP_SHARED,
                            fd,
                            0
                            );
    if(file_addr == NULL) {
        perror("Map failed\n");
        exit(errno);
    }
    close(fd);//после отображения файла на память - закрываем файл


    //создадим массив размера size, в котором будем хранить 0 или 1, в зависимости от того, начинается ли на этом байте строка, или нет
    struct beg_end *work_array = (struct beg_end*)calloc(size * sizeof(struct beg_end), 1);
    //заполняем массив - отдельная функция
    int *c_str;
    if(size == 0) {
        perror("The size of file is 0\n");
        exit(-1);
    }
    work_array = fillingArray(pointer1, size, c_str);
    work_array = (struct beg_end*)realloc(work_array, (*c_str) * sizeof(struct beg_end));//изменяем размер массива work_array

    //!!!Отладочный вывод
    int i;
    for(i = 0; i < (*c_str); i++) {
        printf("%d-%d  ", work_array[i].beg, work_array[i].end);
    }
    printf("\n");

    //Теперь создадим вторую область разделяемой памяти. Здесь будет находиться семафор и специальный массив -
    if((key2 = ftok(pathname2, 1)) == -1) {
        perror("Error with getting key2\n");
        exit(errno);
    }

    int size2 = (*c_str) * sizeof(struct beg_end);
    if((shmid2 = shmget(key2, size2, 0666|IPC_EXCL|IPC_CREAT)) == -1) {//получение дескриптора 2 области разделяемой памяти
        perror("Error with opening shared memory2\n");//!!!ТУТ НИЧЕГО НЕ ПОЛУЧАЕТСЯ!
        exit(errno);
    }//!!!Здесь возникла проблема с "открытием" разделяемой памяти:
    //!!!При первом открытии её не существует (No such file or directory), нужно писать ещё флаг IPC_CREAT
    //!!!При следующем открытии, если не убирать флаг IPC_CREAT, то возникает другая проблема: File exists
    //!!!Если останется время - разобраться с этой проблемой!
    pointer2 = (struct beg_end*)shmat(shmid2, NULL, 0);
    //Семафор не буде храниться во второй области разделяемой памяти, его значение лишь будет проверяться каждый раз
    //Когда какой-то из процессов заходет обратиться к этой области
    sem_t *semaphore2 = sem_open("/my_semaphore2", O_CREAT|O_RDWR, S_IRWXU, 1);//открытие семафора, возвращает указатель на описатель семафора
    if(semaphore2 == SEM_FAILED) {
        perror("Can't open semaphore2\n");
        exit(errno);
    }
    //вот здесь нужно будет записать массив в pointer2
    if(write(shmid2, work_array, (*c_str) * sizeof(struct beg_end)) == -1) {
        perror("Error writing into shmid2");
        exit(errno);
    }

    //после чего со спокойной душой и чистым сердцем удаляем массив
    free(work_array);

    //Теперь создаём третью область разделяемой памяти
     //Сначала создадим область, где будет отражение файла на память
    if((key3 = ftok(pathname3, 1)) == -1) {
        perror("Error with getting key3\n");
        exit(errno);
    }
    //!!!Уточнить размер
    if((shmid3 = shmget(key3, size * 2 + 256, 0666|IPC_EXCL|IPC_CREAT)) == -1) {//получение дескриптора 3 области разделяемой памяти
        perror("Error with opening shared memory3\n");
        exit(errno);
    }
    pointer3 = (char*)shmat(shmid3, NULL, 0);//включение области разделяемой памяти в адресное пространство текущего процесса
    //Создаём семафор на третью область разделяемой памяти
    sem_t *semaphore3 = sem_open("/my_semaphore3", O_CREAT|O_RDWR, S_IRWXU, 1);//открытие семафора, возвращает указатель на описатель семафора
    if(semaphore3 == SEM_FAILED) {
        perror("Can't open semaphore3\n");
        exit(errno);
    }

    //Будем считать, что подготовка главного процесса к работе закончена

    /*Работа дочерних процессов*/
    pid_t pid = fork();
    if(pid == 0) {//если это ребёнок, то начинаем работу
        //блокируем вторую область разделяемой памяти
        sem_wait(semaphore2);

        //Работа со второй областью разделяемой памяти
        //Думаю сделать так: буду проходить по области 2 и искать не первый ненулевой(по beg)элемент, а первый элемент не равный -1
        //Потом запоминаю в памяти процесса beg и end и в области 2 ставлю -1 в beg и в end
        int j = 0;
        //!!!Тут всё неправильно! Надо во-первых не выходить за границы массива
        //!!!Во вторых проверить, если весь массив уже занят -1 - убить ребёнка!
        while(j < (*c_str) && pointer2[j].beg != -1) j++;
        if(j == (*c_str)) {//если весь массив занят -1, то ребёнок нам больше не нужен - убиваем его
            exit(pid);
        }
        unsigned int begin = pointer2[j].beg;
        unsigned int end = pointer2[j].end;
        pointer2[j].beg = -1;
        pointer2[j].end = -1;

        //снимаем блокировку со второй области разделяемой памяти
        sem_post(semaphore2);

        //Чтение из первой области разделяемой памяти, работа с полученной строкой
        char *buf = (char*)malloc((end - begin + 1) * sizeof(char));
        if(read(shmid1, buf, end - begin + 1) == -1) {
            perror("Can't read from shmid1\n");
            exit(errno);
        }
        //Затем применим stringsWork к buf
        int *len;
        buf = stringsWork(buf, len);//Теперь buf - это та строка, которую нужно записать с третью область

        //Запись ответа в третью область
        sem_wait(semaphore3);

        //Тут, пользуясь буфером, begin и end запишем данные в третью область

        sem_post(semaphore3);
        //Убиваем процесс

    }

    pid = fork();//делаем ещё дочку
    if(pid == 0) {

    }//в конце работы удалить дочку

    /*Главный процесс формирует ответ и выводит его в stdout*/

    //Он должен сформировать ответ по порядку, то есть пользуясь заголовками и тд и тп (пока ещё не до конца представляю себе, как это сделать)

    sem_close(semaphore3);
    shmdt(pointer3);
    sem_close(semaphore2);
    shmdt(pointer2);
    munmap(file_addr, size);
    shmdt(pointer1);//окончили работу с первой областью разделяемой памяти
    return 0;
}
