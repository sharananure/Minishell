#include "main.h"

extern char *external_commands[200];
extern char *builtins[];

// Function to extract external commands from a file
void extract_external_commands(char **external_commands)
{
    char buff[30];
    char ch;
    int i = 0, count = 0;
    int fd = open("external.txt", O_RDONLY);
    
    if (fd == -1)  // Check if file exists and is accessible
    {
        perror("Error: file not found");
        return;
    }

    while (read(fd, &ch, 1) > 0)  // Read file character by character
    {
        if (ch == '\n')  // End of a command
        {
            buff[i] = '\0';
            external_commands[count] = strdup(buff); // Allocate memory and store command
            count++;
            i = 0;
        }
        else
        {
            if (i < sizeof(buff) - 1) // Prevent buffer overflow
            {
                buff[i++] = ch;
            }
        }
    }
    
    if (i > 0)  // Store last command if file does not end with '\n'
    {
        buff[i] = '\0';
        external_commands[count] = strdup(buff);
        count++;
    }
    
    external_commands[count] = NULL; // Mark end of commands array
    close(fd); // Close file descriptor
}

// Function to validate PS1 prompt format
int validate_ps1(char* input_string)
{
    if ((input_string[0] == 'P' || input_string[0] == 'p') &&
        (input_string[1] == 's' || input_string[1] == 'S') &&
        (input_string[2] == '1') && (input_string[3] == '=') &&
        (input_string[4] != ' '))
    {
        return 0; // Valid PS1 format
    }
    else
    {
        return 1; // Invalid PS1 format
    }
}


// Function to extract the command from input string
char *get_command(char *input_string)
{
    int len = 0;
    while (input_string[len] != ' ' && input_string[len] != '\0')
    {
        len++;
    }
    
    char *command = malloc(len + 1);
    if (!command)  // Check memory allocation
    {
        perror("Memory allocation failed");
        exit(1);
    }
    
    strncpy(command, input_string, len);
    command[len] = '\0';
    return command;
}

// Function to check if a command is builtin or external
int check_command_type(char *command)
{
    int i;
    for (i = 0; builtins[i] != NULL; i++)  // Check if command is builtin
    {
        if (strcmp(builtins[i], command) == 0)
        {
            return BUILTIN;
        }
    }
    
    for (i = 0; external_commands[i] != NULL; i++)  // Check if command is external
    {
        if (strcmp(external_commands[i], command) == 0)
        {
            return EXTERNAL;
        }
    }
    
    return NO_COMMAND; // Command not found
}
