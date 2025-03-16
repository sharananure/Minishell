#include "main.h"

s_process *process_list = NULL;
int total_stopped_process = 0;

extern pid_t pid;

// Signal handler for SIGINT (Ctrl+C) and SIGTSTP (Ctrl+Z)
void signal_handler(int signum)
{
    if (signum == SIGINT || signum == SIGTSTP)
    {
        printf("\n"); // Print a new line for clean output
    }
}

// Function to store a newly stopped process in the process list
void store_process(int pid, char* pname)
{
    s_process *new_process = (s_process *)malloc(sizeof(s_process));
    if (!new_process)
    {
        perror("malloc failed");
        return;
    }

    new_process->pid = pid;
    strcpy(new_process->pname, pname);
    new_process->link = NULL;

    if (process_list == NULL) // If list is empty, set new process as head
    {
        process_list = new_process;
    }
    else
    {
        // Traverse to the end of the list and append the new process
        s_process *temp = process_list;
        while (temp->link != NULL)
        {
            temp = temp->link;
        }
        temp->link = new_process;
    }
    
    total_stopped_process++; // Increment process count
}

// Function to print all stopped processes (jobs command)
void print_stopped_process()
{
    s_process *temp = process_list;
    if (temp == NULL)
    {
        return; // No stopped processes to display
    }

    int i = 1;
    while (temp != NULL)
    {
        printf("[%d]  Stopped           %s\n", i, temp->pname);
        temp = temp->link;
        i++;
    }
}

// Function to resume a process in the background (bg command)
void bg_process(int pid)
{
    if (kill(pid, SIGCONT) == 0)
    {
        printf("[Background] Resumed PID: %d\n", pid);
    }
    else
    {
        perror("Failed to resume process in background");
    }
}

// Function to resume a process in the foreground (fg command)
void fg_process(int pid)
{
    if (kill(pid, SIGCONT) == 0)
    {
        waitpid(pid, &status, 0); // Wait for the process to finish
        printf("[Foreground] Completed PID: %d\n", pid);
    }
    else
    {
        perror("Failed to resume process in foreground");
    }
}

// Function to continue the most recently stopped process
void continue_stopped_process(int p)
{
    if (process_list == NULL) // If no stopped processes exist
    {
        return;
    }

    s_process *temp = process_list;
    s_process *prev = NULL;

    // Traverse to the last stopped process
    while (temp->link != NULL)
    {
        prev = temp;
        temp = temp->link;
    }

    printf("%s\n", temp->pname);

    // Resume the process in foreground or background based on 'p'
    if (p == 1)
    {
        fg_process(temp->pid);
    }
    else
    {
        bg_process(temp->pid);
    }

    // Remove process from list after resuming
    if (prev == NULL) // If it's the only process in the list
    {
        process_list = NULL;
    }
    else
    {
        prev->link = NULL;
    }

    free(temp); // Free the allocated memory
    total_stopped_process--; // Decrement process count
}
