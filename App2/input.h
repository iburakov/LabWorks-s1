#ifndef INPUT_H
#define INPUT_H

#include "globals.h"

int getchar_after_spaces(void);

void clean_buffer(char lastchar);

// Copies a token from the stdin buffer to _dest_.
// Returns a length of the token.
uint getword(char *dest, char *lastchar);

// Parses a double from the stdin buffer to _dest_. Cleans the buffer on failure.
// Returns SUCCESS or FAILURE.
bool getdouble(double *dest);

// Reads the input to the Input struct that _input_ points to. Returns TRUE on success, otherwise FALSE.
//
// actually, the implementation looks ugly enough to me in matters of code design. consider redesigning.
bool read_input(input_t *input);

#endif // !INPUT_H