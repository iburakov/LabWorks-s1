#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>

typedef uint8_t bool;
#define TRUE 1
#define FALSE 0
#define SUCCESS TRUE
#define FAILURE FALSE

typedef enum {
	errNo = 0,
	errExit = 1,

} error_t;

extern error_t ERROR;
extern char* ERRSTR;

#endif GLOBALS_H