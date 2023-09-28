#include <pthread.h>
#include <stdio.h> //for printing
#include <stdlib.h> 
#include <time.h>


#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h> 
#include <errno.h>
#include <string.h>
#include <poll.h>
#include <termios.h>
#include <sys/wait.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include <netdb.h> 



typedef struct  {
    long long *pointer;
    long long iterations;
    int tid; 
} addArguments;

void add_value(long long* pointer, long long value) {
    long long sum = *pointer + value;
    *pointer = sum;
}

void* add(void* args) {

    addArguments* args_input = (addArguments* ) args;

    for(int i = 0; i < args_input->iterations; i++) {
        add_value(args_input->pointer, 1);
    }

    for(int i = 0; i < args_input->iterations; i++) {
        add_value(args_input->pointer, -1); 
    }
    pthread_exit(NULL);
    return NULL;
}

int main(int argc, char *argv[]) {

    int curr_option;
    const struct option options[] = {
        { "threads",  required_argument, NULL,  't' },
        { "iterations", required_argument, NULL,  'i' },
        { 0, 0, 0, 0}
    };

    int num_threads = 1;
    int num_iterations = 1;

    while((curr_option = getopt_long(argc, argv, "i:t", options, NULL)) != -1)  {
        switch(curr_option) {
            case 't':
                sscanf(optarg, "%d", &num_threads);
                // num_threads = optarg;
                break;
            case 'i':
                sscanf(optarg, "%d", &num_iterations);
                // num_iterations = optarg;
                break;
            default:
                fprintf(stderr, "Use the options --iterations --threads");
                exit(1);
                break;
        }
    }

    printf("The number of threads is %d \n", num_threads);
    printf("The number of interations is %d \n", num_iterations);

    // num_threads = 10;
    pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);
    // struct addArguments a; 
    addArguments *args = malloc(sizeof(addArguments) * num_threads);
    // struct addArguments args[10];
    long long pointer_value = 0;

    struct timespec start, stop;
    
    if( clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
        fprintf(stderr, "Clock Get Time Fails %s\n", strerror(errno));
        exit(1);
    }

    for (int i = 0; i < num_threads; i++) {
        args[i].pointer = &pointer_value;
        args[i].tid = i;
        args[i].iterations = num_iterations;
        int rc = pthread_create(&threads[i], NULL, add, (void*)(&args[i]));

        if(rc != 0) {
            fprintf(stderr, "Pthread Creation Failed due to %s \n", strerror(errno));
            exit(1);
        }
    }


    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) {
        fprintf(stderr, "Clock Get Time Fails %s\n", strerror(errno));
        exit(1);
    }

    double accum = (stop.tv_sec - start.tv_sec) * 1000000000
          + (stop.tv_nsec - start.tv_nsec);

    int num_operations = 2 * num_threads * num_iterations;
    printf("add-none, %d, %d, %d, %f, %lld\n", num_threads, num_iterations, num_operations, accum/num_operations, pointer_value);

    free(threads);
    free(args);
}  