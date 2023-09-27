#include <pthread.h>
#include <stdio.h> //for printing

// void add(long long *pointer, long long value) {
//     long long sum = *pointer + value;
//     *pointer = sum;
// }


struct addArguments {
    long long *pointer;
    long long value;
    int tid; 
};

void add(struct addArguments* args) {
    long long sum = *(args->pointer) + 1;
    *args->pointer = sum;
    printf("The thread is %d \n", args->tid);
}

int main() {
    int num_threads = 10;
    pthread_t threads[num_threads];
    struct addArguments args[num_threads];
    long long pointer_value = 0;

    for (int i = 0; i < num_threads; i++) {
        args[i].pointer = &pointer_value;
        args[i].tid = i;
        int rc = pthread_create(&threads[i], NULL, &add, &args[i]);
    }

    printf("The pointer_value is %lld", pointer_value);
}