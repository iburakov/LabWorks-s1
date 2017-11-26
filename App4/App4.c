#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define BUF_SIZE 256
#define TRUE 1
#define FALSE 0

#define EXIT_NO_FILE 1
#define EXIT_WRONG_ARGUMENTS 2
#define EXIT_FILE_NOT_FOUND 3
#define EXIT_FILE_READ_ERROR 4

typedef unsigned short bufsize_t;
// typedef unsigned int bool;

void run_file(FILE * fp)
{
	enum {
		stNothing,
		stSpace,
		stFirstDigit,
		stSecondDigit
	} last_state = stNothing;
	
	int c;
	long line_beg;
	while ((c = fgetc(fp)) != EOF) {
		if (c == '\n') {
			// process string termination
		}

		if (/*found 2-digit number*/) {
			// print a string based on stored info
		}

		printf("Next character code read is: %d\n", c);
	}
}

int finish_file(FILE* fp, char* fn) {
	char buf[BUF_SIZE];
	int exit_code; 

	if (ferror(fp)) {
		strerror_s(buf, BUF_SIZE, errno);
		printf("File \"%s\" can't be read: %s.\n", fn, buf);
		exit_code = EXIT_FILE_READ_ERROR;
	}
	else if (feof(fp)) {
		exit_code = EXIT_SUCCESS;
	}
	else {
		assert(FALSE);
	}

	if (fclose(fp) == EOF) {
		printf("Couldn't properly close a file!\n");
	}

	return exit_code;
}

int main(int argc, char* argv[]) {
	if (argc == 1) {
		printf("Please, provide a name of the file.\n");
		return EXIT_NO_FILE;
	}
	else if (argc > 2) {
		printf("Wrong arguments for application: 1 expected, found %d.\n", argc - 1);
		return EXIT_WRONG_ARGUMENTS;
	}

	FILE *fp;
	errno_t error = fopen_s(&fp, argv[1], "r");

	if (!fp) {
		if (error == ENOENT)
		{
			printf("File \"%s\" not found.\n", argv[1]);
			return EXIT_FILE_NOT_FOUND;
		}
		else {
			char errbuf[BUF_SIZE];
			strerror_s(errbuf, BUF_SIZE, error);
			printf("File \"%s\" can't be read: %s.\n", argv[1], errbuf);
			return EXIT_FILE_READ_ERROR;
		}
	}

	run_file(fp);
	return finish_file(fp, argv[1]);
}