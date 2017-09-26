#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


#define MAX_WORDLEN 0x100
#define SUCCESS 1
#define FAILURE 0
#define TRUE 1
#define FALSE 0
#define EPS 1e-07


typedef unsigned int uint, bool;


struct {
	double R, X1, X2, dX;
} input;

struct {
	uint x, y;
} twidth;

struct Point {
	int x, y;
};


bool readinp(void);
double func(double x);
uint strfunc(double x);


int main(void)
{
	printf("Laboratornaya rabota #2, variant 3\n");
	printf("Ilya Burakov, P3117\n");

	if (readinp() == SUCCESS) {
		
	}

	return EXIT_SUCCESS;
}


int getchar_after_spaces(void) {
	int c;
	while (isspace(c = getchar()) && c != '\n')
		;
	return c;
}


void clean_buffer(char lastchar) {
	while (lastchar != '\n' && getchar() != '\n')
		;
}


// returns a length of the token copied from the stdin buffer to _dest_
uint getword(char *dest, char *lastchar) {
	uint len = 0;
	int c = getchar_after_spaces();
	while (!isspace(c) && len < (MAX_WORDLEN - 1)) {
		dest[len++] = c;
		c = getchar();
	}
	dest[len] = '\0';
	*lastchar = c;
	return len;
}


// returns TRUE if double from stdin was successfully parsed to _dest_, otherwise cleans the buffer and returns FALSE
bool getdouble(double *dest) {
	char lastchar;
	char *token = (char*)malloc(MAX_WORDLEN);
	char *token_end;
	uint len = getword(token, &lastchar);
	*dest = strtod(token, &token_end);
	if (token != token_end && token_end == (token + len) && (lastchar == '\n' || getchar_after_spaces() == '\n')) {
		return SUCCESS;
	}
	else {
		clean_buffer(lastchar);
		return FAILURE;
	}
}


bool readinp(void) {
	while (TRUE) {
		double val;
		uint error = 0;

		printf("Input Radius value: ");
		if (getdouble(&(input.R))) {
			if (input.R > 0 - EPS) {
				printf("Input Xstart: ");
				if (getdouble(&(input.X1))) {
					printf("Input Xend: ");
					if (getdouble(&(input.X2))) {
						printf("Input deltaX: ");
						if (getdouble(&(input.dX))) {
							return SUCCESS;
						}
						else error = 1;
					}
					else error = 1;
				}
				else error = 1;
			}
			else error = 2;
		}
		else error = 1;

		char* errstr;
		switch (error)
		{
		case 1: errstr = "can't parse a valid number"; break;
		case 2: errstr = "radius should be greater than zero"; break;
		default: errstr = "unknown error"; break;
		}
		printf("Wrong input: %s. Type 'y' for repeat, or any other for exit from application: ", errstr);
		if (getchar_after_spaces() == 'y' && getchar_after_spaces() == '\n') {
			continue;
		}
		else return FAILURE;
	}
}


double func(double x) {
	return .0;
}


// returns a length of string representation of func(x) written to _dest_
uint strfunc(char *dest, uint dest_size, double x) {
	double y = func(x);
	if (isnan(y)) {
		dest = "NaN";
		return 4;
	}
	else if (isinf(y) && y > 0.0) {
		dest = "+INF";
		return 4;
	}
	else if (isinf(y) && y < 0.0) {
		dest = "-INF";
		return 4;
	}
	else {
		char* tmp = (char*)malloc(dest_size);
		sprintf_s(tmp, dest_size, "%.3d", y);
		char* dot = strchr(tmp, '.');
		for (uint i = 1; i <= 3; ++i) {
			if (dot[i] == '\0') {
				dot[i] = '0';
				dot[i + 1] = '\0';
			}
		}
	}
	
}