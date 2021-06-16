/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32191197 
*	    Student name : Kim Chae Eun 
*
*   lab1_sched_types.h :
*       - lab1 header file.
*       - must contains scueduler algorithm function's declations.
*
*/

#pragma once

#ifndef _LAB1_HEADER_H
#define _LAB1_HEADER_H

#define RUN 1 
#define READY 0
#define QSIZE 20

int pc_num;

typedef struct Process {

	char name;
	int pid;
	int state;
	int art; // Arrival time
	int svt; // Service time
	int rt; // Run time
}PROCESS;

typedef struct Queue {
	int front;
	int rear;
	int timeQt; // Time Quantum
	int arr[QSIZE];
}QUEUE;

// Queue

void initQueue(QUEUE* q);
void emptyQueue(QUEUE* q);
void insertQueue(QUEUE* q, int val);
int getQueue(QUEUE* q);

// Workload table
void setWorkload(PROCESS* p);
void sortWorkload(PROCESS* p);


void swapProc(PROCESS* a, PROCESS* b);

int getSumSvt(PROCESS* p);

void resultPrint(PROCESS* p, int** arr);

void FIFO(PROCESS* p);
void SJF(PROCESS* p);
void RR(PROCESS* p);

#endif /* LAB1_HEADER_H*/ 


