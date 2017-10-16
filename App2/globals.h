#ifndef GLOBALS_H
#define GLOBALS_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // !M_PI

#define MAX_WORDLEN 0x100
#define MAX_STEPS 1000
#define SUCCESS 1
#define FAILURE 0
#define TRUE 1
#define FALSE 0
#define EPS 1e-07

#define MY_MALLOC(type, count) (type)malloc(sizeof(type) * (count))
#define MALLOC_CHK(var, errmsg) if (!(var)) {printf(#errmsg "\n"); exit(EXIT_FAILURE);}

#define BD_TOP_LEFT 218
#define BD_TOP_MID 194
#define BD_TOP_RIGHT 191
#define BD_MID_LEFT 195
#define BD_MID_MID 197
#define BD_MID_RIGHT 180
#define BD_BOT_LEFT 192
#define BD_BOT_MID 193
#define BD_BOT_RIGHT 217
#define BD_HORIZONTAL 196
#define BD_VERTICAL 179

#define FOR_X_IN_INPUT for (double x = input.X1; fabs(input.X1 - x) < fabs(input.X2 - input.X1) + EPS; x += input.dX)


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