#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define BUF_SIZE 256
#define TRUE 1
#define FALSE 0

#define EXIT_NO_FILE 1
#define EXIT_WRONG_ARGUMENTS 2
#define EXIT_FILE_NOT_FOUND 3
#define EXIT_FILE_READ_ERROR 4

typedef unsigned short bufsize_t;
typedef unsigned int bool;

int print_line_from_file(FILE *fp, long start);
void process_file(FILE * fp);
int finish_file(FILE* fp, char* fn);

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

	process_file(fp);
	return finish_file(fp, argv[1]);
}


int print_line_from_file(FILE *fp, long start) {
	fseek(fp, start, SEEK_SET);

	int c;
	while ((c = fgetc(fp)) != EOF) {
		putc(c, stdout);
		if (c == '\n') {
			return TRUE;
		}
	}

	if (feof(fp)) return TRUE;
	else return FALSE;
}


void process_file(FILE * fp) {
	enum {
		stNothing,
		stPrefix,
		stFirstDigit,
		stSecondDigit,
		stFound
	} last_state = stPrefix;

	int c;
	long line_beg = ftell(fp);
	while ((c = fgetc(fp)) != EOF) {
		switch (last_state)
		{
		case stNothing:
		case stSecondDigit: {
			if (!isalnum(c)) ++last_state;
			else last_state = stNothing;
		} break;

		case stPrefix: {
			if (isdigit(c)) ++last_state;
			else if (isalnum(c)) last_state = stNothing;
		} break;

		case stFirstDigit: {
			if (isdigit(c)) ++last_state;
			else last_state = stNothing;
		} break;
		}

		if (last_state == stFound) {
			if (print_line_from_file(fp, line_beg)) c = '\n';
			else return;
		}

		if (c == '\n') {
			line_beg = ftell(fp);
			last_state = stPrefix;
		}
	}
}


int finish_file(FILE* fp, char* fn) {
	char buf[BUF_SIZE];
	int exit_code;

	if (ferror(fp)) {
		strerror_s(buf, BUF_SIZE, errno);
		printf("File \"%s\" can't be read (a error occurred while reading a file): %s.\n", fn, buf);
		exit_code = EXIT_FILE_READ_ERROR;
	}
	else if (feof(fp)) {
		exit_code = EXIT_SUCCESS;
	}

	if (fclose(fp) == EOF) {
		printf("Couldn't properly close a file!\n");
	}

	return exit_code;
}
