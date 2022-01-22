/*
*   DKU Operating System Lab
*           Lab2 (Vehicle Production Problem)
*           Student id : 32191197 
*           Student name : Kim Chae-eun 
*
*   lab1_sync_types.h :
*       - lab2 header file.
*       - must contains Vehicle Production Problem's declations.
*
*/

#ifndef _LAB2_HEADER_H
#define _LAB2_HEADER_H

#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>


#define NUM 5
#define MAX 10

typedef struct Vehicle {
    int id;
    int prodNum;
}Vehicle;

typedef struct Queue {

    int fill_ptr;
    int use_ptr;
    int buf[MAX];
    pthread_mutex_t fillLock;
    pthread_mutex_t useLock;
}Queue;

void init(Queue* q);
void put(Queue* q, int value);
int get(Queue* q);
void* producer(void* arg);
void* consumer(void* arg);



#endif /* LAB2_HEADER_H*/


