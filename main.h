#ifndef MAIN_H
#define MAIN_H

// Standard Library Headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

// Process Structure Definition
// Used to store background processes and their details
typedef struct store_pro
{
    int pid;                 // Process ID
    char pname[30];          // Process name
    struct store_pro *link;  // Pointer to next process
}s_process;

// Global Variable
extern int status; // Stores the status of the last executed process

// Command Types
#define BUILTIN     1   // Built-in shell command
#define EXTERNAL    2   // External command
#define NO_COMMAND  3   // Invalid/unknown command

// ANSI Color Codes for Terminal Output
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// Function Prototypes

// User Input Handling
void scan_input(char *prompt, char *input_string);
char *get_command(char *input_string);

// Command Execution
void execute_internal_commands(char *input_string);
void execute_external_commands(char *input_string);

// Command Type Checking
int check_command_type(char *command);

// Shell Utility Functions
void copy_change(char *prompt, char *input_string);
void echo(char *input_string);
void extract_external_commands(char **external_commands);
int validate_ps1(char* input_string);

// Process Management
void store_process(int pid, char* pname);
void print_stopped_process();
void continue_stopped_process(int p);

// Signal Handling
void signal_handler(int signum);

#endif // MAIN_H
