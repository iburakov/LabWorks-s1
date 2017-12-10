#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

typedef uint8_t bool;
#define TRUE 1
#define FALSE 0
#define SUCCESS TRUE
#define FAILURE FALSE

#define TOKEN_SIZE 1024

typedef enum {
	errNo = 0,
	errExit,
	errFatal,
	errTechnical,
} error_t;

extern error_t ERROR;
extern char* ERRSTR;

#endif GLOBALS_H