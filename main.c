/*
************************************************************************************************************************************************************
Name                : Sharan A
Date                : 11/03/2025
Project Description : MiniShell is a lightweight command-line shell that provides essential shell functionalities, 
                      including executing built-in and external commands, handling signals, and managing background and foreground processes. 
                      It also features job control, allowing users to pause, resume, and track running processes efficiently.
**************************************************************************************************************************************************************
*/
#include "main.h"

// Global Variables
pid_t pid;                          // Process ID
char pwd_str[40];                   // String to store current working directory
char *external_commands[200];       // Array to store external commands

// List of built-in shell commands
char *builtins[] = {
    "echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
    "set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
    "exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", NULL
};

int main()
{
    // Handle signals for interruption (Ctrl+C) and suspension (Ctrl+Z)
    signal(SIGINT, signal_handler);
    signal(SIGTSTP, signal_handler);
    
    // Get the current working directory
    getcwd(pwd_str, sizeof(pwd_str));
    
    // Define the default shell prompt
    char prompt[100] = "Minishell$";
    char input_string[100];
    
    // Clear the terminal screen at startup
    system("clear");
    
    // Extract external commands from a predefined source
    extract_external_commands(external_commands);
    
    // Start scanning user input in a loop
    scan_input(prompt, input_string);
}

// Function to continuously scan user input and process commands
void scan_input(char *prompt, char *input_string)
{
    while (1)
    {
        pid = 0;
        input_string[0] = '\0'; // Reset input string
        
        // Display the prompt with colors
        printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET ":", prompt);
        printf(ANSI_COLOR_BLUE "~%s" ANSI_COLOR_RESET "$ ", pwd_str);
        
        // Read input from the user
        scanf("%[^\n]", input_string);
        getchar(); // Consume newline character
        
        // Trim leading spaces
        while (input_string[0] == ' ')
        {
            input_string++;
        }
        
        // If input is empty, continue to next iteration
        if (input_string[0] == '\0')
            continue;
        
        // Validate PS1 (prompt string) input
        if (validate_ps1(input_string) == 0)
        {
            strcpy(prompt, input_string + 4); // Update prompt
            prompt[strlen(input_string + 4)] = '\0';
            continue;
        }
        
        // Handle job control commands
        if (strcmp(get_command(input_string), "jobs") == 0)
        {
            print_stopped_process();
            continue;
        }
        if (strcmp(get_command(input_string), "fg") == 0)
        {
            continue_stopped_process(1);
            continue;
        }
        if (strcmp(get_command(input_string), "bg") == 0)
        {
            continue_stopped_process(2);
            continue;
        }
        
        // Extract command and determine its type
        char *command = get_command(input_string);
        int command_type = check_command_type(command);
        
        switch (command_type)
        {
            case BUILTIN:
                // Execute built-in shell commands
                execute_internal_commands(input_string);
                break;
            
            case EXTERNAL:
                pid = fork(); // Create a child process
                if (pid == 0) // Child process
                {
                    signal(SIGINT, SIG_DFL);
                    signal(SIGTSTP, SIG_DFL);
                    //printf("Child process %d\n", getpid());
                    execute_external_commands(input_string);
                }
                else if (pid > 0) // Parent process
                {
                    int child_pid;
                    int status;
                    child_pid = waitpid(-1, &status, WUNTRACED); // Wait for child process
                    //printf("Parent process %d\n", getpid());
                    //printf("Status -> %d\n", status);
                    
                    // If process is stopped, store it
                    if (status == 5247)
                    {
                        store_process(child_pid, input_string);
                    }
                }
                else // Error handling
                {
                    printf("ERROR: Creating Child Process\n");
                }
                break;
            
            default:
                printf("%s: command not found\n", command);
        }
    }
}
