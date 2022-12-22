# OSN Monsoon 2022 assignmnent2


## Requirements

- GCC compiler
- Linux OS (preferably Ubuntu)

## Running Shell

 Run `make` followed by `./a.out`

## File & Functions


### Supporting Files

###### Files containing functions required for proper functioning of the shell.

- `main.c`

  - Run Shell Loop
  - Tokenizes commands and identifies tags
  - Stores and updates history
  - Prints prompt
  - Checks for terminated background processes

### Command Specific Files


- `echo.c`
  - Implements the echo command
  - Removes extra spaces and tabs

- `pwd.c`
  - Prints the working directory

- `cd.c`
  - Navigates to different directories

- `ls.c`
  - Prints all files and sub-directories in a given directory
  - Flags implemented: `a`,`l`,`al` and `la`
  - Supports multiple directories
  - directories are printed in blue , executable in green and regular files are printed in white

- `pinfo.c`
  - Prints info about process with given PID
  - Prints info about the shell script itself if no pid is given

- `discover.c`
  - if no filename is given , it finds all files and directories in the cureent directory / directory whose name is gven
  - if a filename is gven , it filnds all files with that name in the current directory/directory whose name is given

- `foreground.c`
  - Creates a child process and implements it in the foreground.
  - Returns time taken by the process .

- `background.c`
  - Creates a child process and implements it in the foreground.
  - Returns the pid of the process created
- `sig.c`
  - takes job number of a running job and send signal corresponding to the signl number to it
  - gives error for invalid job number

- `jobs.c`
  - displays a list of all current background processes with their pid and status

- `fg.c`
  - brings the running or stopped background process with given job number to the foreground

- `bg.c`
  - changes the status of a stopped background process to running


### Other Implementations
- History
  - stores command history
  - displays upto 20 commands , starting from the latest(default number of commands displayed is 10)
- Redirection
  - > , >> and  <  implemented
- Piping
- Signals
  - `Ctrl+D` exits the shell implementation .
  - `Ctrl+Z` Sends current foreground process to background.
  - `Ctrl+C` Stops current foreground process.

## Assumptions


- `echo` will treat everything as normal character.

- File and directory names don't contain spaces , quotes or other special characters

- `pinfo` may not work on Operating Systems other than Linux / Ubuntu

- Commands will be stored in history if they are not the same as the last command

- only 20 commands are stored history at a time. If the number of commands is more than 20 , the previous ones are deleted.

- History is stored in a the file "history.txt" in the home directory.

- History is stored in the file only if the shell is terminated using "exit".

- Multiple commands are tokenised  based on ';' and '&'

- commands ending with '&' are considered background commands
- exit the shell using 'exit' or `Ctrl+D`
