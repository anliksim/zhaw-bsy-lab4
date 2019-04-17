/******************************************************************************
// Aufgabe:    Simple Shell   
// File:       sishell.c
// Fach:       Betriebssysteme
// Autor:      M. Thaler
// Version:    v.fs19
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h> 
#include <sys/wait.h>

#include "readline.h"

//*****************************************************************************

#define MAX_ARGV  16                    // max number of command line arguments
#define STR_LEN  255                    // max length of command line arguments
#define CMD_DL " "
//*****************************************************************************
// split command line into tokens and store tokens -> argv[]

// !!!! only one token is implemented here 

void tokenizeCommand(char *cmdLine, char *argv[]) {
    const char *errMsg = "\n*** too many arguments ***\n";
    int idx = 0;

    argv[idx++] = strtok(cmdLine, CMD_DL);  // get first word
    while ((argv[idx++] = strtok(NULL, CMD_DL)) != NULL); // continue until end is reached

    if (idx < MAX_ARGV)
        argv[idx] = NULL;                   // terminate argument list by NULL
    else {
        printf("%s", errMsg);
        argv[0] = NULL;
    }
}

//-----------------------------------------------------------------------------
// execute an external command, exit on failure of fork

void externalCommand(char *argv[]) {
    pid_t PID;                          // process identifier
    if ((PID = fork()) == 0) {          // fork child process
        execvp(argv[0],  &argv[0]);     // execute command
        printf("!!! panic !!!\n");      // should not come here
            exit(-1);                   // if we came here ... we have to exit
    }
    else if (PID < 0) {
        printf("fork failed\n");        // fork didn't succeed
        exit(-1);                       // terminate sishell
    }
    else  {                             // here we are parents
        wait(0);                        // wait for child process to terminate
    }
}

//-----------------------------------------------------------------------------
// executes command and returns returns 1 if internal, 0 if not

int internalCommand(char *argv[]) {
    char *cmd = argv[0];

    if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "logout") == 0) {
        // $ terminate if internal command 'exit' or 'logout'
        exit(0);
    } else if (strcmp(cmd, "cd") == 0) {
        // $ cd to path or HOME path if none given
        char *path = argv[1];
        chdir(path != NULL ? path : getenv("HOME"));

        // $ 1 if internal command
        return 1;
    } else {
        // $ 0 if external command
        return 0;
    }
}

//-----------------------------------------------------------------------------
// execute command if not NULL pointer (invalid or "empty" command)

void executeCommand(char *argv[]) {
    if (argv[0] != NULL && !internalCommand(argv)) {
        externalCommand(argv);
    }
}

//-----------------------------------------------------------------------------
// main program for shell

int main(void) {
    char  *argv[MAX_ARGV];              // pointer to command line arguments
    char  buf[STR_LEN];                 // buffer for command line and command

    while (1) {    
        printf("si ? ");                // print prompt
        readline(buf, STR_LEN);         // read one line from stdin 
        tokenizeCommand(buf, argv);     // split command line into tokens
        executeCommand(argv);           // execute command
    }      
    exit(0);
}

//*****************************************************************************
