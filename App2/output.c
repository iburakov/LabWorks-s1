#include "globals.h"
#include "output.h"

// Double To String Formatted
// Writes the trailingzeros-formatted string representation of double _x_ to _dest_.
void dtostr_formatted(char *dest, uint dest_size, double x) {
	sprintf_s(dest, dest_size, "%.3lf", x);

	char* dot = strchr(dest, '.');
	if (dot != NULL) {
		for (uint i = 1; i <= 3; ++i) {
			if (dot[i] == '\0') {
				dot[i] = '0';
				dot[i + 1] = '\0';
			}
		}
	}
	else {
		strcat_s(dest, dest_size, ".000");
	}
}


// Double To String Width Formatted
// Writes a string representation of _x_ to _dest_.
//
// string has a fixed _width_ (aligning to right, hiding overflow), and trailingzeros-formatted as well.
void dtostr_width_formatted(char *dest, uint dest_size, double x, uint width) {
	char* xstr = MY_MALLOC(char*, dest_size);
	MALLOC_CHK(xstr, "Couldn't allocate memory for xstr string buffer in dtostr_width_formatted()!");
	dtostr_formatted(xstr, dest_size, x);
	string_width_formatted(dest, dest_size, xstr, width);
	free(xstr);
}	


// String Width Formatted
// Writes a width-formatted string (aligning to right, hiding overflow)
void string_width_formatted(char *dest, uint dest_size, char* str, uint width) {
	char* format = MY_MALLOC(char*, dest_size);
	MALLOC_CHK(format, "Couldn't allocate memory for format string buffer in string_width_formatted()!");
	sprintf_s(format, dest_size, "%%%d.%ds", width, width);
	sprintf_s(dest, dest_size, format, str);
	free(format);
}

// Prints a separator for a table with width defined in _twidth_
void print_table_separator(tablewidth_t twidth, char left, char mid, char right) {
	printf("%c%c", left, BD_HORIZONTAL);
	for (int i = 0; i < twidth.x; ++i) {
		putchar(BD_HORIZONTAL);
	}
	printf("%c%c%c", BD_HORIZONTAL, mid, BD_HORIZONTAL);
	for (int i = 0; i < twidth.y; ++i) {
		putchar(BD_HORIZONTAL);
	}
	printf("%c%c\n", BD_HORIZONTAL, right);
}