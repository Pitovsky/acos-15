#include "matrix.h"

#include <stdlib.h>

Matrix *createMatrix(void *data, long int rowQty, long int columnQty)
{
    Matrix *matrix = (Matrix *)malloc(sizeof(Matrix));
    
    matrix->rowQty = rowQty;
    matrix->columnQty = columnQty;
    
    matrix->elements = (double **)malloc(sizeof(double *) * rowQty);
    
    for (size_t i = 0; i < rowQty; i++)
    {
        matrix->elements[i] = (double *)malloc(sizeof(double) * columnQty);
        for (size_t j = 0; j < columnQty; j++)
        {
            int n;
            if ((n = sscanf(data, "%lf", &matrix->elements[i][j])) == 0 && n != EOF)
            {
                perror("Undefined symbols in input");
                exit(1);
            }
            size_t offset = (void *)strstr(data, " ") - data;
            data += offset + 1;
        }
    }
    return matrix;
}

void printMatrix(Matrix *matrix)
{
    for (size_t i = 0; i < matrix->rowQty; i++)
    {
        for (size_t j = 0; j < matrix->columnQty; j++)
        {
            printf("%f ", matrix->elements[i][j]);
        }
        printf("\n");
    }
}

void destroyMatrix(Matrix *matrix)
{
    for (size_t i = 0; i < matrix->rowQty; i++)
    {
        free(matrix->elements[i]);
    }
    free(matrix->elements);
    return;
}

double sumRow(Matrix *matrix, long int row)
{
    if (row > matrix->rowQty)
        return 0;
    double sum = 0;
    for (size_t i = 0; i < matrix->columnQty; i++)
    {
        sum += matrix->elements[row][i];
    }
    return sum;
}

double sumColumn(Matrix *matrix, long int column)
{
    if (column > matrix->columnQty)
        return 0;
    double sum = 0;
    for (size_t i = 0; i < matrix->rowQty; i++)
    {
        sum += matrix->elements[i][column];
    }
    return sum;
}

void swap(Matrix *matrix, long int firstRow, long int secondRow)
{
    void *buffer = malloc(matrix->columnQty * sizeof(double));
    memcpy(buffer, matrix->elements[firstRow], matrix->columnQty * sizeof(double));
    memcpy(matrix->elements[firstRow], matrix->elements[secondRow], matrix->columnQty * sizeof(double));
    memcpy(matrix->elements[secondRow], buffer, matrix->columnQty * sizeof(double));
    free(buffer);
}