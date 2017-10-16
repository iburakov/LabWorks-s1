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
	char *token = MY_MALLOC(char*, MAX_WORDLEN);
	MALLOC_CHK(token, "Couldn't allocate memory for token string buffer in getdouble()!");
	char *token_end;
	

	uint len = getword(token, &lastchar);
	*dest = strtod(token, &token_end);
	if (token != token_end && token_end == (token + len) 
		&& (lastchar == '\n' || getchar_after_spaces() == '\n')
		&& !isnan(*dest) && !isinf(*dest)) {
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
bool read_input(input_t *input) {
	uint error = 0;
	while (TRUE) {
		if (error) {
			char* errstr;
			switch (error)
			{
			case 1: errstr = "can't parse a valid number"; break;
			case 2: errstr = "radius should be greater than zero"; break;
			case 3: errstr = "incompatible parameters - calculation will never stop"; break;
			case 4: errstr = "incompatible parameters - too many steps to do"; break;
			default: errstr = "unknown error"; break;
			}
			printf("Wrong input: %s. Type 'y' to retry, or any other to exit from application: ", errstr);

			char c;
			if ((c = getchar_after_spaces()) == 'y' && (c = getchar_after_spaces()) == '\n') {
				error = 0;
				continue;
			}
			else {
				clean_buffer(c);
				return FAILURE;
			}
		}

		printf("Input Radius value: ");
		if (getdouble(&(input->R)) == FAILURE) {
			error = 1;
			continue;
		}
		if (input->R <= 0 + EPS) {
			error = 2;
			continue;
		}

		printf("Input Xstart: ");
		if (getdouble(&(input->X1)) == FAILURE) {
			error = 1;
			continue;
		}

		printf("Input Xend: ");
		if (getdouble(&(input->X2)) == FAILURE) {
			error = 1;
			continue;
		}

		printf("Input deltaX: ");
		if (getdouble(&(input->dX)) == FAILURE) {
			error = 1;
			continue;
		}
		if (fabs(input->dX - 0) < EPS || 
			(fabs(input->X1 - input->X2) > EPS) && 
			(((input->X2 - input->X1) >= 0) != (input->dX > 0))) {
			error = 3;
			continue;
		}
		if (fabs(input->X1 - input->X2) > EPS && (input->X2 - input->X1) / input->dX > MAX_STEPS) {
			error = 4;
			continue;
		}
		return SUCCESS;
	}
}
