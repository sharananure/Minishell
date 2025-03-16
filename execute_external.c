#include "main.h"

int status;
char** command_to_array(char *input_string);

// Function to execute external commands, including handling pipes
void execute_external_commands(char *input_string)
{
    char **command_array;
    int pipe_count = 0, i;

    // Count the number of pipes in the input
    for (i = 0; input_string[i] != '\0'; i++)
    {
        if (input_string[i] == '|')
            pipe_count++;
    }
    
    // If no pipes, execute the command directly
    if (pipe_count == 0)
    {
        command_array = command_to_array(input_string);
        if (execvp(command_array[0], command_array) == -1)
        {
            perror("Error: Invalid command");
            exit(0);
        }
        exit(0);
    }

    int pipe_fd[2];
    int read_pipe = 0;

    // Process each command in the pipeline
    for (i = 0; i < pipe_count + 1; i++)
    {
        // Tokenize input string for commands
        if (i == 0)
        {
            command_array = command_to_array(strtok(input_string, "|"));
        }
        else
        {
            command_array = command_to_array(strtok(NULL, "|"));
        }
        
        if (i < pipe_count)
        {
            pipe(pipe_fd); // Create a pipe for inter-process communication
        }

        pid_t pid = fork();

        if (pid > 0) // Parent process
        {
            if (i != 0) 
            {
                close(read_pipe); // Close previous read end
            }
            if (i < pipe_count) 
            {
                close(pipe_fd[1]);
                read_pipe = pipe_fd[0]; // Save read end for next process
            }
        }
        else if (pid == 0) // Child process
        {
            if (i == 0) // First command
            {
                close(pipe_fd[0]);
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            } 
            else if (i < pipe_count) // Intermediate command
            {
                dup2(read_pipe, STDIN_FILENO);
                close(read_pipe);
                close(pipe_fd[0]);
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            } 
            else // Last command
            {
                dup2(read_pipe, STDIN_FILENO);
                close(read_pipe);
            }
            
            // Execute the command
            if (execvp(command_array[0], command_array) == -1) 
            {
                perror("Command execution failed");
                exit(0);
            }
            exit(0);
        }
        else
        {
            perror("ERROR: Creating child process");
            exit(0);
        }
    }

    // Wait for all child processes to finish
    for (i = 0; i < pipe_count + 1; i++)
    {
        wait(&status);
    }
    exit(0);
}

// Function to convert input string into an array of arguments
char** command_to_array(char *input_string) 
{
    int max_args = 10;
    char **command_array = malloc(sizeof(char*) * max_args);
    int i = 0, j = 0, count = 0;
    char buff[20];

    while (input_string[j] != '\0') 
    {
        if (input_string[j] == ' ') 
        {
            if (i > 0) 
            { 
                buff[i] = '\0';
                if (count >= max_args - 1) 
                { 
                    max_args *= 2;
                    command_array = realloc(command_array, sizeof(char*) * max_args);
                }
                command_array[count] = strdup(buff);
                count++;
                i = 0;
            }
            while (input_string[j] == ' ') 
            {
                j++;
            }
        }
        else 
        {
            if (i >= sizeof(buff) - 1) 
            { 
                buff[i] = '\0';
                i = 0;
            }
            buff[i++] = input_string[j++];
        }
    }
    if (i > 0) 
    {
        buff[i] = '\0';
        if (count >= max_args - 1) 
        {
            max_args++;
            command_array = realloc(command_array, sizeof(char*) * max_args);
        }
        command_array[count] = strdup(buff);
        count++;
    }
    command_array[count] = NULL;
    return command_array;
}