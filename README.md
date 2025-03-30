# Minishell

A simple shell implementation based on Bash, created as part of the 42 curriculum.

## Overview

This minishell project is a simplified command-line interpreter that parses user input, tokenizes it, and executes commands. The implementation includes handling of:

- Command execution
- Redirection operators (`>`, `>>`, `<`, `<<`)
- Pipes (`|`)
- Environment variables
- Quote handling (single and double quotes)
- Error checking

## Architecture

The program is structured around the following main components:

1. **Command Reader**: Uses readline to capture user input.
2. **Syntax Error Checker**: Validates input for proper syntax before processing.
3. **Tokenizer**: Breaks the input into tokens for further processing.
4. **Command Executor**: Handles the execution of commands (not visible in provided code).

## Main Components

### Tokenization Process

The tokenization process breaks down user input into tokens that can be further processed:

1. Input is read using the readline library
2. Syntax is validated to check for errors
3. The input is broken down into tokens
4. Each token is categorized by type (command, argument, redirection, etc.)

### Token Types

Tokens are categorized into several types:

- `WORD_T`: Regular words (commands, arguments)
- `VAR_T`: Environment variables (starting with `$`)
- `PIPE_T`: Pipe operator (`|`)
- `REDIR_F_T`: Forward redirection (`>`)
- `REDIR_B_T`: Backward redirection (`<`)
- `APPEND_T`: Append redirection (`>>`)
- `HERDOC_T`: Heredoc redirection (`<<`)

## Key Functions

### Main Program (`main.c`)

```c
int main()
```
- Presents the command prompt (`minishell$`)
- Reads input from the user
- Checks syntax for errors
- Tokenizes the input
- Displays tokens for debugging

### Tokenizer (`tokenizer.c`)

```c
t_token *tokenize(char *cmd)
```
- Takes the command line input and breaks it into tokens
- Returns a linked list of token structures

```c
int handle_cmd(char *cmd, t_token **head)
```
- Determines the type of token and calls appropriate handler
- Supports variables, operations, quotes, and regular words

```c
int handle_var(char *cmd, t_token **head)
```
- Processes environment variables (tokens starting with `$`)
- Extracts the variable name and creates a token

```c
int handle_operation(char *cmd, t_token **head)
```
- Processes operation tokens (pipes and redirections)
- Determines the exact type of operation

```c
int handle_single_quote(char *cmd, t_token **head)
```
- Processes text within single quotes
- Creates tokens with the quoted text content

### Error Checker (`error_checker.c`)

```c
int syntax_error(char *cmd)
```
- Main error checking function that validates input syntax
- Reports specific errors to the user
- Returns 1 if an error is found, 0 otherwise

```c
int has_unclosed_quotes(char *cmd)
```
- Checks if any quotes (single or double) are left unclosed
- Properly handles escaped quotes

```c
int has_misplaced_pipes(char *cmd)
```
- Verifies that pipes are used correctly
- Checks for empty commands between pipes
- Ignores pipes inside quotes

```c
int has_invalid_redir(char *cmd)
```
- Validates redirection syntax
- Checks for proper use of `>`, `>>`, `<`, and `<<`

```c
int has_logical_op(char *cmd)
```
- Checks for unsupported logical operators (`&&` and `||`)
- Reports them as errors (not implemented in this shell)

## Usage Examples

### Basic Commands

```bash
minishell$ ls -la
```
Tokenizes into:
- `ls` (WORD_T)
- `-la` (WORD_T)

### Pipes

```bash
minishell$ ls -l | grep ".c"
```
Tokenizes into:
- `ls` (WORD_T)
- `-l` (WORD_T)
- `|` (PIPE_T)
- `grep` (WORD_T)
- `".c"` (WORD_T)

### Redirections

```bash
minishell$ echo "Hello World" > output.txt
```
Tokenizes into:
- `echo` (WORD_T)
- `"Hello World"` (WORD_T)
- `>` (REDIR_F_T)
- `output.txt` (WORD_T)

### Variables

```bash
minishell$ echo $USER
```
Tokenizes into:
- `echo` (WORD_T)
- `$USER` (VAR_T)

### Error Examples

```bash
minishell$ echo "unclosed quote
minishell: syntax error: unclosed quote
```

```bash
minishell$ ls |
minishell: syntax error near unexpected token |
```

```bash
minishell$ ls && pwd
minishell: syntax error : unsupported operator &
```

## Error Handling

The shell provides specific error messages for different syntax issues:

1. Unclosed quotes
2. Misplaced pipes (e.g., empty commands)
3. Invalid redirections (e.g., multiple consecutive redirections)
4. Unsupported operators (e.g., `&&`, `||`)

## Implementation Details

### Quote Handling

- Single quotes (`'`) prevent interpretation of all special characters
- Double quotes (`"`) prevent interpretation of most special characters except `$`
- Escaped quotes (`\'` or `\"`) are treated as literal characters

### Variable Expansion

Variables are recognized when prefixed with `$` and are processed separately from regular words.

### Function Return Values

Most handler functions return the number of characters processed, which is used to advance the parsing position:
- Return `-1`: Error in processing
- Return `0`: No characters processed
- Return `>0`: Number of characters processed

