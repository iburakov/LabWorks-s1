#include "globals.h"
#include "input.h"
#include "queue.h"
#include "matrix.h"


int main(void)
{
	setlocale(LC_CTYPE, "Russian");
	printf("Илья Бураков, вариант 3\n");
	setlocale(LC_CTYPE, "C");

	printf("Please, choose a part of task: ");
	int inp = getchar_after_spaces();

	if (inp != '1' && inp != '2' || getchar_after_spaces() != '\n') {
		printf("Invalid part number, type '1' or '2'\n");
		return 1;
	}

	char* desc = (inp == '1') ? "partdesc1" : "partdesc2";
	printf("Part #%c - %s", inp, desc);

	if (inp == '1') return run_queue_interpreter();
	else return run_matrix();
	// "Incorrect command, please try again!"
	// printf("Bye!\n");
}