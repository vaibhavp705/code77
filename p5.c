
#include <stdio.h>
#include <stdbool.h>
int main() {
int num_processes, num_resources;
printf("=== Banker's Algorithm (Deadlock Avoidance) ===\n");
printf("Enter the number of processes: ");
scanf("%d", &num_processes);
printf("Enter the number of resource types: ");
scanf("%d", &num_resources);
int alloc[num_processes][num_resources];
int max[num_processes][num_resources];
int need[num_processes][num_resources];
int avail[num_resources];
// Input Allocation Matrix
printf("\nEnter the Allocation Matrix:\n");
for (int i = 0; i < num_processes; i++) {
printf("Process P%d: ", i);
for (int j = 0; j < num_resources; j++) {
scanf("%d", &alloc[i][j]);
}
}
// Input Max Matrix
printf("\nEnter the Max Matrix:\n");
for (int i = 0; i < num_processes; i++) {
printf("Process P%d: ", i);
for (int j = 0; j < num_resources; j++) {
scanf("%d", &max[i][j]);
}
}
// Input Available Resources
printf("\nEnter the Available Resources:\n");
for (int i = 0; i < num_resources; i++) {
printf("Resource Type %c: ", 'A' + i);
scanf("%d", &avail[i]);
}
// Calculate the Need Matrix (Need = Max - Allocation)
for (int i = 0; i < num_processes; i++) {
for (int j = 0; j < num_resources; j++) {
need[i][j] = max[i][j] - alloc[i][j];
}
}
// Print calculated Need Matrix for confirmation
printf("\nCalculated Need Matrix:\n");
for (int i = 0; i < num_processes; i++) {
printf("P%d: ", i);
for (int j = 0; j < num_resources; j++) {
printf("%d ", need[i][j]);
}
printf("\n");
}
// Arrays to keep track of safety sequence execution
bool finish[num_processes];
int safe_seq[num_processes];
int work[num_resources];
int count = 0;
// Initialize tracking structures
for (int i = 0; i < num_processes; i++) {
finish[i] = false;
}
for (int i = 0; i < num_resources; i++) {
work[i] = avail[i];
}
// Main Safety Algorithm loop
while (count < num_processes) {
bool found_executable_process = false;
for (int p = 0; p < num_processes; p++) {
// Check if process has not finished yet
if (finish[p] == false) {
bool can_allocate = true;
// Check if its needs can be met by current work resources
for (int r = 0; r < num_resources; r++) {
if (need[p][r] > work[r]) {
can_allocate = false;
break;
}
}
// If resources are available, execute the process
if (can_allocate) {
for (int r = 0; r < num_resources; r++) {
work[r] += alloc[p][r];
}
safe_seq[count++] = p;
finish[p] = true;
found_executable_process = true;
}
}
}
// If no process could be safely executed in an entire loop, a deadlock is present
if (found_executable_process == false) {
printf("\nSystem is in an UNSAFE state! Deadlock cannot be avoided.\n");
return 0;
}
}
// If all processes executed successfully, print the Safe Sequence
printf("\nSystem is in a SAFE state.\nSafe Sequence is: < ");
for (int i = 0; i < num_processes; i++) {
printf("P%d ", safe_seq[i]);
if (i < num_processes - 1) printf("-> ");
}
printf(">\n");
return 0;
}
OUTPUT:
=== Banker's Algorithm (Deadlock Avoidance) ===
Enter the number of processes: 5
Enter the number of resource types: 3
Enter the Allocation Matrix:
Process P0: 0 1 0
Process P1: 2 0 0
Process P2: 3 0 2
Process P3: 2 1 1
Process P4: 0 0 2
Enter the Max Matrix:
Process P0: 7 5 3
Process P1: 3 2 2
Process P2: 9 0 2
Process P3: 2 2 2
Process P4: 4 3 3
Enter the Available Resources:
Resource Type A: 3
Resource Type B: 3
Resource Type C: 2
Calculated Need Matrix:
P0: 7 4 3
P1: 1 2 2
P2: 6 0 0
P3: 0 1 1
P4: 4 3 1
System is in a SAFE state.
Safe Sequence is: < P1 -> P3 -> P4 -> P0 -> P2 >
Practical No 5
Implement the C program for deadlock avoidance algorithm: Bankers Algorithm
#include <stdio.h>
#include <stdbool.h>
#define P 5 // Number of processes
#define R 3 // Number of resource types
// Function to check if the system is in a safe state
bool isSafeState(int available[R], int max[P][R], int allocation[P][R], int safeSequence[P]) {
int need[P][R];
int work[R];
bool finish[P] = {false};
// 1. Calculate the Need matrix: Need = Max - Allocation
for (int i = 0; i < P; i++) {
for (int j = 0; j < R; j++) {
need[i][j] = max[i][j] - allocation[i][j];
}
}
// 2. Initialize Work vector with Available resources
for (int i = 0; i < R; i++) {
work[i] = available[i];
}
int count = 0;
// 3. Find a process whose needs can be satisfied by current Work
while (count < P) {
bool foundProcess = false;
for (int p = 0; p < P; p++) {
// Check if process is not finished yet
if (!finish[p]) {
bool canAllocate = true;
// Check if Need <= Work for all resources
for (int j = 0; j < R; j++) {
if (need[p][j] > work[j]) {
canAllocate = false;
break;
}
}
// If resources can be allocated, simulate completion
if (canAllocate) {
for (int j = 0; j < R; j++) {
work[j] += allocation[p][j];
}
safeSequence[count++] = p;
finish[p] = true;
foundProcess = true;
}
}
}
// If no process could be safely executed in this iteration, system is unsafe
if (!foundProcess) {
return false;
}
}
return true;
}
int main() {
// Available instances of each resource type (A, B, C)
int available[R] = {3, 3, 2};
// Maximum demand of each process
int max[P][R] = {
{7, 5, 3}, // P0
{3, 2, 2}, // P1
{9, 0, 2}, // P2
{2, 2, 2}, // P3
{4, 3, 3} // P4
};
// Resources currently allocated to each process
int allocation[P][R] = {
{0, 1, 0}, // P0
{2, 0, 0}, // P1
{3, 0, 2}, // P2
{2, 1, 1}, // P3
{0, 0, 2} // P4
};
int safeSequence[P];
printf("--- Banker's Algorithm Deadlock Avoidance ---\n\n");
if (isSafeState(available, max, allocation, safeSequence)) {
printf("Result: The system is in a SAFE state.\n");
printf("Safe Execution Sequence: ");
for (int i = 0; i < P; i++) {
printf("P%d", safeSequence[i]);
if (i < P - 1) printf(" -> ");
}
printf("\n");
} else {
printf("Result: The system is in an UNSAFE state! (Deadlock danger)\n");
}
return 0;
}
