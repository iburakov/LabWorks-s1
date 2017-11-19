#include "globals.h"
#include "input.h"
#include "matrix.h"

int run_matrix(){
	double **matr;
	size_t w, h;

	if (!get_params(&w, &h)) {
		printf("Incorrect input!\n");
		return EXIT_FAILURE;
	}
	
	if (!allocate_matrix_mem(&matr, w, h)) {
		printf("Couldn't allocate memory to store values of the matrix.\n");
		return EXIT_FAILURE;
	}

	printf("Enter %llu lines each containing %llu CORRECT values. Incorrect values will be ignored.\n", h, w);
	for (size_t i = 0; i < h; i++) {
		if (!get_row(matr[i], w)) {
			printf("Not enough correct values were met in the last line.\n");
			return EXIT_FAILURE;
		}
	}
	
	printf("Columns with zero: %llu\n", count_zero_rows(matr, w, h));
	printf("Line with same elements: %llu\n", get_longest_eqseq_row(matr, w, h) + 1);

	return EXIT_SUCCESS;
}

bool get_params(long long *wptr, long long *hptr) {
	long long w, h;

	printf("Input matrix width: ");
	if (!get_next_ll(&w) || w < MIN_MATRIX_WIDTH) {
		return FAILURE;
	}

	printf("Input matrix height: ");
	if (!get_next_ll(&h) || h < MIN_MATRIX_HEIGHT) {
		return FAILURE;
	}
	 
	*wptr = w;
	*hptr = h;
	return SUCCESS;
}

bool allocate_matrix_mem(double *** mptr, size_t w, size_t h){
	*mptr = (double**)malloc(sizeof(double*) * h);
	if (!(*mptr)) {
		return FAILURE;
	}

	for (size_t i = 0; i < h; ++i) {
		(*mptr)[i] = (double*)malloc(sizeof(double) * w);
		if (!(*mptr)[i]) {
			return FAILURE;
		}
	}

	return SUCCESS;
}

bool get_row(double * rowptr, size_t w){
	size_t i = 0;
	while (i < w) {
		if (get_next_double(rowptr + i)) {
			++i;
		} else if (ERROR == errFatal) return FAILURE;

		char c;
		if ((c = getchar_after_spaces()) != '\n' || i == w) {
			ungetc(c, stdin);
		} else return FAILURE;
	}
	return SUCCESS;
}

size_t count_zero_rows(double ** matr, size_t w, size_t h){
	size_t counter = 0;
	for (size_t j = 0; j < w; ++j) {
		for (size_t i = 0; i < h; ++i) {
			if (fabs(matr[i][j]) < EPS) {
				++counter;
				break;
			}
		}
	}
	return counter;
}

size_t get_longest_eqseq_row(double ** matr, size_t w, size_t h){
	size_t maxn = 0, maxi = 0, counter = 0;
	double cval = NAN;
	for (size_t i = 0; i < h; ++i) {
		counter = 0;
		for (size_t j = 0; j < w; ++j) {
			if (fabs(matr[i][j] - cval) < EPS) {
				++counter;
			}
			else {
				if (counter > maxn) {
					maxn = counter;
					maxi = i;
				}
				counter = 1;
				cval = matr[i][j];
			}
			if (counter > maxn) {
				maxn = counter;
				maxi = i;
			}
		}
	}
	return maxi;
}
