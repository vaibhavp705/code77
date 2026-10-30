
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// Helper function to sort an array
void sortArray(int arr[], int n) {
for (int i = 0; i < n - 1; i++) {
for (int j = 0; j < n - i - 1; j++) {
if (arr[j] > arr[j + 1]) {
int temp = arr[j];
arr[j] = arr[j + 1];
arr[j + 1] = temp;
}
}
}
}
// 1. Shortest Seek Time First (SSTF) Algorithm
void sstf(int request[], int n, int head) {
int total_seek = 0;
int current_head = head;
int visited[100] = {0};
printf("\n--- SSTF Disk Scheduling ---\n");
printf("Seek Sequence: %d", head);
for (int i = 0; i < n; i++) {
int min_distance = 1e9;
int index = -1;
for (int j = 0; j < n; j++) {
if (!visited[j]) {
int distance = abs(request[j] - current_head);
if (distance < min_distance) {
min_distance = distance;
index = j;
}
}
}
visited[index] = 1;
total_seek += min_distance;
current_head = request[index];
printf(" -> %d", current_head);
}
printf("\nTotal Seek Operations: %d\n", total_seek);
}
// 2. SCAN Algorithm (Elevator) - Moving away from spindle (Right/High)
void scan(int request[], int n, int head, int max_disk) {
int total_seek = 0;
int current_head = head;
// Create a temporary array to hold requests and boundary if needed
int temp[102], total_elements = 0;
for (int i = 0; i < n; i++) {
temp[total_elements++] = request[i];
}
// SCAN needs to go to the extreme end (max_disk - 1) in the initial direction
temp[total_elements++] = max_disk - 1;
sortArray(temp, total_elements);
// Find the split point where requests are greater than or equal to the head
int split_idx = 0;
while (split_idx < total_elements && temp[split_idx] < head) {
split_idx++;
}
printf("\n--- SCAN Disk Scheduling (Moving away from spindle) ---\n");
printf("Seek Sequence: %d", head);
// Step 1: Move right (away from spindle) to the end of the disk
for (int i = split_idx; i < total_elements; i++) {
total_seek += abs(temp[i] - current_head);
current_head = temp[i];
printf(" -> %d", current_head);
}
// Step 2: Reverse direction and service remaining requests on the left
for (int i = split_idx - 1; i >= 0; i--) {
total_seek += abs(temp[i] - current_head);
current_head = temp[i];
printf(" -> %d", current_head);
}
printf("\nTotal Seek Operations: %d\n", total_seek);
}
// 3. C-LOOK Algorithm - Moving away from spindle (Right/High)
void clook(int request[], int n, int head) {
int total_seek = 0;
int current_head = head;
int temp[100];
for (int i = 0; i < n; i++) {
temp[i] = request[i];
}
sortArray(temp, n);
// Find the split point
int split_idx = 0;
while (split_idx < n && temp[split_idx] < head) {
split_idx++;
}
printf("\n--- C-LOOK Disk Scheduling (Moving away from spindle) ---\n");
printf("Seek Sequence: %d", head);
// Step 1: Move right up to the largest request
for (int i = split_idx; i < n; i++) {
total_seek += abs(temp[i] - current_head);
current_head = temp[i];
printf(" -> %d", current_head);
}
// Step 2: Jump directly to the lowest request without servicing tracks in between
if (split_idx > 0) {
total_seek += abs(temp[0] - current_head);
current_head = temp[0];
printf(" -> %d", current_head);
// Step 3: Move right again to service remaining requests up to the split point
for (int i = 1; i < split_idx; i++) {
total_seek += abs(temp[i] - current_head);
current_head = temp[i];
printf(" -> %d", current_head);
}
}
printf("\nTotal Seek Operations: %d\n", total_seek);
}
int main() {
int n, head, max_disk;
printf("Enter the number of requests: ");
scanf("%d", &n);
int request[100];
printf("Enter the request queue elements: ");
for (int i = 0; i < n; i++) {
scanf("%d", &request[i]);
}
printf("Enter initial head position: ");
scanf("%d", &head);
printf("Enter total disk size (e.g., 200 for 0-199): ");
scanf("%d", &max_disk);
// Execute Algorithms
sstf(request, n, head);
scan(request, n, head, max_disk);
clook(request, n, head);
return 0;
}
