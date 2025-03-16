# Minishell

## Overview
Minishell is a lightweight shell implementation designed to replicate essential features of a Unix shell. This project provides a custom command-line interpreter that executes user commands, handles input/output redirection, and supports process control mechanisms.

## Features
- Execution of built-in and external commands
- Input and output redirection
- Support for pipes (`|`)
- Environment variable handling
- Signal handling (Ctrl+C, Ctrl+Z, etc.)
- Process management (foreground & background execution)

## Installation
```bash
git clone https://github.com/yourusername/Minishell.git
cd Minishell
make
```

## Usage

```bash
./minishell
```

Then, enter commands just like in a standard Unix shell.

## Example

```bash
ls -l
echo "Hello, World!"
cat file.txt | grep keyword
export VAR=value
echo $VAR
```

## Project Files
- `main.c` – Entry point of the shell, initializes and runs the main loop.
- `main.h` – Header file containing function declarations and macros.
- `functions.c` – Implements helper functions for shell operations.
- `signals.c` – Handles signal processing (e.g., Ctrl+C, Ctrl+Z).
- `execute_builtin.c` – Implements built-in shell commands.
- `execute_external.c` – Handles execution of external commands.
- `external.txt` – Possibly a configuration or reference file.
- `a.out` – Compiled output (can be ignored or added to `.gitignore`).

## Requirements

- GCC compiler
- Make utility
- Linux/Unix-based system

## Compilation

To compile the project, use:

```bash
make
```

To clean generated files:

```bash
make clean
```

## Contributing

Feel free to fork this repository and submit pull requests with improvements.


