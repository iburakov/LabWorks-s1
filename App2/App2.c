#include "globals.h"
#include "input.h"
#include "output.h"


input_t input;
tablewidth_t twidth;


double func(double x, double R) {
	if (x > M_PI) {
		return x - M_PI;
	}
	else if (x > 0) {
		return sin(x);
	}
	else if (x > -4) {
		return -0.5 * x;
	}
	else if (x > -5) {
		double k = fabs(x) - 4;
		return 2 + (R - 2) * k;
	}
	else if (x > -5.0 - 2.0 * R) {
		return R - sqrt(R*R - pow(x + 5 + R, 2));
	}
	else if (fabs(x - (-5 - 2 * R)) < EPS) {
		return R;
	}
	else {
		return NAN;
	}
}


// Writes a string repersentation of func(x) to dest.
//
// the string is already baked for printing to table (including fixed _width_)
void strfunc(char *dest, uint dest_size, double x, uint width) {
	double y = func(x, input.R);
	if (fabs(y - 0.0) < 0.001) {
		// -0.0 protection
		y = 0.0;
	}
	if (isnan(y)) {
		string_width_formatted(dest, dest_size, "NaN", width);
	}
	else if (isinf(y) && y > 0.0) {
		string_width_formatted(dest, dest_size, "+Inf", width);
	}
	else if (isinf(y) && y < 0.0) {
		string_width_formatted(dest, dest_size, "-Inf", width);
	}
	else {
		dtostr_width_formatted(dest, dest_size, y, width);
	}
}


int main(void)
{
	setlocale(LC_CTYPE, "Russian");
	printf("Лабораторная работа №2, вариант 3\n");
	printf("Илья Бураков, P3117\n");
	setlocale(LC_CTYPE, "C");

	if (read_input(&input) == SUCCESS) {
		FOR_X_IN_INPUT {
			double y = func(x, input.R);
			uint xstrl = (fabs(x) > 0 - EPS) ? (uint)log10(fabs(x)) + 5 + ((x < 0) ? 1 : 0) : 5;
			uint ystrl = (fabs(y) > 0 - EPS) ? (uint)log10(fabs(y)) + 5 + ((y < 0) ? 1 : 0) : 5;
			
			if (xstrl > twidth.x) twidth.x = xstrl;
			if (ystrl > twidth.y) twidth.y = ystrl;
		}	

		print_table_separator(twidth, BD_TOP_LEFT, BD_TOP_MID, BD_TOP_RIGHT);
		printf("%c ", BD_VERTICAL);
		uint xpos = twidth.x / 2;
		uint ypos = twidth.y / 2;
		for (uint i = 0; i < twidth.x; ++i) {
			if (i == xpos) 
				putchar('X');
			else 
				putchar(' ');
		}
		printf(" %c ", BD_VERTICAL);
		for (uint i = 0; i < twidth.y; ++i) {
			if (i == ypos)
				putchar('Y');
			else
				putchar(' ');
		}
		printf(" %c\n", BD_VERTICAL);
		print_table_separator(twidth, BD_MID_LEFT, BD_MID_MID, BD_MID_RIGHT);

		char *xstr = MY_MALLOC(char*, twidth.x + 1);
		char *ystr = MY_MALLOC(char*, twidth.y + 1);
		MALLOC_CHK(xstr, "Couldn't allocate memory for xstr string buffer during table printing in main()!");
		MALLOC_CHK(ystr, "Couldn't allocate memory for ystr string buffer during table printing in main()!");

		FOR_X_IN_INPUT {
			dtostr_width_formatted(xstr, twidth.x + 1, x, twidth.x);
			strfunc(ystr, twidth.y + 1, x, twidth.y);
			printf("%c %s %c %s %c\n", BD_VERTICAL, xstr, BD_VERTICAL, ystr, BD_VERTICAL);
		}

		print_table_separator(twidth, BD_BOT_LEFT, BD_BOT_MID, BD_BOT_RIGHT);

		free(xstr);
		free(ystr);
	}

	return EXIT_SUCCESS;
}