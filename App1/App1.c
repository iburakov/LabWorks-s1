#include <stdio.h>
#include <math.h>
#include <string.h>

// clean the stdin buffer
void clear_buffer(char last_char)
{
	if (last_char != '\n') {
		while ((last_char = getchar()) != '\n')
			;
	}
}

// get the first char from the standart buffer skipping spaces. acts like getchar(), but "eats" all spaces before.
char getchar_after_spaces(void)
{
	char c;
	while ((c = getchar()) == ' ') {
		;
	}	
	return c;
}

// compares two doubles: 1 if equal, 0 otherwise.
char equald(double A, double B)
{
	return fabs(A - B) < 1e-7;
}

double z1(double a)
{
	double denom = cos(a) + 1 - 2 * pow(sin(2 * a), 2);
	return equald(denom, 0.0) ? NAN : (sin(2 * a) + sin(5 * a) - sin(3 * a)) / denom;
}

double z2(double a)
{
	return 2 * sin(a);
}

void formatd(double X, char* dest, int destbufsize)
{
	sprintf_s(dest, destbufsize, "%.3F", X);
	int i = strlen(dest) - 1;
	while ('0' == dest[i]) {
		dest[i--] = '\0';
	}
	if ('.' == dest[i]) {
		dest[i] = '\0';
	}
}

int main(void)
{
	while (1) {
		// MAIN MENU
		printf("1. Compute a function\n2. Exit\nChoose a menu item: ");
		int answer = -1;
		do {
			if (!answer) {
				printf("Please, choose a correct menu item: ");
			}
			char c = getchar_after_spaces(), c2;

			if ('1' == c || '2' == c) {
				c2 = getchar_after_spaces();
				if (c2 == '\n')
					answer = c - '0';
				else {
					clear_buffer(c2);
					answer = 0;
				}
			} 
			else {
				clear_buffer(c);
				answer = 0;
			}
		} while (!answer);

		if (1 == answer) {
			// CALCULATION UNIT
			char gotarg = 0;
			double a;
			while (1) {
				printf("Z1 = (sin(2a) + sin(5a) - sin(3a)) / (cos(a) + 1 - 2(sin(2a))^2) \nZ2 = 2sin(a)\nInput argument A: ");

				char c = getchar_after_spaces();
				a = 0.0;
				signed char minus = 1;
				if ('-' == c) {
					minus = -1;
					c = getchar_after_spaces();
				}
				if ('\n' != c) {
					char dot_state = 0;
					while (c >= '0' && c <= '9') {
						a = 10 * a + c - '0';
						if (isinf(a)) {
							dot_state = 0;
							break;
						}
						c = getchar();
						dot_state |= 1;
					}
					if ('.' == c && dot_state) {
						int divisor = 1;
						dot_state |= 2;
						while ((c = getchar()) >= '0' && c <= '9') {
							divisor *= 10;
							a = a + (c - '0') / (float)divisor;
							dot_state |= 4;
						}
					}
					if ((1 == dot_state || 7 == dot_state) && ('\n' == c || ' ' == c && getchar_after_spaces() == '\n')) {
						a *= minus;
						if (!equald(a, 0) || 1 == minus) {
							gotarg = 1;
							break;
						}							
					}
				}
				// invalid input
				clear_buffer(c);
				printf("Wrong input argument A. Press 'y' to continue, another key to return back: ");
				c = getchar_after_spaces();
				if (!(('y' == c || 'Y' == c) && getchar_after_spaces() == '\n')) {
					break;
				}
			}
			if (gotarg) {
				// printf("A = %F\n", a);
				char buf[50];

				double z1_val = z1(a);
				if (isnan(z1_val)) {
					printf("Z1 is undefined\n");
				} 
				else {
					formatd(z1_val, buf, 50);
					printf("Z1 = %s\n", buf);
				}
				formatd(z2(a), buf, 50);
				printf("Z2 = %s\n", buf);
			}
			
		}
		else {	
			// EXIT
			break;
		}
		printf("\n");
	}
	return 0;
}
