#include "globals.h"
#include "input.h"

char* get_description(int inp) {
	switch (inp)
	{
	case '1': return "partdesc1"; break;
	case '2': return "partdesc2"; break;
	default: return "No description provided"; break;
	}
}

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
	} // стоит ли писать else после if'а с безусловным return'ом?

	char* desc = (inp == '1') ? "partdesc1" : "partdesc2";
	printf("Part #%c - %s", inp, desc);






	printf("Bye!\n");
	return 0;
}