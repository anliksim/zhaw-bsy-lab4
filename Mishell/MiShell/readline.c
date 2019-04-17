/******************************************************************************
// File:    readline.c
// Fach:    BSy
// Autor:   M. Thaler
// Version: v.fs19
******************************************************************************/

#include <stdio.h>

/*****************************************************************************/
/* readline reads at most count-1 characters from stdin into the buffer buf  */
/* returns the number of characters (! must be less/equal : count-1          */

int readline(char *buf, int count) {

    char c = EOF;
    int i;
    for(i = 0; (c = getchar()) != EOF && c != '\n' && i < (count - 1); i++) {
        buf[i] = c;
    }
    buf[i] = '\0';

    return i;
}

/*****************************************************************************/
