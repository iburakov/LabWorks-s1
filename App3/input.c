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
	c = getchar_after_spaces();
	int i = 0;
	while (!isspace(c)) {
		if (i == dest_size) {
			ERROR = errFatal;
			ERRSTR = "Destination buffer is too small to write next token.";
			return FAILURE;
		}
		if (i == 0 && c == EOF) {
			ERROR = errTechnical;
			ERRSTR = "Couldn't get next token: EOF reached.";
			return FAILURE;
		}
		dest[i++] = c;
		c = getchar();
	}
	ungetc(c, stdin);
	dest[i] = '\0';
	return i;
}

size_t tokenize_input(char *** tokens, size_t tokens_size){
	int i = 0;
	while (i < tokens_size) {
		(*tokens)[i] = (char*)malloc(sizeof(char) * TOKEN_BUF_SIZE);
		if (!(*tokens)[i]) {
			ERROR = errFatal;
			ERRSTR = "Couldn't allocate memory for next token while input tokenizing.";
			return FAILURE;
		}
		
		if (!get_next_token((*tokens)[i], TOKEN_BUF_SIZE)) {
			ERROR = errFatal;
			ERRSTR = "Reached end of file while waiting for token.";
			return FAILURE;
		}
		++i;

		char c = getchar_after_spaces();
		if (c == '\n') {
			return i;
		}
		else ungetc(c, stdin);

	}

	eatline();
	ERROR = errTechnical;
	ERRSTR = "Too small tokens buffer to tokenize input.";
	return FAILURE;
}

void eatline() {
	while (getchar() != '\n')
		;
}
