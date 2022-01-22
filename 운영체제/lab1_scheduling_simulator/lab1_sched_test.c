/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32191197	 
*	    Student name : Kim Chae Eun 
*
*
*/

#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "lab1_sched_types.h"


int main(int argc, char* argv[]) {

	printf("How many process: ");
	scanf("%d", &pc_num);

	PROCESS* proc = (PROCESS*)malloc(sizeof(PROCESS)*pc_num);

	setWorkload(proc);

	printf("[ FIFO ] ");
	FIFO(proc);

	printf("[ SJF ] ");
	SJF(proc);

	printf("[ RR ] ");
	RR(proc);

	printf("[ RR ] ");
	RR(proc);

	free(proc);

	return 0;
}

