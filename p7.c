 
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#define BUFFER_SIZE 1024
int main() {
int pipe1[2]; // Parent -> Child (Pathname of file)
int pipe2[2]; // Child -> Parent (Content of file)
pid_t pid;
// Create the first pipe
if (pipe(pipe1) == -1) {
perror("pipe1 creation failed");
exit(EXIT_FAILURE);
}
// Create the second pipe
if (pipe(pipe2) == -1) {
perror("pipe2 creation failed");
exit(EXIT_FAILURE);
}
// Fork the process
pid = fork();
if (pid < 0) {
perror("fork failed");
exit(EXIT_FAILURE);
}
if (pid > 0) {
// ==================== PARENT PROCESS ====================
close(pipe1[0]); // Close unused read end of pipe1
close(pipe2[1]); // Close unused write end of pipe2
char filepath[BUFFER_SIZE];
char buffer[BUFFER_SIZE];
ssize_t bytes_read;
// Get file path from user or send a test pathname
printf("Parent Process: Enter the file pathname to read: ");
if (fgets(filepath, sizeof(filepath), stdin) != NULL) {
// Remove trailing newline if present
filepath[strcspn(filepath, "\n")] = 0;
}
// Write the file pathname to pipe1
write(pipe1[1], filepath, strlen(filepath) + 1);
close(pipe1[1]); // Close write end after sending
// Wait for the child process to finish writing
wait(NULL);
// Read the file contents from pipe2 and display on standard output
printf("\n--- Content received from Child Process ---\n");
while ((bytes_read = read(pipe2[0], buffer, sizeof(buffer) - 1)) > 0) {
buffer[bytes_read] = '\0';
printf("%s", buffer);
}
printf("\n------------------------------------------\n");
close(pipe2[0]); // Close read end of pipe2
} else {
// ==================== CHILD PROCESS ====================
close(pipe1[1]); // Close unused write end of pipe1
close(pipe2[0]); // Close unused read end of pipe2
char filepath[BUFFER_SIZE];
char buffer[BUFFER_SIZE];
ssize_t bytes_read;
// Read the file pathname from pipe1
bytes_read = read(pipe1[0], filepath, sizeof(filepath) - 1);
if (bytes_read > 0) {
filepath[bytes_read] = '\0';
// Open the requested file
int fd = open(filepath, O_RDONLY);
if (fd < 0) {
char *err_msg = "Error: Could not open the specified file.";
write(pipe2[1], err_msg, strlen(err_msg) + 1);
} else {
// Read contents of the file and write to pipe2
ssize_t n;
while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
write(pipe2[1], buffer, n);
}
close(fd);
}
}
close(pipe1[0]); // Close read end of pipe1
close(pipe2[1]); // Close write end of pipe2
exit(EXIT_SUCCESS);
}
return 0;
}
