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


char* get_rand_string() {
    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";        
    int length = 2;
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
                opt_yield = 1; 
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

    //iinitalized list;
    list.prev = NULL;
    list.next = NULL;
    list.key = NULL;

    elements = malloc(sizeof(SortedListElement_t) * num_threads * num_iterations);
    
    //initalize the elements
    for (int i = 0; i < (num_threads*num_iterations); i++) {
        elements[i].key = get_rand_string();
        elements[i].prev = NULL;
        elements[i].next = NULL;
    }

    for(int i = 0; i < (num_threads* num_iterations); i++) {
        SortedList_insert(&list, &elements[i]);
    }
    

    // SortedListElement_t elem1;
    // elem1.key = "a";
    // SortedList_insert(&list, &elem1);

    // SortedListElement_t elem2;
    // elem2.key = "c";
    // SortedList_insert(&list, &elem2);

    // SortedListElement_t elem3;
    // elem3.key = "b";
    // SortedList_insert(&list, &elem3);

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
}