
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX 100
// Bubble Sort Algorithm (Used by Parent Process)
void bubble_sort(int arr[], int n) {
int i, j, temp;
for (i = 0; i < n - 1; i++) {
for (j = 0; j < n - i - 1; j++) {
if (arr[j] > arr[j + 1]) {
temp = arr[j];
arr[j] = arr[j + 1];
arr[j + 1] = temp;
}
}
}
}
// Selection Sort Algorithm (Used by Child Process)
void selection_sort(int arr[], int n) {
int i, j, min_idx, temp;
for (i = 0; i < n - 1; i++) {
min_idx = i;
for (j = i + 1; j < n; j++) {
if (arr[j] < arr[min_idx])
min_idx = j;
}
temp = arr[min_idx];
arr[min_idx] = arr[i];
arr[i] = temp;
}
}
// Function to print array elements
void print_array(int arr[], int n) {
for (int i = 0; i < n; i++) {
printf("%d ", arr[i]);
}
printf("\n");
}
int main() {
int arr[MAX], child_arr[MAX], parent_arr[MAX];
int n, choice;
printf("Enter the number of integers to sort: ");
if (scanf("%d", &n) != 1 || n <= 0) {
printf("Invalid array size.\n");
return 1;
}
printf("Enter %d integers: ", n);
for (int i = 0; i < n; i++) {
scanf("%d", &arr[i]);
child_arr[i] = arr[i]; // Copy for child
parent_arr[i] = arr[i]; // Copy for parent
}
printf("\n--- Choose Execution Mode ---\n");
printf("1. Normal Execution (Parent waits for Child using
wait())\n");
printf("2. Demonstrate Zombie State\n");
printf("3. Demonstrate Orphan State\n");
printf("Enter choice (1-3): ");
scanf("%d", &choice);
pid_t pid = fork(); // Create child process
if (pid < 0) {
perror("Fork failed");
exit(1);
}
else if (pid == 0) {
// --- CHILD PROCESS ---
printf("\n[Child] Child Process Started. PID: %d, Parent PID:
%d\n", getpid(), getppid());
if (choice == 3) {
// Orphan state demo: Child sleeps while parent terminates
immediately
printf("[Child] Sleeping for 5 seconds to become an
ORPHAN...\n");
sleep(5);
printf("\n[Child] Awake! My new Parent PID (Adopted by
systemd/init): %d\n", getppid());
}
printf("[Child] Sorting array using Selection Sort...\n");
selection_sort(child_arr, n);
printf("[Child] Sorted array: ");
print_array(child_arr, n);
printf("[Child] Child Process Exiting.\n");
exit(0);
}
else {
// --- PARENT PROCESS ---
printf("\n[Parent] Parent Process Started. PID: %d, Child PID:
%d\n", getpid(), pid);
if (choice == 1) {
// Normal Execution
printf("[Parent] Sorting array using Bubble Sort...\n");
bubble_sort(parent_arr, n);
printf("[Parent] Sorted array: ");
print_array(parent_arr, n);
printf("[Parent] Waiting for Child process to finish via
wait()...\n");
wait(NULL); // Reaps the child process safely
printf("[Parent] Child process reaped successfully. Parent
Exiting.\n");
}
else if (choice == 2) {
// Zombie state demo: Parent finishes sorting and sleeps
without calling wait()
printf("[Parent] Sorting array using Bubble Sort...\n");
bubble_sort(parent_arr, n);
printf("[Parent] Sleeping for 10 seconds without calling
wait()...\n");
printf("[Parent] Open another terminal now and run 'ps -eo
pid,ppid,stat,cmd | grep Z' to see the ZOMBIE child!\n");
sleep(10);
printf("[Parent] Parent waking up and reaping child now.\n");
wait(NULL);
printf("[Parent] Parent Exiting.\n");
}
else if (choice == 3) {
// Orphan state demo: Parent exits immediately
printf("[Parent] Sorting array using Bubble Sort...\n");
bubble_sort(parent_arr, n);
printf("[Parent] Parent exiting immediately to leave child
ORPHANED.\n");
exit(0);
}
}
return 0;
}
