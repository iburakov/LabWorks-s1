#ifndef MATRIX_H
#define MATRIX_H

/**
	Handler for part 2 of the task.

	@return Exit code.
*/
int run_matrix();

/**
	Asks user about matrix width and height and reads these params from the stdin buffer.

	@param x A pointer to matrix width storage.
	@param y A pointer to matrix height storage.
	@return Whether an operation was successful or not.
*/
bool get_params(long long * wptr, long long * hptr);

/**
	Allocates memory for the matrix.

	@param w Target width.
	@param h Target height.
	@return Whether an operation was successful or not.
*/
bool allocate_matrix_mem(double ***mptr, size_t w, size_t h);

/**
	Gets a row from user.

	@param rowptr An array where row values have to be written.
	@param w Length of the row (a.k.a. width of the matrix)
	@return Whether an operation was successful or not.
*/
bool get_row(double *rowptr, size_t w);

/**
	Counts rows with at least 1 zero element in a matrix.

	@param matr Target matrix to process.
	@return A number of rows satisfying described condition.
*/
size_t count_zero_rows(double **matr, size_t w, size_t h);

/**
	Gets an index of row where the longest sequence of equal numbers is presented in a matrix.

	@param matr Target matrix.
	@return An index of row where the longest sequence of equal numbers is presented.
*/
size_t get_longest_eqseq_row(double **matr, size_t w, size_t h);

#endif // !MATRIX_H

