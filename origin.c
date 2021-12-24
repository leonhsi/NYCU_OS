#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define num_thread 2

float time_diff(struct timeval *start, struct timeval *end){
    return (end->tv_sec - start->tv_sec) + 1e-6*(end->tv_usec - start->tv_usec);
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
    
    for(int i=0; i<5; i++){

		printf("%d : computing with data size %d \n",i, s[i]);

		float total_time = 0;

		for(int time=0; time<10; time++){
		    int sum = 0;

		    //create array
		    int *array = (int *)malloc(s[i] * sizeof(int));
		    for(int idx=0; idx<s[i]; idx++){
		        array[idx] = rand()%10;
		    }

		    gettimeofday(&t_start, NULL);

		    for(int j=0; j<s[i]; j++){
				if(array[j] == target)sum++;
		    }

		    printf("Integer %d occurs %d times in the array\n", target, sum);

		    gettimeofday(&t_end, NULL);
		    total_time += time_diff(&t_start, &t_end)*1000;
		}

		printf("average time of data size %d: %0.4f ms \n",s[i], total_time / 10  );
		printf("==\n");
    }
}
