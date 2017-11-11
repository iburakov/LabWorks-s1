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
	@return The length of the token read by the function. 0 on failure.
*/
size_t get_next_token(char *dest, size_t dest_size);

#endif // !INPUT_H