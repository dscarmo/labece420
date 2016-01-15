#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "lab1_IO.h"
#include "timer.h"

int num_thread;
int **A; int **B; int **C; int n;
void *calculate(void* rank);

int main (int argc, char* argv[])
{
	
	double t_start; double t_end; double time = 0;
	if(argv[1] == NULL){
		printf("Number of threads not specified.\n");
		exit(1);
	}

	num_thread = atoi(argv[1]);
	GET_TIME(t_start);
	Lab1_loadinput(&A, &B, &n);
	pthread_t* thread_handles = malloc (num_thread * sizeof(pthread_t));

	long i;

	C = malloc(n * sizeof(int*));
	for (i = 0; i < n; i++){
      		C[i] = malloc(n * sizeof(int));
	}

	if((n*n)%num_thread != 0){
		printf("Invalid input\n");
		exit(1);
	} 
	if(floor(sqrt(num_thread)) - sqrt(num_thread) != 0){
		printf("Invalid input\n");
		exit(1);
	}
	for(i = 0; i < num_thread; i++){
		pthread_create(&thread_handles[i], NULL, calculate, (void*) i);  
	}
	long thread;
	for (thread = 0; thread < num_thread; thread++) 
      		pthread_join(thread_handles[thread], NULL); 

	Lab1_saveoutput(C, &n, time);
	GET_TIME(t_end);
	printf("Time elapsed: %f\n", t_end - t_start);
   	free(thread_handles);
	free(C);
	return 0;
}

void *calculate(void* rank) {
	long my_rank = (long) rank; 
	int x = floor(my_rank / sqrt(num_thread)); 
	int y = my_rank % (long)sqrt(num_thread);
	int i; int j;
	for(i = (n/sqrt(num_thread)*x); i<= (n/sqrt(num_thread)*(x+1)-1); i++){
		for(j = (n/sqrt(num_thread)*y); j<= (n/sqrt(num_thread)*(y+1)-1); j++){
			C[i][j] = 0;
			int k;
			//printf("%d %d rank: %ld\n", i, j, my_rank);
			//fflush(stdout);
            		for (k = 0; k < n; k++)
				C[i][j] += A[i][k] * B[k][j];
		}
	}

	return NULL;
}
