#include "hw2_output.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

int **A = NULL;
int **B = NULL;
int **C = NULL;
int **D = NULL;
int **E = NULL;
int **F = NULL;
int **result = NULL;
int row1 = 0;
int col1 = 0;
int row2 = 0;
int col2 = 0;
pthread_t *threads;
int *columnSignal;
int *rowSignal;
sem_t *semArray;
sem_t *matrixEsem;
sem_t *matrixFsem;
sem_t myMutex;

typedef struct {
    int **first;
    int **second;
    int **dest;
    int rowIndex;
    int length;

} addition_struct;

typedef struct { // first matrix=E, second matrix=F, dest matrix=result
    int rowIndex;
    int rowLength;
    int columnLength;
} multiplication_struct;

void joinThreads(void){
    for(int i = 0; i < 2*row1+row2;i++){
        pthread_join(threads[i],NULL);
    }
}

void printMatrix(void){
    for(int i = 0; i < row1; i++){
        for(int j = 0; j < col2; j++){
            if( j != col2-1 ){
                printf("%d ",result[i][j]);
            }
            else{
                printf("%d",result[i][j]);
            }
        }
        if( i != row1-1 ){
            printf("\n");
        }

    }
    return;
}

void getInputs(void){

    scanf("%d %d",&row1,&col1);
    A = (int **) malloc(sizeof(int *) * row1);
    E = (int **) malloc(sizeof(int *) * row1);
    for(int i = 0; i < row1; i++){
        A[i] = (int *) malloc(sizeof(int) * col1);
        E[i] = (int *) malloc(sizeof(int) * col1);
    }
    for(int i = 0; i < row1; i++){
        for(int j = 0; j < col1; j++){
            scanf("%d",&A[i][j]);
            E[i][j] = -1;
        }
    }

    scanf("%d %d",&row1,&col1);
    B = (int **) malloc(sizeof(int *) * row1);
    for(int i = 0; i < row1; i++){
        B[i] = (int *) malloc(sizeof(int) * col1);
    }
    for(int i = 0; i < row1; i++){
        for(int j = 0; j < col1; j++){
            scanf("%d",&B[i][j]);
        }
    }

    scanf("%d %d",&row2,&col2);
    C = (int **) malloc(sizeof(int *) * row2);
    F = (int **) malloc(sizeof(int *) * row2);
    for(int i = 0; i < row2; i++){
        C[i] = (int *) malloc(sizeof(int) * col2);
        F[i] = (int *) malloc(sizeof(int) * col2);
    }
    for(int i = 0; i < row2; i++){
        for(int j = 0; j < col2; j++){
            scanf("%d",&C[i][j]);
            F[i][j] = -1;
        }
    }

    scanf("%d %d",&row2,&col2);
    D = (int **) malloc(sizeof(int *) * row2);
    for(int i = 0; i < row2; i++){
        D[i] = (int *) malloc(sizeof(int) * col2);
    }
    for(int i = 0; i < row2; i++){
        for(int j = 0; j < col2; j++){
            scanf("%d",&D[i][j]);
        }
    }

    result = (int **) malloc(sizeof(int *) * row1);
    for(int i = 0; i < row1; i++){
        result[i] = (int *) malloc(sizeof(int) * col2);
    }

    return;
}

void setSemaphores(void){

    rowSignal = malloc(sizeof(int) * row1);
    matrixEsem = malloc(sizeof(sem_t) * row1);
    for(int i = 0; i < row1; i++){ //
        rowSignal[i] = col1; // initialize semaphore array to col1-row2
        sem_init(&matrixEsem[i],0,0); // semaphore for each row of E matrix
    }

    semArray = malloc(sizeof(sem_t) * col2);
    matrixFsem = malloc(sizeof(sem_t) * col2);
    columnSignal = malloc(sizeof(int) * col2);
    for(int i = 0; i < col2; i++){
        columnSignal[i] = row2; // initialize semaphore array to col1-row2
        sem_init(&semArray[i],0,1); // semaphore array to edit columnArray synchronized
        sem_init(&matrixFsem[i],0,0); // seamphore for each column of F matrix
    }

    sem_init(&myMutex,0,1); // mutex for printing

    return;
}

void *addition(void *args){
    addition_struct *actual_args = args;
    int len = actual_args->length;
    int **dest = actual_args->dest;
    int **first = actual_args->first;
    int **second = actual_args->second;
    int rowIndex = actual_args->rowIndex;
    for(int j = 0; j < len; j++){
        dest[rowIndex][j] = first[rowIndex][j] + second[rowIndex][j];
        if( first == A ){ // E matrix getting created
            sem_wait(&myMutex);
            hw2_write_output(0,rowIndex+1,j+1,dest[rowIndex][j]);
            sem_post(&myMutex);

        }
        else{ // F matrix getting created
            sem_wait(&myMutex);
            hw2_write_output(1,rowIndex+1,j+1,dest[rowIndex][j]);
            sem_post(&myMutex);

            sem_wait(&semArray[j]);
            columnSignal[j] -= 1;
            sem_post(&semArray[j]);

            if( columnSignal[j] == 0){ // if column is fully calculated in Matrix F, send signal for multiplication
                sem_post(&matrixFsem[j]);
            }
        }
    }

    if( first == A){ // if row is fully calculated in Matrix E, send signal for multiplication
        sem_post(&matrixEsem[rowIndex]);
    }

    free(args);
    return NULL;
}

void *multiplication(void *args){
    multiplication_struct  *actual_args = args;
    int rowIndex = actual_args->rowIndex;
    int rowLength = actual_args->rowLength;
    int columnLength = actual_args->columnLength;
    int temp = 0;
    for(int col = 0; col < columnLength; col++){ // select column
        sem_wait(&matrixEsem[rowIndex]); // wait matrix E's row to finish
        sem_post(&matrixEsem[rowIndex]); // signal other waiting threads for this row
        sem_wait(&matrixFsem[col]); // wait matrix F's column to finish
        sem_post(&matrixFsem[col]); // signal other waiting thread for this column
        temp = 0;
        for(int j = 0;j < rowLength; j++){ // multiply each cell
            temp += E[rowIndex][j] * F[j][col];
        }
        result[rowIndex][col] = temp;
        sem_wait(&myMutex);
        hw2_write_output(2,rowIndex+1,col+1,temp);
        sem_post(&myMutex);
    }
    free(args);
    return NULL;
}

void createThreads(void){
    threads = malloc(sizeof(pthread_t) * (2*row1 + row2));
    int count = 0;
    for(int i = 0; i < row1; i++,count++){ // Addition threads for Matrix A and B
        addition_struct *args = malloc(sizeof(addition_struct));
        args->rowIndex = i;
        args->first = A;
        args->second = B;
        args->dest = E;
        args->length = col1;
        pthread_create(&threads[count],NULL,addition,args);
    }
    for(int i = 0 ;i < row2; i++,count++){ // Addition threads for Matrix C and D
        addition_struct *args = malloc(sizeof(addition_struct));
        args->rowIndex = i;
        args->first = C;
        args->second = D;
        args->dest = F;
        args->length = col2;
        pthread_create(&threads[count],NULL,addition,args);
    }

    for(int i = 0 ;i < row1; i++,count++){ // Multiplication threads for E and F
        multiplication_struct *args = malloc(sizeof(multiplication_struct));
        args->rowIndex = i;
        args->rowLength = col1;
        args->columnLength = col2;
        pthread_create(&threads[count],NULL,multiplication,args);
    }
}

int main(void){

    getInputs();

    setSemaphores();

    hw2_init_output();

    createThreads();

    joinThreads();

    printMatrix();

    return 0;
}