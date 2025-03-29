#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>

#define SHM_NAME "/shared_mem"
#define MAX_ITEMS 2
#define MICROSECOND 1000000

sem_t *sem_empty;
sem_t *sem_full;
sem_t *sem_mutex;

int *shared_buffer;

void* consumer_thread(void* arg) {
    srand(time(NULL) ^ pthread_self());

    while (1) {
        sem_wait(sem_full);      
        sem_wait(sem_mutex);     

        shared_buffer[0]--;
        printf("Consumer: Consumed Product\t|\t Product On Table: %d\n", shared_buffer[0]);

        sem_post(sem_mutex);     
        sem_post(sem_empty);     

        //  Makes it so that there is a chance for the consumer to eat more than a producer made, and vice versa
        double interval = 0.5 + ((double)rand() / RAND_MAX) * 3;
        usleep(interval * MICROSECOND);
    }

    return NULL;
}

void setup_shared_resources() {
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        exit(1);
    }

    shared_buffer = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_buffer == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }

    sem_empty = sem_open("/empty", 0);
    sem_full  = sem_open("/full",  0);
    sem_mutex = sem_open("/mutex", 0);
}

int main() {
    setup_shared_resources();

    pthread_t tid;
    pthread_create(&tid, NULL, consumer_thread, NULL);
    pthread_join(tid, NULL);

    return 0;
}
