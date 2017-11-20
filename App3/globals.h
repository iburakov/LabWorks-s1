#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <assert.h>

typedef unsigned int bool;

#define SUCCESS 1
#define FAILURE 0
#define TRUE 1
#define FALSE 0 
#define TOKEN_SIZE 1024
#define TOKENS_BUF_SIZE 4

#define QUEUE_DEFAULT_ARRAY_SIZE 100
#define QUEUE_ARRAY_REALLOC_STEP 50

#define MIN_MATRIX_WIDTH 2
#define MIN_MATRIX_HEIGHT 2

#define EPS 1e-13

typedef enum {
	errNo = FALSE,
	errUnknownCmd,
	errInvalidOper,
	errTechnical,
	errFatal
} error_t; 

extern char *ERRSTR;
extern error_t ERROR;

#endif // !GLOBALS_H