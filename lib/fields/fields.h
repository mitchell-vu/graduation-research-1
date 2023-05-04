/* Libraries for fields, doubly-linked lists and red-black trees.
   Copyright (C) 2021 James S. Plank */

#ifndef _FIELDS_
#define _FIELDS_

/* The fields library -- making input processing easier */

#include <stdio.h>
#include <string.h>
#define MAXLEN 1001
#define MAXFIELDS 1000

typedef struct inputstruct
{
  const char *name;        /* File name */
  FILE *f;                 /* File descriptor */
  int line;                /* Line number */
  char text1[MAXLEN];      /* The line */
  char text2[MAXLEN];      /* Working -- contains fields */
  int NF;                  /* Number of fields */
  char *fields[MAXFIELDS]; /* Pointers to fields */
  int file;                /* 1 for file, 0 for popen */
} *IS;

/**
 * @brief Creates a new inputstruct
 * @note Use NULL for stdin.
 *
 * @param filename
 * @return Return inputstruct, or NULL on failure.
 */
extern IS new_inputstruct(char *filename);

/**
 * @brief Creates a new pipe inputstruct from a shell command
 *
 * @param shell_command
 * @return Return inputstruct, or NULL on failure.
 */
extern IS pipe_inputstruct(char *shell_command);

/**
 * @brief Get the line object
 *
 * @param inputstruct
 * @return NF, or -1 on EOF.
 */
extern int get_line(IS inputstruct);

/**
 * @brief frees the IS and fcloses/pcloses the file
 *
 * @param inputstruct
 */
extern void jettison_inputstruct(IS inputstruct); /*  */
#endif