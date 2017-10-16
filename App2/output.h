#ifndef OUTPUT_H
#define OUTPUT_H

#include "globals.h"

// Double To String Formatted
// Writes the trailingzeros-formatted string representation of double _x_ to _dest_.
void dtostr_formatted(char *dest, uint dest_size, double x);

// Double To String Width Formatted
// Writes a string representation of _x_ to _dest_.
//
// string has a fixed _width_ (aligning to right, hiding overflow), and trailingzeros-formatted as well.
void dtostr_width_formatted(char *dest, uint dest_size, double x, uint width);


// String Width Formatted
// Writes a width-formatted string (aligning to right, hiding overflow)
void string_width_formatted(char * dest, uint dest_size, char * str, uint width);

// Prints a separator for a table with width defined in _twidth_
void print_table_separator(tablewidth_t twidth, char left, char mid, char right);

#endif // !OUTPUT_H