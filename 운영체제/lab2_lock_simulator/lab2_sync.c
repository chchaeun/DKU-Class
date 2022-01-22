
/*
*   DKU Operating System Lab
*           Lab2 (Vehicle production Problem)
*           Student id : 32191197
*           Student name : Kim Chae-eun
*
*   lab2_sync.c :
*       - lab2 main file.
*       - must contains Vehicle production Problem function's declations.
*
*/

#include <aio.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "lab2_sync_types.h"


int PROD = 0;
int TimeQt = 0;
int idx = 0;

pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t fill = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int balance = 0;
int lock = 1;


void init(Queue* q) {
    q->fill_ptr = 0;
    q->use_ptr = 0;
    for (int i = 0; i < MAX; i++) {
        q->buf[i] = 0;
    }   
    if (lock == 3) {
        pthread_mutex_init(&q->fillLock, NULL);
        pthread_mutex_init(&q->useLock, NULL);
    }
}

void put(Queue* q, int value) {

    if (lock == 3) pthread_mutex_lock(&q->fillLock);
    q->buf[q->fill_ptr] = value;
    if (lock == 3) pthread_mutex_unlock(&q->fillLock);

    if (lock == 3) pthread_mutex_lock(&q->fillLock);
    q->fill_ptr = (q->fill_ptr + 1) % MAX;
    if (lock == 3) pthread_mutex_unlock(&q->fillLock);

    //printf("put value: %d, balance: %d\n", value, balance);

}

int get(Queue* q) {

    if (lock == 3) pthread_mutex_lock(&q->useLock);
    int tmp = q->buf[q->use_ptr];
    if (lock == 3) pthread_mutex_unlock(&q->useLock);

    if (lock == 3) pthread_mutex_lock(&q->useLock);
    q->use_ptr = (q->use_ptr + 1) % MAX;
    if (lock == 3) pthread_mutex_unlock(&q->useLock);

    printf("get value: %d, balance: %d\n", tmp, balance);

    return tmp;
}

Queue queue;

void* producer(void* arg) {

    Vehicle* v = (Vehicle*)arg; // 인자로 받은 Vehicle 구조체 배열
    pthread_mutex_t balanceLock;
    pthread_mutex_init(&balanceLock, NULL);

    if (lock == 2 || lock == 3) {
        pthread_mutex_lock(&mutex);
        while (balance == MAX)
            pthread_cond_wait(&empty, &mutex);
    }
    put(&queue, v[idx].id);
    if (lock == 2 || lock == 3) {
        pthread_cond_signal(&fill);
        pthread_mutex_unlock(&mutex);
    }

    if (lock == 3) {
        pthread_mutex_lock(&balanceLock);
    }
    balance++;
    if (lock == 3) {
        pthread_mutex_unlock(&balanceLock);
    }
    
    return NULL;
}


void* consumer(void* arg) {
    pthread_mutex_t balanceLock;
    pthread_mutex_init(&balanceLock, NULL);
    if (lock == 2 || lock == 3) {
        pthread_mutex_lock(&mutex);
        while (balance == 0)
            pthread_cond_wait(&fill, &mutex);
    }
    int tmp = get(&queue);    

    if (lock == 2 || lock == 3) {
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    }
    if (lock == 3) {
        pthread_mutex_lock(&balanceLock);
    }
    balance--;
    if (lock == 3) {
        pthread_mutex_unlock(&balanceLock);
    }
        
    return NULL;
}


int main() {

    Vehicle vehi[NUM];
    pthread_t pid[100000], cid[100000];

    double result = 0;
    struct timeval start, end;
    int menu = 0;

    printf("실행할 프로그램\n1. No lock 2. Coarse-grained Lock 3. Fine-grained Lock -> ");
    scanf("%d", &menu);

    printf("차량 총 생산량: ");
    scanf("%d", &PROD);
    printf("Time Quantum: ");
    scanf("%d", &TimeQt);

    for (int i = 0; i < NUM; i++) {
        vehi[i].id = i + 1;
        switch (i) {
        case 0:
            vehi[i].prodNum = (int)(PROD * 0.1);
        case 1:
        case 2:
        case 3:
            vehi[i].prodNum = (int)(PROD * 0.2);
        case 4:
            vehi[i].prodNum = (int)(PROD * 0.3);
        }
    }

    printf("\n== Vehicle Production Problem ==\n\n");
    if (menu == 1) {
        printf("1. No Lock Experiment\n");

        balance = 0;
        init(&queue);
        lock = 1;

        gettimeofday(&start, NULL);
        idx = 0;
        int round = 0;
        for (int k = 0; k < PROD; k++) {
   
            if (round == TimeQt || vehi[idx].prodNum == 0) {
                // 타임퀀텀이 찼거나, 차량이 모두 생산 되었으면 차량 종류 바꿔주고 round = 0
                idx = (idx + 1) % NUM;
                round = 0;
            }
            pthread_create(&pid[k], NULL, producer, (void*)&vehi);

            round++; // 타임퀀텀 체크
            vehi[idx].prodNum--;

            pthread_create(&cid[k], NULL, consumer, NULL);
        }

        for (int k = 0; k < PROD; k++) {
            pthread_join(pid[k], NULL);
            pthread_join(cid[k], NULL);
        }

        gettimeofday(&end, NULL);

        result = (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000000);

        printf("Final balance value: %d\n", balance);
        printf("Excution time: %f\n\n", result);
    }
    
    if (menu == 2) {
        printf("2. Coarse-grained Lock Experiment\n");

        balance = 0;
        init(&queue);
        lock = 2;

        gettimeofday(&start, NULL);

        idx = 0;
        int round = 0;
        for (int k = 0; k < PROD; k++) {
       
            if (round == TimeQt || vehi[idx].prodNum == 0) {
                // 타임퀀텀이 찼거나, 차량이 모두 생산 되었으면 차량 종류 바꿔주고 round = 0
                idx = (idx + 1) % NUM;
                round = 0;
            }
            pthread_create(&pid[k], NULL, producer, (void*)&vehi);

            round++; // 타임퀀텀 체크
            vehi[idx].prodNum--;

            pthread_create(&cid[k], NULL, consumer, NULL);
        }

        for (int k = 0; k < PROD; k++) {
            pthread_join(pid[k], NULL);
            pthread_join(cid[k], NULL);
        }


        gettimeofday(&end, NULL);

        result = (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000000);

        printf("Final balance value: %d\n", balance);
        printf("Excution time: %f\n\n", result);
    }
    
    if (menu == 3) {
        printf("3. Fine-grained Lock Experiment\n");
        balance = 0;
        init(&queue);
        lock = 3;
        gettimeofday(&start, NULL);

        idx = 0;
        int round = 0;

        for (int k = 0; k < PROD; k++) {

            if (round == TimeQt || vehi[idx].prodNum == 0) {
                // 타임퀀텀이 찼거나, 차량이 모두 생산 되었으면 차량 종류 바꿔주고 round = 0
                idx = (idx + 1) % NUM;
                round = 0;
            }
            pthread_create(&pid[k], NULL, producer, (void*)&vehi);

            round++; // 타임퀀텀 체크
            vehi[idx].prodNum--;

            pthread_create(&cid[k], NULL, consumer, NULL);
        }

        for (int k = 0; k < PROD; k++) {
            pthread_join(pid[k], NULL);
            pthread_join(cid[k], NULL);
        }

        gettimeofday(&end, NULL);

        result = (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000000);

        printf("Final balance value: %d\n", balance);
        printf("Excution time: %f\n\n", result);
    }
    return 0;
}
