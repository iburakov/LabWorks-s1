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
	} while (isspace(c) && (c != '\n'));

	int i = 0;
	while (!isspace(c) && (c != '\n')) {
		if (i == dest_size) {
			assert(FALSE);
			ERROR = errFatal;
			ERRSTR = "Destination buffer is too small to write next token.";
			return FAILURE;
		}
		if (i == 0 && c == EOF) {
			assert(FALSE);
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

size_t tokenize_input(char **tokens, size_t tokens_size) {
	int i = 0;
	while (i < tokens_size) {
		tokens[i] = (char*)malloc(sizeof(char) * TOKEN_SIZE);
		if (!tokens[i]) {
			assert(FALSE);
			ERROR = errFatal;
			ERRSTR = "Couldn't allocate memory for next token while input tokenizing.";
			return FAILURE;
		}

		if (!get_next_token(tokens[i], TOKEN_SIZE)) {
			cleanbuf();
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
	ERRSTR = "Invalid input: too much tokens."; //"Too small tokens buffer to tokenize input.";
	return FAILURE;
}

void cleanbuf() {
	while (getchar() != '\n')
		;
}

char getkey() {
	int key;
	char next;
	if ((key = getchar_after_spaces()) != EOF && key != '\n' && (next = getchar_after_spaces()) == '\n') {
		return (char)key;
	}

	// wrong input
	if (key != EOF && key != '\n') {
		ungetc(next, stdin);
		cleanbuf();
	}
	ERROR = errTechnical;
	ERRSTR = "Invalid input.";
	return FAILURE;
}

bool user_agrees() {
	char key = getkey();
	while (!key && key != 'y' && key != 'n') {
		printf("Invalid input. Try again. 'y' or 'n'.\n");
		key = getkey();
	}
	if (!key) return FAILURE;
	return (key == 'y');
}
