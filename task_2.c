#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 10
long total_sum = 0;
pthread_mutex_t sum_mutex = PTHREAD_MUTEX_INITIALIZER;

void *sum_range() {
    for (int i = 0; i < 10000000; i++) {
        pthread_mutex_lock(&sum_mutex);
        total_sum++;
        pthread_mutex_unlock(&sum_mutex);
    }
    
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int ch; 
    for (int i = 0; i < NUM_THREADS; i++) {
        ch = pthread_create(&threads[i], NULL, sum_range, NULL);
        if (ch) {
            printf("ERROR; return code from pthread_create() is %d\n", ch);
            exit(-1);
        }
    }

    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    printf("Total sum = %ld\n", total_sum);

    exit(EXIT_SUCCESS);
}
