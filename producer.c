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

void* producer_thread(void* arg) {
    srand(time(NULL) ^ pthread_self());

    while (1) {
        sem_wait(sem_empty);     
        sem_wait(sem_mutex);     

        shared_buffer[0]++;
        printf("Producer: Created Product\t|\t Product On Table: %d\n", shared_buffer[0]);

        sem_post(sem_mutex);
        sem_post(sem_full);      

        // makes it in random intervals so that there is a way for producer to create more than one before consumer took one
        double interval = 0.5 + ((double)rand() / RAND_MAX) * 3;
        usleep(interval * MICROSECOND);
    }

    return NULL;
}

void setup_shared_resources() {
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        exit(1);
    }

    if (ftruncate(shm_fd, sizeof(int)) == -1) {
        perror("ftruncate failed");
        exit(1);
    }

    shared_buffer = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_buffer == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }

    shared_buffer[0] = 0;  // initialize product count

    sem_empty = sem_open("/empty", O_CREAT, 0666, MAX_ITEMS);
    sem_full  = sem_open("/full",  O_CREAT, 0666, 0);
    sem_mutex = sem_open("/mutex", O_CREAT, 0666, 1);
}

int main() {
    setup_shared_resources();

    pthread_t tid;
    pthread_create(&tid, NULL, producer_thread, NULL);
    pthread_join(tid, NULL);

    return 0;
}
