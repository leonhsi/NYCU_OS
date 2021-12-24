#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float time_diff(struct timeval *start, struct timeval *end){
    return (end->tv_sec - start->tv_sec) + 1e-6*(end->tv_usec - start->tv_usec);
}

int main(int argc, char *argv[]){

    if(argc > 2){
		printf("input error!\n");
		exit(1);
    }

    int target = atoi(argv[1]);
    int num_child  = 2;
    int s[5] = {10000,50000, 100000,500000, 1000000};
    srand(time(NULL));

    struct timeval t_start;
    struct timeval t_end;

    float total_time[5];
    for(int i=0; i<5; i++){
		total_time[i] = 0;
    }

    for(int i=0; i<5; i++){
        printf("%d : generating array with size of : %d \n", i , s[i]);

        for(int time = 0; time < 10; time++){
	    	//create array	
	    	int *array = (int *)malloc(s[i] * sizeof(int));
	    	for(int idx=0; idx<s[i]; idx++){
	    	    array[idx] = rand() % 10;
	    	}

	    	int segment = s[i] / num_child;
	    	int pids[num_child];

	    	gettimeofday(&t_start, NULL);
	    	
	    	//fork here
	    	for(int j=0; j<num_child; j++){
	    	    pids[j] = fork();
	    	    if( pids[j] < 0){
	    			perror("fork");
	    			abort();
	    	    }
	    	    else if( pids[j] == 0){
	    			int start = j*segment;
	    			int end = (j+1)*segment;
	    			int sum_tmp = 0;

			    	for(int k=start; k<end; k++){
	    		    	if(array[k] == target) sum_tmp++;
			    	}

	    			exit(sum_tmp);
	    	    }
	    	}
        	int sum = 0;
        	int status;

	    	//wait here
        	for(int j=0; j<num_child; j++){
        	    waitpid(pids[j], &status, 0);
        	    if (WEXITSTATUS(status)){
        	        int returned = WEXITSTATUS(status);
        	        sum += returned;
        	        printf("child with pid %ld exit with status 0x%x, sum = %d \n", (long)pids[j], status, returned);
        	    }
        	}

	    	printf("Integer %d occurs %d times in the array \n", target, sum);
        	gettimeofday(&t_end, NULL);
	    	total_time[i] += time_diff(&t_start, &t_end)*1000;
		}

		printf("average time of size %d: %0.4f ms \n",s[i], total_time[i] / 10);
		printf("==\n");
    }
    //printf("[%0.4f",total_time[0]/10);
    //for(int i=1; i<5; i++){
    //    printf(", %0.4f", total_time[i]/10);
    //}
    //printf(" ]\n");
}
