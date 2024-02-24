#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5

void *print_thread_index(void *args) {
    int *tid = (int *)args;
    printf("Thread id: %d\n", *tid);
    pthread_exit(NULL);
}

int main (void) {
    pthread_t threads[NUM_THREADS];
    int ch;
    int param_vals[NUM_THREADS];

    for(int i=0; i < NUM_THREADS; i++){
        param_vals[i] = i;
        ch = pthread_create(&threads[i], NULL, print_thread_index, (void *)&param_vals[i]);
        if (ch){
            printf("ERROR; return code from pthread_create() is: %d\n", ch);
            exit(EXIT_FAILURE);
        }
    }

    // Ожидаем завершения потоков
    for(int t=0; t<NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    exit(EXIT_SUCCESS);
}
