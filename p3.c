
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#define BUFFER_SIZE 3 // Small buffer size to clearly see synchronization bounds
#define TOTAL_ITEMS 5 // Total items each thread will process
int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;
pthread_mutex_t mutex;
sem_t empty;
sem_t full;
// Producer thread function
void* producer(void* arg) {
int thread_id = *(int*)arg;
for (int i = 0; i < TOTAL_ITEMS; i++) {
int item = rand() % 100 + 1; // Generate random item (1-100)
sem_wait(&empty); // Wait for an empty slot
pthread_mutex_lock(&mutex); // Enter critical section
// --- Critical Section ---
buffer[in] = item;
printf("[PRODUCER %d] Inserted Item %d at Slot %d\n", thread_id, item, in);
in = (in + 1) % BUFFER_SIZE;
// ------------------------
pthread_mutex_unlock(&mutex); // Leave critical section
sem_post(&full); // Signal that buffer has data
sleep(1); // Fast producer
}
pthread_exit(NULL);
}
// Consumer thread function
void* consumer(void* arg) {
int thread_id = *(int*)arg;
for (int i = 0; i < TOTAL_ITEMS; i++) {
sem_wait(&full); // Wait for a full slot
pthread_mutex_lock(&mutex); // Enter critical section
// --- Critical Section ---
int item = buffer[out];
printf("[CONSUMER %d] Extracted Item %d from Slot %d\n", thread_id, item, out);
out = (out + 1) % BUFFER_SIZE;
// ------------------------
pthread_mutex_unlock(&mutex); // Leave critical section
sem_post(&empty); // Signal that buffer has space
sleep(2); // Slow consumer forces buffer to fill up
}
pthread_exit(NULL);
}
int main() {
pthread_t prod_thread, cons_thread;
int p_id = 1, c_id = 1;
srand(time(NULL));
pthread_mutex_init(&mutex, NULL);
sem_init(&empty, 0, BUFFER_SIZE); // 3 empty slots initially
sem_init(&full, 0, 0); // 0 full slots initially
pthread_create(&prod_thread, NULL, producer, &p_id);
pthread_create(&cons_thread, NULL, consumer, &c_id);
pthread_join(prod_thread, NULL);
pthread_join(cons_thread, NULL);
pthread_mutex_destroy(&mutex);
sem_destroy(&empty);
sem_destroy(&full);
printf("Execution completed successfully.\n");
return 0;
}
