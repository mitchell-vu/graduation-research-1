#ifndef PRINT_H
#define PRINT_H

#include <stdio.h>
#include <stdlib.h>

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

extern void printGreen(char *string);
extern void printYellow(char *string);
extern void printError(char *string);

extern void printWait();
extern int printStateMenu();

#endif
