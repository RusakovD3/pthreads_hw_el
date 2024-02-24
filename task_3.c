#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define NUM_SHOPS 5
#define NUM_CUSTOMERS 3
#define RESTOCK_AMOUNT 5000
#define MIN_STOCK 9000
#define MAX_STOCK 12000
#define MIN_DEMAND 80000
#define MAX_DEMAND 100000

int shops[NUM_SHOPS];
pthread_mutex_t shop_mutexes[NUM_SHOPS];

void *customer(void *arg) {
    int demand = MIN_DEMAND + rand() % (MAX_DEMAND - MIN_DEMAND + 1);
    printf("Покупатель %ld с потребностью %d\n", (long)arg, demand);

    while (demand > 0) {
        for (int i = 0; i < NUM_SHOPS; i++) {
            pthread_mutex_lock(&shop_mutexes[i]);
            if (shops[i] > 0) {
                int taken = (demand >= shops[i]) ? shops[i] : demand;
                shops[i] -= taken;
                demand -= taken;
                printf("Покупатель %ld забрал %d из магазина %d, осталось потребность %d\n", (long)arg, taken, i, demand);
                pthread_mutex_unlock(&shop_mutexes[i]);
                sleep(2);
                break;
            }
            pthread_mutex_unlock(&shop_mutexes[i]);
        }
    }
    pthread_exit(NULL);
}

void *loader() {
    while (1) {
        int restocked = 0;
        for (int i = 0; i < NUM_SHOPS && !restocked; i++) {
            pthread_mutex_lock(&shop_mutexes[i]);
            if (shops[i] + RESTOCK_AMOUNT <= MAX_STOCK) {
                shops[i] += RESTOCK_AMOUNT;
                printf("Погрузчик добавил %d товаров в магазин %d\n", RESTOCK_AMOUNT, i);
                restocked = 1;
            }
            pthread_mutex_unlock(&shop_mutexes[i]);
        }
        if (!restocked) {
            printf("Погрузчик не нашел магазин для пополнения\n");
        }
        sleep(1);
    }
}

int main() {
    srand(time(NULL));
    pthread_t customers[NUM_CUSTOMERS];
    pthread_t loader_thread;

    for (int i = 0; i < NUM_SHOPS; i++) {
        shops[i] = MIN_STOCK + rand() % (MAX_STOCK - MIN_STOCK + 1);
        pthread_mutex_init(&shop_mutexes[i], NULL);
    }

    for (long i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_create(&customers[i], NULL, customer, (void *)i);
    }
    pthread_create(&loader_thread, NULL, loader, NULL);

    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_join(customers[i], NULL);
    }

    // Завершаем поток погрузчика после выполнения всех потребностей покупателей
    pthread_cancel(loader_thread);

    for (int i = 0; i < NUM_SHOPS; i++) {
        pthread_mutex_destroy(&shop_mutexes[i]);
    }

    exit(EXIT_SUCCESS);
}
