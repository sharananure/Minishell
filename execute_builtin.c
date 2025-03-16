#include "main.h"

extern char pwd_str[40]; // Global variable storing current working directory

// Function to execute built-in shell commands
void execute_internal_commands(char *input_string)
{
    // Handle "exit" command
    if (strcmp(get_command(input_string), "exit") == 0)
    {
        exit(0);
    }
    // Handle "pwd" command
    else if (strcmp(get_command(input_string), "pwd") == 0)
    {
        char buff[40];
        getcwd(buff, sizeof(buff)); // Get current working directory
        printf("%s\n", buff);
    }
    // Handle "cd" command
    else if (strcmp(get_command(input_string), "cd") == 0)
    {
        input_string += 3; // Move past "cd "
        
        // Skip leading spaces
        while (*input_string == ' ')
        {
            input_string++;
        }
        
        // Change directory and handle errors
        if (chdir(input_string) == -1)
        {
            printf("-bash: cd: %s: No such file or directory\n", input_string);
        }
        
        // Update the global working directory variable
        getcwd(pwd_str, sizeof(pwd_str));
    }
    // Handle "echo" command
    else if (strcmp(get_command(input_string), "echo") == 0)
    {
        echo(input_string);
    }
}

// Function to handle the "echo" command
void echo(char *input_string)
{
    input_string += 4; // Move past "echo "
    
    // Skip leading spaces
    while (*input_string == ' ')
    {
        input_string++;
    }
    
    // Handle special cases
    if (input_string[0] == '$' && input_string[1] == '?')
    {
        printf("%d\n", status); // Print last command exit status
    }
    else if (input_string[0] == '$' && input_string[1] == '$')
    {
        printf("%d\n", getpid()); // Print current process ID
    }
    else if (input_string[0] == '$' && strstr(input_string + 1, "shell") != NULL)
    {
        printf("%s\n", getenv("SHELL")); // Print shell environment variable
    }
    else
    {
        printf("%s\n", input_string); // Print the remaining input
    }
}
