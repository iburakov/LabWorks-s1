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
	int c = getchar_after_spaces();
	int i = 0;
	while (!isspace(c)) {
		if (i == dest_size) return FAILURE; // buffer too small!
		if (i == 0 && c == EOF) return FAILURE; // end of file
		dest[i++] = c;
		c = getchar();
	}
	dest[i] = '\0';
	return i;
}
