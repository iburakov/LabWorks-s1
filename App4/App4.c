#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define BUF_SIZE 32768
#define TRUE 1
#define FALSE 0

#define EXIT_NO_FILE 1
#define EXIT_WRONG_ARGUMENTS 2
#define EXIT_FILE_NOT_FOUND 3
#define EXIT_FILE_READ_ERROR 4

typedef unsigned short bufsize_t;

void run_through_file(FILE *fp, char* buf, bufsize_t buf_size);

int main(int argc, char* argv[]) {
	if (argc == 1) {
		printf("Please, provide a name of the file.");
		return EXIT_NO_FILE;
	}
	else if (argc > 2) {
		printf("Wrong arguments for application: 1 expected, found %d.", argc - 1);
		return EXIT_WRONG_ARGUMENTS;
	}

	char buf[BUF_SIZE];
	FILE *fp;
	errno_t error = fopen_s(&fp, argv[1], "r");

	if (!fp) {
		if (error == ENOENT)
		{
			printf("File \"%s\" not found.", argv[1]);
			return EXIT_FILE_NOT_FOUND;
		}
		else {
			strerror_s(buf, BUF_SIZE, error);
			printf("File \"%s\" can't be read: %s.", argv[1], buf);
			return EXIT_FILE_READ_ERROR;
		}
	}

	run_through_file(fp, buf, BUF_SIZE);	

	if (ferror(fp)) {
		strerror_s(buf, BUF_SIZE, error);
		printf("File \"%s\" can't be read: %s.", argv[1], buf);
		return EXIT_FILE_READ_ERROR;
	}
	else if (feof(fp)) {
		fclose(fp);
		return EXIT_SUCCESS;
	}
	else {
		assert(FALSE);
	}
}

void run_through_file(FILE * fp, char * buf, bufsize_t buf_size)
{
	enum {
		stNothing,
		stSpace,
		stFirstDigit,
		stSecondDigit
	} last_state = stNothing;

	while (fgets(buf, buf_size, fp)) {
		for (bufsize_t i = 0; buf[i] != '\n'; ++i) {
			if (buf[i] == '\0') {
				errno = EOVERFLOW;

				return;
			}


			
		}
	}		
}
