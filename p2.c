
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int n, a[50], b[50], i, j, t, mode;

    printf("Enter number of integers (1-50): ");
    scanf("%d", &n);

    if (n < 1 || n > 50)
    {
        printf("Invalid number of integers!\n");
        return 1;
    }

    printf("Enter %d integers:\n", n);

    for (i = 0; i < n; i++)
    {
        scanf("%d", &a[i]);
    }

    printf("\n1. Normal Process");
    printf("\n2. Zombie Process");
    printf("\n3. Orphan Process");
    printf("\nEnter your choice: ");
    scanf("%d", &mode);

    if (mode < 1 || mode > 3)
    {
        printf("Invalid choice!\n");
        return 1;
    }

    for (i = 0; i < n; i++)
    {
        b[i] = a[i];
    }

    pid_t p = fork();

    if (p < 0)
    {
        perror("fork failed");
        return 1;
    }

    // CHILD PROCESS
    if (p == 0)
    {
        printf("\nChild Process Started");
        printf("\nChild PID: %d\n", getpid());

        // Selection Sort in child process
        for (i = 0; i < n - 1; i++)
        {
            for (j = i + 1; j < n; j++)
            {
                if (b[i] > b[j])
                {
                    t = b[i];
                    b[i] = b[j];
                    b[j] = t;
                }
            }
        }

        printf("Child Sorted Array: ");

        for (i = 0; i < n; i++)
        {
            printf("%d ", b[i]);
        }

        printf("\n");

        if (mode == 3)
        {
            sleep(5);

            printf("Orphan Child PID: %d\n", getpid());
            printf("Orphan Child New PPID: %d\n", getppid());
        }

        exit(0);
    }

    // PARENT PROCESS
    else
    {
        printf("\nParent Process Started");
        printf("\nParent PID: %d\n", getpid());

        if (mode == 2)
        {
            printf("Parent sleeping for 10 seconds...\n");

            sleep(10);

            printf("Parent woke up.\n");
            printf("The child may have been a zombie during this time.\n");
        }

        if (mode == 3)
        {
            printf("Parent is exiting without waiting.\n");
            printf("The child may become an orphan.\n");
            return 0;
        }

        // Bubble Sort in parent process
        for (i = 0; i < n - 1; i++)
        {
            for (j = 0; j < n - 1 - i; j++)
            {
                if (a[j] > a[j + 1])
                {
                    t = a[j];
                    a[j] = a[j + 1];
                    a[j + 1] = t;
                }
            }
        }

        printf("Parent Sorted Array: ");

        for (i = 0; i < n; i++)
        {
            printf("%d ", a[i]);
        }

        printf("\n");

        // Wait for the child process
        wait(NULL);

        printf("Parent collected child exit status.\n");
    }

    return 0;
}

