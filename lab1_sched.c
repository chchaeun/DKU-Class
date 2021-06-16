/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32191197	 
*	    Student name : Kim Chae Eun 
*
*
*/

#include <aio.h>
#include <unistd.h>
#include <pthread.h>
#include <asm/unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include "lab1_sched_types.h"


// Queue Functions

void initQueue(QUEUE* q) {

	q->front = 0;
	q->rear = 0;

	printf("Enter Time quantum: ");
	scanf("%d", &q->timeQt);
}

void emptyQueue(QUEUE* q) {
	q->front = q->rear;
}
void insertQueue(QUEUE* q, int val) {
	if ((q->rear + 1) % QSIZE == q->front) {
		printf("Queue is full");
		return;
	}
	q->arr[q->rear] = val;
	q->rear = ++q->rear % QSIZE;
}
int getQueue(QUEUE* q) {
	int temp;

	temp = q->arr[q->front];
	q->front = ++q->front % QSIZE;
	return temp;
}

void setWorkload(PROCESS* p) {

	int i, j;

	for (i = 0; i < pc_num; i++) {
		p[i].name = 'A' + i;
		p[i].pid = i;
		p[i].state = READY;

		printf("Process %c\n", p[i].name);
		printf("Arrival Time: ");
		scanf("%d", &p[i].art);
		printf("Service Time: ");
		scanf("%d", &p[i].svt);
	}
	sortWorkload(p);
}
void sortWorkload(PROCESS* p) {
	
	int i, j;

	for (i = 0; i < pc_num - 1; i++) {
		for (j = i + 1; j < pc_num; j++) {
			if (p[i].art > p[j].art) {
				swapProc(&p[i], &p[j]);
			}
		}
	}
}
void swapProc(PROCESS* a, PROCESS* b) {
	PROCESS temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

int getSumSvt(PROCESS *p) {
	int i;
	int sum = 0;
	for (i = 0; i < pc_num; i++) {
		sum += p[i].svt;
	}
	sum += p[0].art-1;
	return sum;
}

void resultPrint(PROCESS* p, int** arr) {

	int i, j;	
	int count;
	int totalSvt = getSumSvt(p);
	i = 0;
	count = 0;
	while (count<pc_num) {
		if (p[i].pid == count) {
			printf("%c ", p[i].name);
			for (j = 0; j <= totalSvt; j++) {
				if (arr[i][j] == 0) printf("□ ");
				else printf("■ ");
			}
			printf("\n");
			i = 0;
			count++;
		}
		else i++;
	}
}

void FIFO(PROCESS* p) {
	int i, j, k, w;
	int count;
	int totalSvt;
	int time;
	int **stateArr;

	QUEUE* queue = malloc(sizeof(QUEUE)); // single queue
	PROCESS* temp = (PROCESS*)malloc(sizeof(PROCESS) * pc_num);

	for (i = 0; i < pc_num; i++) temp[i] = p[i];
	totalSvt = getSumSvt(temp);
	
	stateArr = (int**)malloc(sizeof(int*) * pc_num);
	for (i = 0; i < totalSvt; i++) {
		stateArr[i] = (int*)malloc(sizeof(int) * totalSvt);
	}

	initQueue(queue);

	for (i = 0; i < pc_num; i++) {
		insertQueue(queue, temp[i].pid);
	}	

	time = 1;
	for (i = 0; i < pc_num; i++) {

		int front = getQueue(queue);
		if (front == temp[i].pid) {

			while (time < temp[i].art) time++;
			if (time >= temp[i].art) {
				for (j = 0; j < time - 1; j++) {
					stateArr[i][j] = READY;
				}
			}
			while (temp[i].svt != 0) {
				stateArr[i][time - 1] = RUN;
				temp[i].svt--;
				time++;
			}
		}
		for (w = time - 1; w <= totalSvt; w++)
			stateArr[i][w] = READY;
	}
	resultPrint(p, stateArr);

	free(queue);
	free(temp);
}

void SJF(PROCESS* p) {

	int i, j, k, w;
	int count;
	int totalSvt;
	int index;
	int time;
	int** stateArr;

	QUEUE* queue = malloc(sizeof(QUEUE)); // single queue
	PROCESS* temp = (PROCESS*)malloc(sizeof(PROCESS) * pc_num);
	for (i = 0; i < pc_num; i++) temp[i] = p[i];
	totalSvt = getSumSvt(temp);

	stateArr = (int**)malloc(sizeof(int*) * pc_num);
	for (i = 0; i < totalSvt; i++) {
		stateArr[i] = (int*)malloc(sizeof(int) * totalSvt);
	}

	initQueue(queue);

	count = 1;
	insertQueue(queue, temp[0].pid);
	count += temp[0].svt;

	// sort by service time
	for (i = 1; i < pc_num - 1; i++) {
		for (j = i + 1; j < pc_num; j++) {
			if (temp[i].svt > temp[j].svt) {
				swapProc(&temp[i], &temp[j]);
			}
		}
	}
	for (i = 0; i < pc_num-1; i++) {
		for (index = 1; index < pc_num; index++) {
			if (count > temp[index].art) {
				insertQueue(queue, temp[index].pid);
				count += temp[index].svt;
			}
		}
	}

	// sort by queue
	for (i = 0; i < pc_num-1; i++) {
		if (queue->arr[i] != temp[i].pid){
			for (j = i + 1; j < pc_num; j++) {
				if (queue->arr[i] == temp[j].pid) {
					swapProc(&temp[i], &temp[j]);
				}
			}
		}
	}
	
	time = 1;
	for (i = 0; i < pc_num; i++) {

		int front = getQueue(queue);
		if (front == temp[i].pid) {

			while (time < temp[i].art) time++;
			if (time >= temp[i].art) {
				for (j = 0; j < time - 1; j++) {
					stateArr[i][j] = READY;
				}
			}
			while (temp[i].svt != 0) {
				stateArr[i][time - 1] = RUN;
				temp[i].svt--;
				time++;
			}
		}
		for (w = time - 1; w <= totalSvt; w++)
			stateArr[i][w] = READY;
	}
	// resultPrint(temp, stateArr);
	// 안 되는 원인 파악 X

	i = 0;
	count = 0;
	while (count < pc_num) {
		if (temp[i].pid == count) {
			printf("%c ", temp[i].name);
			for (j = 0; j <= totalSvt; j++) {
				if (stateArr[i][j] == 0) printf("□ ");
				else printf("■ ");
			}
			printf("\n");
			i = 0;
			count++;
		}
		else i++;
	}
	free(queue);
	free(temp);
}

void RR(PROCESS* p) {
	int i, j;
	int count;
	int totalSvt;
	int time;
	int front;
	int** stateArr;
	

	QUEUE* queue = malloc(sizeof(QUEUE)); // single queue
	PROCESS* temp = (PROCESS*)malloc(sizeof(PROCESS) * pc_num);

	for (i = 0; i < pc_num; i++) temp[i] = p[i];
	totalSvt = getSumSvt(temp);

	stateArr = (int**)malloc(sizeof(int*) * pc_num);
	for (i = 0; i < totalSvt; i++) {
		stateArr[i] = (int*)malloc(sizeof(int) * totalSvt);
	}

	initQueue(queue);

	for (i = 0; i < pc_num; i++) {
		for (j = 0; j < totalSvt+1; j++) {
			stateArr[i][j] = READY;
		}
	}
	front = -1;
	for (time = 1; time <= totalSvt; time++) {
		for (i = front + 1; i <= front + pc_num; i++) {
			if (time > temp[i % pc_num].art && temp[i % pc_num].svt != 0) {
				insertQueue(queue, temp[i % pc_num].pid);
				temp[i % pc_num].svt--;
				if (time % queue->timeQt == 0) {
					for (j = queue->timeQt - 1; j >= 0; j--) {
						front = getQueue(queue);
						stateArr[front][time - 1 - j] = RUN;
						i = front;
					}
				}
				else if (time == totalSvt) {
					int size = queue->rear - queue->front;
					for (j = size; j >= 0; j--) {
						front = getQueue(queue);
						stateArr[front][time - 1 - j] = RUN;
						i = front;
					}
				}
				break;
			}
		}
	}
	resultPrint(p, stateArr);
	free(queue);
	free(temp);
}

