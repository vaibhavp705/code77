
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define N 5
sem_t forks[N];
sem_t room;
void *philosopher(void *num)
{
int id = *(int *)num;
while (1)
{
printf("Philosopher %d is THINKING\n", id);
sleep(1);
// Enter room
sem_wait(&room);
// Pick up left fork
sem_wait(&forks[id]);
printf("Philosopher %d picked LEFT fork %d\n", id, id);
// Pick up right fork
sem_wait(&forks[(id + 1) % N]);
printf("Philosopher %d picked RIGHT fork %d\n",
id, (id + 1) % N);
printf("Philosopher %d is EATING\n", id);
sleep(2);
// Put down forks
sem_post(&forks[id]);
sem_post(&forks[(id + 1) % N]);
printf("Philosopher %d finished EATING\n", id);
// Leave room
sem_post(&room);
sleep(1);
}
return NULL;
}
int main()
{
pthread_t thread[N];
int id[N];
sem_init(&room, 0, N - 1);
for (int i = 0; i < N; i++)
sem_init(&forks[i], 0, 1);
for (int i = 0; i < N; i++)
{
id[i] = i;
pthread_create(&thread[i], NULL, philosopher, &id[i]);
}
for (int i = 0; i < N; i++)
pthread_join(thread[i], NULL);
return 0;
}
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define N 5 // Number of philosophers
sem_t chopsticks[N];
void* philosopher(void* num) {
int id = *(int*)num;
// Run for 3 cycles to demonstrate deadlock-free behavior
for (int i = 0; i < 3; i++) {
printf("Philosopher %d is thinking.\n", id);
sleep(1); // Simulating thinking
if (id % 2 == 0) {
// Even philosopher: Left chopstick then Right chopstick
sem_wait(&chopsticks[id]);
sem_wait(&chopsticks[(id + 1) % N]);
} else {
// Odd philosopher: Right chopstick then Left chopstick
sem_wait(&chopsticks[(id + 1) % N]);
sem_wait(&chopsticks[id]);
}
// Critical Section (Eating)
printf("Philosopher %d is EATING.\n", id);
sleep(2); // Simulating eating
// Release chopsticks
sem_post(&chopsticks[id]);
sem_post(&chopsticks[(id + 1) % N]);
printf("Philosopher %d finished eating and put down chopsticks.\n", id);
}
return NULL;
}
int main() {
pthread_t threads[N];
int phil_ids[N];
// Initialize semaphores to 1 (available)
for (int i = 0; i < N; i++) {
sem_init(&chopsticks[i], 0, 1);
}
// Create 5 philosopher threads
for (int i = 0; i < N; i++) {
phil_ids[i] = i;
pthread_create(&threads[i], NULL, philosopher, &phil_ids[i]);
}
// Wait for all threads to finish execution
for (int i = 0; i < N; i++) {
pthread_join(threads[i], NULL);
}
// Clean up semaphores
for (int i = 0; i < N; i++) {
sem_destroy(&chopsticks[i]);
}
printf("\nSimulation completed successfully without deadlock!\n");
return 0;
}
