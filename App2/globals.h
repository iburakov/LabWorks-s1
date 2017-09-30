#ifndef GLOBALS_H
#define GLOBALS_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <locale.h>

#define MAX_WORDLEN 0x100
#define SUCCESS 1
#define FAILURE 0
#define TRUE 1
#define FALSE 0
#define EPS 1e-07

#define mymalloc(type, count) (type)malloc(sizeof(type) * count)


typedef unsigned int uint, bool;


typedef struct {
	double R, X1, X2, dX;
} input_t;

typedef struct {
	uint x, y;
} tablewidth_t;

typedef struct {
	int x, y;
} point_t;

#endif // !GLOBALS_H