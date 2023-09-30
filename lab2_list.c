#include <pthread.h>
#include <stdio.h> //for printing
#include <stdlib.h> 
#include <time.h>
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
#include <stdlib.h>


#include "SortedList.h"

int num_threads = 1;
int num_iterations = 1;
int opt_yield = 0;
int got_sync = 0;
int got_pthread_mutex = 0;
int got_spin_lock = 0;
int got_compare_swap = 0;

SortedList_t list;
SortedListElement_t* elements; 


void* thread_action(void* i) {
    long tid = (long)i;
    long start = tid*num_iterations;
    printf("The tid is %ld \n", tid);

    for(int i = start; i < start+num_iterations; i++){
        SortedList_insert(&list, &elements[i]);
    }

    pthread_exit(NULL);
    return NULL;
}


char* get_rand_string() {
    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";        
    int length = 1;
    char* random_string = malloc(sizeof(char) * (length +1));

    if(random_string) {
        for (int i = 0; i < length; i++) {

            int key = rand() % (int)(sizeof(charset) -1) + i;
            random_string[i] = charset[key];
        }

        random_string[length] = '\0';

    }else {
        fprintf(stderr, "Unable to malloc a random string \n");
        exit(1);
    }

    return random_string;
}

int main(int argc, char *argv[]) {
    srand(time(0));


    int curr_option;
    const struct option options[] = {
        { "threads",  required_argument, NULL,  't' },
        { "iterations", required_argument, NULL,  'i' },
        { "yield", required_argument, NULL, 'y'},
        { "sync", required_argument, NULL, 's'},
        { 0, 0, 0, 0}
    };

    char* yield_argument = NULL; 
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
                yield_argument = optarg;  
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

    if(yield_argument) {
        for(int i = 0; i < (int)(strlen(yield_argument)); i++){
            if(yield_argument[i] == 'i')  opt_yield |= INSERT_YIELD;
            else if(yield_argument[i] == 'd') opt_yield |= DELETE_YIELD;
            else if (yield_argument[i] == 'l')  opt_yield |= LOOKUP_YIELD;
            else {
                fprintf(stderr, "Gave an invalid yield argument \n");
                exit(1);
            }
        }
    }

    //iinitalized list;
    list.prev = NULL;
    list.next = NULL;
    list.key = NULL;

    elements = malloc(sizeof(SortedListElement_t) * num_threads * num_iterations);
    pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);
    // int* integers = malloc(sizeof(int) * num_threads);
    

    //initalize the elements
    for (int i = 0; i < (num_threads*num_iterations); i++) {
        elements[i].key = get_rand_string();
        elements[i].prev = NULL;
        elements[i].next = NULL;
    }

    // for(int i = 0; i < (num_threads* num_iterations); i++) {
    //     SortedList_insert(&list, &elements[i]);
    // }

    long t; 
    for (t = 0; t < num_threads; ++t) {
        // integers[t] = t;
        int rc = pthread_create(&threads[t], NULL, thread_action, (void*)t);
        if(rc != 0) {
            fprintf(stderr, "Failed to initialize the pthread \n");
            exit(1);
        }
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    SortedListElement_t* curr = list.next;
    while(1) {
        if(curr == NULL) break;
        if(curr->key != NULL) {
            printf("Key is %s, previous is %s, next is %s \n", curr->key, curr->prev->key, curr->next->key);
        }else {
            break;
        }

        curr = curr->next;
    }

    free(elements);
    free(threads);
    // free(integers);
}