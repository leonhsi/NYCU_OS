#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define num_thread 2

pthread_mutex_t lock;
int sum;

float time_diff(struct timeval *start, struct timeval *end){
    return (end->tv_sec - start->tv_sec) + 1e-6*(end->tv_usec - start->tv_usec);
}

typedef struct thread_data{
    int target;
    int size;
    int index;
    int *arr;
}thread_data;

void *count(void *arg){
    
    thread_data *tdata = (thread_data *)arg;

    int segment = tdata->size / num_thread;
    int start = tdata->index * segment;
    int end = (tdata->index +1) * segment; 
    
    int sum_local = 0; //to verify


    pthread_mutex_lock(&lock);
    for(int i=start; i<end; i++){
		if(tdata->arr[i] == tdata->target){
	    	sum++;
	    	sum_local++;
		}
    }
    printf("thread %d finished computing, sum = %d\n", tdata->index, sum_local);
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}


int main(int argc, char *argv[]){

    if(argc != 2){
		printf("input error ! \n");
		exit(0);
    }

    int s[5] = {10000,50000, 100000, 500000, 1000000};
    int target = atoi(argv[1]);
    srand(time(NULL));

    struct timeval t_start;
    struct timeval t_end;

    float total_time[5];
    for(int i=0; i<5; i++)
	    total_time[i]=0;
    
    for(int i=0; i<5; i++){

		printf("generating multiple threading with data size %d \n", s[i]);

		for(int time=0; time<10; time++){
		    //create array
		    int *array = (int *)malloc(s[i] * sizeof(int));
		    for(int idx=0; idx<s[i]; idx++){
		        array[idx] = rand()%10;
		    }

		    //creat threads and tdata
		    pthread_t *thread_handles;
		    thread_handles = malloc(num_thread * sizeof(pthread_t));
		    thread_data tdata[num_thread];
		    
		    //init mutex
		    if (pthread_mutex_init(&lock, NULL) != 0) {
    	 	    printf("\n mutex init has failed\n");
    	        return 1;
    	    }
		    
		    //init global sum
		    sum = 0;
		    gettimeofday(&t_start, NULL);

		    for(int j=0; j<num_thread; j++){
		        tdata[j].target = target;
		        tdata[j].arr = array;
		        tdata[j].size = s[i];
		        tdata[j].index = j;

		        pthread_create(&thread_handles[j], NULL, count, (void *)&tdata[j]);
		    }

		    for(int j=0; j<num_thread; j++){
		        pthread_join(thread_handles[j], NULL);
		    }

		    pthread_mutex_destroy(&lock);

		    free(thread_handles);

		    printf("Integer %d occurs %d times in the array\n", target, sum);
		    gettimeofday(&t_end, NULL);
		    total_time[i] += time_diff(&t_start, &t_end)*1000;
		}
    	printf("average time of data size %d : %0.4f ms \n",s[i], total_time[i]/10 );
		printf("==\n");
    }
    //printf("[%0.4f",total_time[0]/10);
    //for(int i=1; i<5; i++){
    //    printf(", %0.4f", total_time[i]/10);
    //}
    //printf(" ]\n");
}
