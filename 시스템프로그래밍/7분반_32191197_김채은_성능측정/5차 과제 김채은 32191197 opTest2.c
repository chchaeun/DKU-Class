/* optimization test2 by. chae-eun. 2020-12-05 */

#include<stdio.h>
#include<unistd.h>
#include<sys/time.h>

#define SWAP(a, b, temp) {temp=a; a=b; b=temp;}
#define MAX 999999

void quickSort1(int* a, int left, int right);
void quickSort2(int* a, const int left, const int right);
void quickSort3(int* a, const int left, const int right);
void shuffle(int *a, int n);
void printArr(int* a, const int n);

int main(int argc, char *argv[]) {
		
	int len=0;

	if(argc==2){
		len = atoi(argv[1]);
        	int arr1[len];
	        int arr2[len];
		int arr3[len];
	        int arr4[len];
		int arr5[len+1];
	        int i;
	        struct timeval stime, etime, gap;
	
		for(i=0; i<len+1; i++)
			arr1[i] = i+1;

        	shuffle(arr1, len);             // 섞어줌

      		for(i=0; i<len+1; i++){
	                arr2[i] = arr1[i];
			arr3[i] = arr1[i];
			arr4[i] = arr1[i];
			arr5[i] = arr1[i];
		}
		arr5[len] = MAX;
		/*printArr(arr1, len);
		printArr(arr2, len);
		printArr(arr3, len);
		printArr(arr4, len);
                printArr(arr5, len+1);
		*/
                // 제대로 복사됐고, MAX도 제대로 넣어졌는지 확인

                gettimeofday(&stime, NULL);
                quickSort1(arr1, 0, (sizeof(arr1)/sizeof(int))-1);
                gettimeofday(&etime, NULL);

                gap.tv_sec = etime.tv_sec - stime.tv_sec;
                gap.tv_usec = etime.tv_usec - stime.tv_usec;
                if (gap.tv_usec < 0) {
                        gap.tv_sec = gap.tv_sec-1;
                        gap.tv_usec = gap.tv_usec + 1000000;
                }
                printf("sort1: %ldsec :%ldusec\n", gap.tv_sec, gap.tv_usec);

                gettimeofday(&stime, NULL);
                quickSort2(arr2, 0, (sizeof(arr1)/sizeof(int))-1);
                gettimeofday(&etime, NULL);

                gap.tv_sec = etime.tv_sec - stime.tv_sec;
                gap.tv_usec = etime.tv_usec - stime.tv_usec;
                if (gap.tv_usec < 0) {
                        gap.tv_sec = gap.tv_sec-1;
                        gap.tv_usec = gap.tv_usec + 1000000;
                }
                printf("sort2: %ldsec :%ldusec\n", gap.tv_sec, gap.tv_usec);



                gettimeofday(&stime, NULL);
                quickSort1(arr3, 0, len-1);
                gettimeofday(&etime, NULL);

                gap.tv_sec = etime.tv_sec - stime.tv_sec;
                gap.tv_usec = etime.tv_usec - stime.tv_usec;
                if (gap.tv_usec < 0) {
                        gap.tv_sec = gap.tv_sec-1;
                        gap.tv_usec = gap.tv_usec + 1000000;
                }
                printf("sort3: %ldsec :%ldusec\n", gap.tv_sec, gap.tv_usec);
 		


		gettimeofday(&stime, NULL);
                quickSort2(arr4, 0, len-1);
                gettimeofday(&etime, NULL);

                gap.tv_sec = etime.tv_sec - stime.tv_sec;
                gap.tv_usec = etime.tv_usec - stime.tv_usec;
                if (gap.tv_usec < 0) {
                        gap.tv_sec = gap.tv_sec-1;
                        gap.tv_usec = gap.tv_usec + 1000000;
                }
                printf("sort4: %ldsec :%ldusec\n", gap.tv_sec, gap.tv_usec);


                gettimeofday(&stime, NULL);
                quickSort3(arr5, 0, len);
                gettimeofday(&etime, NULL);

                gap.tv_sec = etime.tv_sec - stime.tv_sec;
                gap.tv_usec = etime.tv_usec - stime.tv_usec;
                if (gap.tv_usec < 0) {
                        gap.tv_sec = gap.tv_sec-1;
                        gap.tv_usec = gap.tv_usec + 1000000;
                }
                printf("sort5: %ldsec :%ldusec\n", gap.tv_sec, gap.tv_usec);



	}
          
        return 0;
}
void shuffle(int *a, int n){ // 배열 랜덤하게 섞어준다

        srand(time(NULL));
        int t, i;
        int rn;

        for(i=0; i<n-1; i++){
                rn = rand()%(n-i)+i;
                SWAP(a[i], a[rn], t);
        }
}

void quickSort1(int* a, int left, int right) {

    int t;

    if (left < right) {
        int i = left, j = right+1, pivot = a[left];
        do {
            do { i++; } while (a[i] < pivot && i <= right);
            do { j--; } while (a[j] > pivot);
            if (i < j) SWAP(a[i], a[j], t);
        } while (i < j);
        SWAP(a[left], a[j], t);
        quickSort1(a, left, j - 1);
        quickSort1(a, j + 1, right);
    }
}
void quickSort2(int* a, const int left, const int right) {

    int t;

    if (left < right) {
        int i = left, j = right + 1, pivot = a[left];
        do {
                do { i++; } while (a[i] < pivot && i<=right);
                do { j--; } while (a[j] > pivot);
                if (i < j) SWAP(a[i], a[j], t);
        } while (i < j);
        SWAP(a[left], a[j], t);
        quickSort2(a, left, j - 1);
        quickSort2(a, j + 1, right);
    }
}


void quickSort3(int* a, const int left, const int right) {

    int t;

    if (left < right) {
        int i = left, j = right + 1, pivot = a[left];
        do {
                do { i++; } while (a[i] < pivot);
                // 배열 마지막에 99,999가 있기 때문에 i가 right 밖으로 넘어가지 않음
                do { j--; } while (a[j] > pivot);
                if (i < j) SWAP(a[i], a[j], t);
        } while (i < j);
        SWAP(a[left], a[j], t);
        quickSort2(a, left, j - 1);
        quickSort2(a, j + 1, right);
    }
}


void printArr(int* a, const int n) {    // 배열 출력
        int i;
        for (i = 0; i < n; i++){
                        printf("%d ", a[i]);
        }
        printf("\n");
}


