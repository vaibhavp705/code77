Practical No 6
FIFO Full duplex communication between two independent processes. First process accepts
sentences and writes on pipe to be read by second process and second process counts number
of characters, number of words and number of lines in accepted sentences,writes this output
in a text file and writes the contents of the file on second pipe to be read by first process and
displays on standard output.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#define BUFFER_SIZE 1024
int main() {
int fd_write, fd_read;
char *fifo1 = "fifo1";
char *fifo2 = "fifo2";
char input_buffer[BUFFER_SIZE];
char output_buffer[BUFFER_SIZE];
printf("--- Process 1 Started ---\n");
while (1) {
// 1. Accept sentences from standard input
printf("\nEnter a sentence (or type 'exit' to quit):\n> ");
if (fgets(input_buffer, BUFFER_SIZE, stdin) == NULL) {
break;
}
// Remove trailing newline character
input_buffer[strcspn(input_buffer, "\n")] = '\0';
// Check for exit condition
if (strcmp(input_buffer, "exit") == 0) {
// Send exit token to Process 2 so it can close gracefully
fd_write = open(fifo1, O_WRONLY);
write(fd_write, input_buffer, strlen(input_buffer) + 1);
close(fd_write);
break;
}
// 2. Open fifo1 for writing and send the text
fd_write = open(fifo1, O_WRONLY);
if (fd_write < 0) {
perror("Error opening fifo1 for writing");
exit(1);
}
write(fd_write, input_buffer, strlen(input_buffer) + 1);
close(fd_write);
// 3. Open fifo2 for reading the processing result
fd_read = open(fifo2, O_RDONLY);
if (fd_read < 0) {
perror("Error opening fifo2 for reading");
exit(1);
}
// Read the contents sent back by Process 2 and display it
printf("\n[Process 1] Received Analysis Report:\n");
int bytes_read;
while ((bytes_read = read(fd_read, output_buffer, BUFFER_SIZE - 1)) > 0) {
output_buffer[bytes_read] = '\0';
printf("%s", output_buffer);
}
close(fd_read);
}
printf("Process 1 terminating.\n");
return 0;
}
Process 2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>
#define BUFFER_SIZE 1024
void analyze_text(const char *text, int *chars, int *words, int *lines) {
int i = 0;
int in_word = 0;
*chars = 0;
*words = 0;
*lines = 0;
if (strlen(text) > 0) {
*lines = 1; // If there is text, there is at least one line
}
while (text[i] != '\0') {
(*chars)++;
if (text[i] == '\n') {
(*lines)++;
}
if (isspace((unsigned char)text[i])) {
in_word = 0;
} else if (!in_word) {
in_word = 1;
(*words)++;
}
i++;
}
}
int main() {
int fd_read, fd_write;
char *fifo1 = "fifo1";
char *fifo2 = "fifo2";
char input_buffer[BUFFER_SIZE];
char file_buffer[BUFFER_SIZE];
printf("--- Process 2 Started (Waiting for data...) ---\n");
while (1) {
// 1. Open fifo1 for reading data from Process 1
fd_read = open(fifo1, O_RDONLY);
if (fd_read < 0) {
perror("Error opening fifo1 for reading");
exit(1);
}
int bytes_read = read(fd_read, input_buffer, BUFFER_SIZE - 1);
close(fd_read);
if (bytes_read <= 0) continue;
input_buffer[bytes_read] = '\0';
// Check for exit condition
if (strcmp(input_buffer, "exit") == 0) {
break;
}
// 2. Perform analytics
int chars = 0, words = 0, lines = 0;
analyze_text(input_buffer, &chars, &words, &lines);
// 3. Write output to a text file
FILE *file_ptr = fopen("report.txt", "w");
if (file_ptr == NULL) {
perror("Error opening file");
exit(1);
}
fprintf(file_ptr, " ------------------------ \n");
fprintf(file_ptr, "Original Text: %s\n", input_buffer);
fprintf(file_ptr, "Total Characters: %d\n", chars);
fprintf(file_ptr, "Total Words : %d\n", words);
fprintf(file_ptr, "Total Lines : %d\n", lines);
fprintf(file_ptr, " ------------------------ \n");
fclose(file_ptr);
// 4. Read the contents of the text file and write to fifo2
fd_write = open(fifo2, O_WRONLY);
if (fd_write < 0) {
perror("Error opening fifo2 for writing");
exit(1);
}
file_ptr = fopen("report.txt", "r");
if (file_ptr != NULL) {
while (fgets(file_buffer, BUFFER_SIZE, file_ptr) != NULL) {
write(fd_write, file_buffer, strlen(file_buffer));
}
fclose(file_ptr);
}
close(fd_write);
}
printf("Process 2 terminating.\n");
return 0;
}
