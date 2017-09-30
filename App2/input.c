#include "globals.h"
#include <stdio.h>

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


// Copies a token from the stdin buffer to _dest_.
// Returns a length of the token.
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


// Parses a double from the stdin buffer to _dest_. Cleans the buffer on failure.
// Returns SUCCESS or FAILURE.
bool getdouble(double *dest) {
	char lastchar;
	char *token = mymalloc(char*, MAX_WORDLEN);
	char *token_end;
	
	assert(token, "Couldn't allocate memory for token string buffer in getdouble()!");

	uint len = getword(token, &lastchar);
	*dest = strtod(token, &token_end);
	if (token != token_end && token_end == (token + len) && (lastchar == '\n' || getchar_after_spaces() == '\n')) {
		free(token);
		return SUCCESS;
	}
	else {
		free(token);
		clean_buffer(lastchar);
		return FAILURE;
	}
}

// Reads the input to the Input struct that _input_ points to. Returns TRUE on success, otherwise FALSE.
//
// actually, the implementation looks ugly enough to me in matters of code design. consider redesigning.
// TODO REDO
bool readinp(input_t *input) {
	while (TRUE) {
		double val;
		uint error = 0;

		printf("Input Radius value: ");
		if (getdouble(&(input->R))) {
			if (input->R > 0 - EPS) {
				printf("Input Xstart: ");
				if (getdouble(&(input->X1))) {
					printf("Input Xend: ");
					if (getdouble(&(input->X2))) {
						printf("Input deltaX: ");
						if (getdouble(&(input->dX))) {
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
