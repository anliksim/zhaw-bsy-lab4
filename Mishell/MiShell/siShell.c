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
#define MAX_REDIR 2
#define REDIR_IN "<"
#define REDIR_IN_IDX 0
#define REDIR_OUT ">"
#define REDIR_OUT_IDX 1

//*****************************************************************************
// split command line into tokens and store tokens -> argv[]

// !!!! only one token is implemented here 

void tokenizeCommand(char *cmdLine, char *argv[], char *redir[]) {
    const char *errMsg = "\n*** too many arguments ***\n";
    int idx = 0;
    char *token;

    argv[idx++] = strtok(cmdLine, CMD_DL);  // get first word
    while ((token = strtok(NULL, CMD_DL)) != NULL) { // $ continue until end is reached

        if (strcmp(token, REDIR_IN) == 0) {
            // $ write left redirects to index 0
            redir[REDIR_IN_IDX] = strtok(NULL, CMD_DL);
        } else if (strcmp(token, REDIR_OUT) == 0) {
            // $ write right redirects to index 1
            redir[REDIR_OUT_IDX] = strtok(NULL, CMD_DL);
        } else {
            // $ add arguments to argv
            argv[idx++] = token;
        }
    }

    if (idx < MAX_ARGV)
        argv[idx] = NULL;                   // terminate argument list by NULL
    else {
        printf("%s", errMsg);
        argv[0] = NULL;
    }
}

//-----------------------------------------------------------------------------
// execute an external command, exit on failure of fork

void externalCommand(char *argv[], char *redir[]) {


    pid_t PID;                          // process identifier
    if ((PID = fork()) == 0) {          // fork child process

        int fdr, fdw;
        if (redir[REDIR_IN_IDX] != NULL) {
            close(0); // $ close stdin stream
            fdr = open(redir[REDIR_IN_IDX], O_RDONLY); // $ open file read stream
        }
        else if (redir[REDIR_OUT_IDX] != NULL) {
            close(1); // $ close stout stream
            fdw = open(redir[REDIR_OUT_IDX], O_CREAT | O_TRUNC | O_WRONLY, 0644); // $ open file write stream
        }

        execvp(argv[0],  &argv[0]);     // execute command
        printf("!!! panic !!!\n");      // should not come here
        exit(-1);                       // if we came here ... we have to exit

        // $ clean up
        if (redir[REDIR_IN_IDX] != NULL) {
            close(fdr); // $ close file read stream
        }
        else if (redir[REDIR_OUT_IDX] != NULL) {
            close(fdw); // $ close file write stream
        }
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

void executeCommand(char *argv[], char *redir[]) {
    if (argv[0] != NULL && !internalCommand(argv)) {
        externalCommand(argv, redir);
    }
}

//-----------------------------------------------------------------------------
// main program for shell

int main(void) {
    char  *argv[MAX_ARGV];              // pointer to command line arguments
    char  buf[STR_LEN];                 // buffer for command line and command
    char  *redir[MAX_REDIR];            // $ redirect buffer

    while (1) {
        redir[REDIR_IN_IDX] = NULL;     // $ reset redir
        redir[REDIR_OUT_IDX] = NULL;    // $ reset redir

        printf("si ? ");                // print prompt
        readline(buf, STR_LEN);         // read one line from stdin 
        tokenizeCommand(buf, argv, redir);     // split command line into tokens
        executeCommand(argv, redir);           // execute command
    }      
    exit(0);
}

//*****************************************************************************
