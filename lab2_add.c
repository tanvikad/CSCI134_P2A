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


int num_threads = 1;
int num_iterations = 1;
int got_yield = 0;
int got_sync = 0;
int got_pthread_mutex = 0;
int got_spin_lock = 0;
int got_compare_swap = 0;
long long counter = 0;

pthread_mutex_t mutex_lock;



void add_value(long long* pointer, long long value) {
    if(got_pthread_mutex) {
        pthread_mutex_lock(&mutex_lock);

    }
    long long sum = *pointer + value;
    if (got_yield) {
        sched_yield();
    }
    *pointer = sum;

    if(got_pthread_mutex){
        pthread_mutex_unlock(&mutex_lock);
    }
}

void* add() {

    for(int i = 0; i < num_iterations; i++) {
        add_value(&counter, 1);
    }

    for(int i = 0; i < num_iterations; i++) {
        add_value(&counter, -1); 
    }
    pthread_exit(NULL);
    return NULL;
}

int main(int argc, char *argv[]) {
    
    int curr_option;
    const struct option options[] = {
        { "threads",  required_argument, NULL,  't' },
        { "iterations", required_argument, NULL,  'i' },
        { "yield", no_argument, NULL, 'y'},
        { "sync", required_argument, NULL, 's'},
        { 0, 0, 0, 0}
    };

    while((curr_option = getopt_long(argc, argv, "i:t:y", options, NULL)) != -1)  {
        switch(curr_option) {
            case 't':
                sscanf(optarg, "%d", &num_threads);
                // num_threads = optarg;
                break;
            case 'i':
                sscanf(optarg, "%d", &num_iterations);
                // num_iterations = optarg;
                break;
            case 'y':
                got_yield = 1; 
                break;
            case 's':
                if(*optarg == 'm') {
                    got_pthread_mutex = 1;
                }else if (*optarg == 's') {
                    got_spin_lock = 1;
                }else if (*optarg == 'c') {
                    got_compare_swap = 1;
                }else {
                    fprintf(stderr, "Did not give a synchronization method \n ");
                    exit(1);
                }
                break;
            default:
                fprintf(stderr, "Use the options --iterations --threads");
                exit(1);
                break;
        }
    }

    printf("The number of threads is %d \n", num_threads);
    printf("The number of interations is %d \n", num_iterations);

    pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);

    if(got_pthread_mutex) {
        int rc = pthread_mutex_init(&mutex_lock, NULL);
        if(rc != 0) {
            fprintf(stderr, "Failed to initialize the pthread mutex \n");
            exit(1);
        }
    }

    struct timespec start, stop;
    
    if( clock_gettime( CLOCK_MONOTONIC, &start) == -1 ) {
        fprintf(stderr, "Clock Get Time Fails %s\n", strerror(errno));
        exit(1);
    }

    for (int i = 0; i < num_threads; i++) {
 
        int rc = pthread_create(&threads[i], NULL, add, NULL);

        if(rc != 0) {
            fprintf(stderr, "Pthread Creation Failed due to %s \n", strerror(errno));
            exit(1);
        }
    }


    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    if( clock_gettime( CLOCK_MONOTONIC, &stop) == -1 ) {
        fprintf(stderr, "Clock Get Time Fails %s\n", strerror(errno));
        exit(1);
    }

    double accum = (stop.tv_sec - start.tv_sec) * 1000000000
          + (stop.tv_nsec - start.tv_nsec);

    int num_operations = 2 * num_threads * num_iterations;
    char* type_of_str = "add-none";
    if(got_yield) {
        type_of_str = "add-yield-none";
    }

    int add_csv_fd = open("lab2_add.csv", O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
    if(add_csv_fd == -1) {
        fprintf(stderr, "Opening the csv file failed %s \n", strerror(errno));
        exit(1);
    }

    char add_csv_buff[200];
    printf("the pointer value is %lld", counter);
    snprintf(add_csv_buff, 200, "%s, %d, %d, %f, %lld\n", type_of_str, num_iterations, num_operations, accum/num_operations, counter);
    write(add_csv_fd, add_csv_buff, strlen(add_csv_buff));

    free(threads);
}  