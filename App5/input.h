#ifndef INPUT_H
#define INPUT_H
#include "globals.h"

/*
	Reads characters from the stdin buffer until a non-space character occurs.

	@return The first char after spaces or -1 on EOF.
*/
int getchar_after_spaces();


/*
	Reads the next token (a sequence of non-space characters, "word") from the stdin buffer.

	@param dest A buffer of chars where token has to be put.
	@param dest_size A size of dest buffer.
	@return The length of the token read by the function. 0 on failure.
*/
size_t get_next_token(char *dest, size_t dest_size);


/*
	Splits current line from stdin into tokens.

	@param tokens An array where read tokens should be put.
	@param tokens_size A size of tokens buffer.
	@return The length of tokens array produced by the function. 0 on failure.
*/
size_t tokenize_input(char **tokens, size_t tokens_size);


/*
	Consumes (clears) everything from the stdin buffer until newline;
*/
void cleanbuf();

char getkey();

//	Asks a regular y/n question.
//	@return	TRUE if user answered yes, FALSE otherwise.
bool user_agrees();


#endif 

