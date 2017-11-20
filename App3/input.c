#include "globals.h"
#include "input.h"

int getchar_after_spaces() {
	char c;
	while (isspace(c = getchar()) && c != '\n')
		;
	return c;
}

size_t get_next_token(char * dest, size_t dest_size)
{
	char c;
	do {
		c = getchar_after_spaces();
	} while (isspace(c));

	int i = 0;
	while (!isspace(c)) {
		if (i == dest_size) {
			ERROR = errFatal;
			ERRSTR = "Destination buffer is too small to write next token.";
			return FAILURE;
		}
		if (i == 0 && c == EOF) {
			ERROR = errFatal;
			ERRSTR = "Couldn't get next token: EOF or EOL reached.";
			return FAILURE;
		}
		dest[i++] = c;
		c = getchar();
	}
	ungetc(c, stdin);
	dest[i] = '\0';
	return i;
}

size_t tokenize_input(char **tokens, size_t tokens_size){
	int i = 0;
	while (i < tokens_size) {
		tokens[i] = (char*)malloc(sizeof(char) * TOKEN_SIZE);
		if (!tokens[i]) {
			ERROR = errFatal;
			ERRSTR = "Couldn't allocate memory for next token while input tokenizing.";
			return FAILURE;
		}
		
		if (!get_next_token(tokens[i], TOKEN_SIZE)) {
			return FAILURE;
		}
		++i;

		char c = getchar_after_spaces();
		if (c == '\n') {
			return i;
		}
		else ungetc(c, stdin);

	}

	// while terminated by condition: exceeded tokens number limit
	cleanbuf();
	ERROR = errTechnical;
	ERRSTR = "Too small tokens buffer to tokenize input.";
	return FAILURE;
}

void cleanbuf() {
	while (getchar() != '\n')
		;
}

bool get_next_ll(long long * valptr){
	char buf[TOKEN_SIZE];
	char *last;

	if (!get_next_token(buf, TOKEN_SIZE) || getchar_after_spaces() != '\n') {
		return FAILURE;
	}

	long long val = strtoll(buf, &last, 10);
	if (*last != '\0') {
		return FAILURE;
	}

	*valptr = val;
	return SUCCESS;
}

bool get_next_double(double * valptr){
	char buf[TOKEN_SIZE];
	char *last;

	if (!get_next_token(buf, TOKEN_SIZE)) {
		return FAILURE;
	}

	double val = strtod(buf, &last);
	if (*last != '\0') {
		return FAILURE;
	}

	if (val > 1 / EPS || isinf(val) || isnan(val)) {
		return FAILURE;
	}

	*valptr = val;
	return SUCCESS;
}
