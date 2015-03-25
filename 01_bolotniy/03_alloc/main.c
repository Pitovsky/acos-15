#include<stdio.h>

#define PageSize 1024

struct MemSeg{
	int size;
	int cond;
	struct MemSeg *next;
	struct MemSeg *prev;
};

void *MyMalloc(int size){
	static struct MemSeg *root;
	if (root == NULL){
		root = (struct MemSeg*)sbrk(PageSize);
		root->size = PageSize - sizeof(struct MemSeg);
		root->cond = 0;
		root->prev = NULL;
		root->next = NULL;
	}

	struct MemSeg *NextSeg = root;
	
	while (NextSeg->next != NULL){
		NextSeg = NextSeg->next;
	}

	struct MemSeg *LessSeg = root;
	while (LessSeg != NULL && ((LessSeg->cond != 0) || (LessSeg->size < size + sizeof(struct MemSeg)))){
		LessSeg = LessSeg->next;
	}
	
	if (LessSeg == NULL){
		printf("debug information: try to increase\n");
		NextSeg->next = (struct MemSeg *)sbrk(PageSize);
		NextSeg->next->cond = 0;
		NextSeg->next->size = PageSize - sizeof(struct MemSeg);
		NextSeg->next->prev = NextSeg;
		NextSeg->next->next = NULL;
		NextSeg = NextSeg->next;
	}

	LessSeg = root;
	while (LessSeg != NULL && ((LessSeg->cond != 0) || (LessSeg->size < size + sizeof(struct MemSeg)))){
		LessSeg = LessSeg->next;
	}

	if (LessSeg == NULL){
		fprintf(stderr, "too big call\n");
		return NULL;
	}else{
		printf("debug information: succes\n");
	}

	NextSeg->next = (struct MemSeg *)((void *)LessSeg + LessSeg->size - size);
	LessSeg->size = LessSeg->size - size - sizeof(struct MemSeg);

	NextSeg->next->next = NULL;
	NextSeg->next->prev = NextSeg;
	NextSeg->next->size = size;
	NextSeg->next->cond = 1;

	return (void *)(NextSeg->next) + sizeof(struct MemSeg);
}

void GetNeigh(struct MemSeg *point, struct MemSeg **RealPrev, struct MemSeg **RealNext)
{
    struct MemSeg *root = point;
    while (root->prev != NULL)
    {
        root = (struct MemSeg *)(root->prev);
    }
    *RealPrev = root;
    *RealNext = root;

    while (*RealPrev != NULL && (void *)(*RealPrev) + ((*RealPrev)->size) + sizeof(struct MemSeg) != point){
        *RealPrev = (*RealPrev)->next;
	}
    while (*RealNext != NULL && (void *)(*RealNext) != (void*)point + sizeof(struct MemSeg) + (point->size)){
        *RealNext = (*RealNext)->next;
	}

    return;
}

int MyFree(void* point)
{
    struct MemSeg *DelSeg = (struct MemSeg*)(point - sizeof(struct MemSeg));
    if (DelSeg == NULL)
    {
        fprintf(stderr, "free - null pointer.\n");
        return -1;
    }

    struct MemSeg* DelPrev = NULL;
    struct MemSeg* DelNext = NULL;
    GetNeigh(DelSeg, &DelPrev, &DelNext);
    DelSeg->cond = 0;

    if (DelPrev != NULL && DelPrev->cond == 0){
        DelPrev->size = DelPrev->size + (DelSeg->size) + sizeof(struct MemSeg);
        if (DelSeg->prev != NULL){
            DelSeg->prev->next = DelSeg->next;
	}
        if (DelSeg->next != NULL){
            DelSeg->next->prev = DelSeg->prev;
	}
        DelSeg = DelPrev;
    }
    if (DelNext != NULL && DelNext->cond == 0) {
        DelSeg->size = DelSeg->size + (DelNext->size) + sizeof(struct MemSeg);
        if (DelNext->prev != NULL){
            DelNext->prev->next = DelNext->next;
	}
        if (DelNext->next != NULL){
            DelNext->next->prev = DelNext->prev;
	}
    }
    return 0;
}

int main(){
	int i;
	int *a = (int *)MyMalloc(100*sizeof(int));
	for (i=0;i<100;i++){
		a[i]=0;
	}
	printf("%d\n",a[5]);
	int *b = (int *)MyMalloc(200*sizeof(int));
	for(i=0;i<200;i++){
		b[i]=0;
	}
	printf("%d\n",b[6]);
	MyFree(a);
	MyFree(b);
	return 0;
}
