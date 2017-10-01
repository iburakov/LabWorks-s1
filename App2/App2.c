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
	if (isnan(y)) {
		swf(dest, dest_size, "NaN", width);
	}
	else if (isinf(y) && y > 0.0) {
		swf(dest, dest_size, "+Inf", width);
	}
	else if (isinf(y) && y < 0.0) {
		swf(dest, dest_size, "-Inf", width);
	}
	else {
		dtoswf(dest, dest_size, y, width);
	}
}


int main(void)
{
	setlocale(LC_CTYPE, "Russian");
	printf("Лабораторная работа №2, вариант 3\n");
	printf("Илья Бураков, P3117\n");

	if (readinp(&input) == SUCCESS) {
		uint bufsz = sizeof(char*) * MAX_WORDLEN;
		char *xstr = mymalloc(char*, MAX_WORDLEN);
		char *ystr = mymalloc(char*, MAX_WORDLEN);
		assert(xstr, "Couldn't allocate memory for xstr string buffer during table printing in main()!");
		assert(ystr, "Couldn't allocate memory for ystr string buffer during table printing in main()!");

		for (double x = input.X1; x < input.X2 + EPS; x += input.dX) {
			double y = func(x, input.R);
			uint xstrl = (fabs(x) > 0.0 - EPS) ? (uint)log10(fabs(x)) + 5 + ((x < 0) ? 1 : 0) : 5;
			uint ystrl = (fabs(y) > 0.0 - EPS) ? (uint)log10(fabs(y)) + 5 + ((y < 0) ? 1 : 0) : 5;
			
			if (xstrl > twidth.x) twidth.x = xstrl;
			if (ystrl > twidth.y) twidth.y = ystrl;
		} 

		tsep(twidth);
		printf("| ");
		uint xpos = twidth.x / 2;
		uint ypos = twidth.y / 2;
		for (uint i = 0; i < twidth.x; ++i) {
			if (i == xpos) 
				putchar('X');
			else 
				putchar(' ');
		}
		printf(" | ");
		for (uint i = 0; i < twidth.y; ++i) {
			if (i == ypos)
				putchar('Y');
			else
				putchar(' ');
		}
		printf(" |\n");
		tsep(twidth);

		for (double x = input.X1; x < input.X2 + EPS; x += input.dX) {
			dtoswf(xstr, bufsz, x, twidth.x);
			strfunc(ystr, bufsz, x, twidth.y);
			printf("| %s | %s |\n", xstr, ystr);
		}	

		tsep(twidth);

		free(xstr);
		free(ystr);
	}

		return EXIT_SUCCESS;
}