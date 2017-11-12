#ifndef INPUT_H
#define INPUT_H

/**
	Reads characters from the stdin buffer until a non-space character occurs. 

	@return The first char after spaces.
*/
int getchar_after_spaces();

/**
	Reads the next token (a sequence of non-space characters, "word") from the stdin buffer.

	@param dest A buffer of chars where token has to be put.
	@param dest_size A size of dest buffer.
	@return The length of the token read by the function. 0 on failure.
*/
size_t get_next_token(char *dest, size_t dest_size);

/**
	Splits current line from stdin into tokens.

	@param tokens A pointer to array where read tokens should be put.
	@param tokens_size A size of tokens buffer.
	@return The length of tokens array produced by the function. 0 on failure.
*/
size_t tokenize_input(char ***tokens, size_t tokens_size);

/**
	Consumes everything from the stdin buffer until newline;
*/
void eatline();

#endif // !INPUT_H