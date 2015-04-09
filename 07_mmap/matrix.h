#ifndef ___7_mmap__matrix__
#define ___7_mmap__matrix__

#include <stdio.h>

typedef struct
{
    double **elements;
    long int rowQty;
    long int columnQty;
} Matrix;

Matrix *createMatrix(void *data, long int rowQty, long int columnQty);
void printMatrix(Matrix *matrix);
void destroyMatrix(Matrix *matrix);

double sumRow(Matrix *matrix, long int row);
double sumColumn(Matrix *matrix, long int column);

void swap(Matrix *matrix, long int firstRow, long int secondRow);

#endif /* defined(___7_mmap__matrix__) */
