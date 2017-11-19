#include "globals.h"
#include "input.h"
#include "queue.h"

int run_queue_interpreter(){
	queue_t queue;
	char **tokens = (char**)malloc(sizeof(char*) * 4);
	if (!tokens) {
		ERROR = errFatal;
		ERRSTR = "Couldn't allocate memory for input tokens buffer!";
	}


	queue_init(&queue);

	while (TRUE) {
		switch (ERROR) {
			case errUnknownCmd: printf("Unknown command, please try again!\n"); ERROR = errNo; break;
			case errInvalidOper: printf("Invalid operation! %s\n", ERRSTR); ERROR = errNo; break;
			case errTechnical: printf("Technical error! %s\n", ERRSTR); ERROR = errNo; break;
			case errFatal: printf("FATAL ERROR! %s\n", ERRSTR); return EXIT_FAILURE;
			default: break;
		}

		size_t tokens_count = tokenize_input(&tokens, 4);

		if (tokens_count == 0) {
			continue;
		}
		else if (tokens_count == 1) {
			if (strcmp(tokens[0], "dequeue") == 0) {
				double x;
				if (!dequeue(&queue, &x)) continue;

				char buf[TOKEN_BUF_SIZE];
				stringify(buf, TOKEN_BUF_SIZE, &queue);
				printf("Read value is %.3f\n", x);
				printf("%s\n", buf);
			}
			else if (strcmp(tokens[0], "done") == 0) {
				printf("Bye!\n");
				return EXIT_SUCCESS;
			}
			else if (strcmp(tokens[0], "enqueue") == 0) {
				ERROR = errInvalidOper;
				ERRSTR = "Expected number to enqueue, found end of file.";
				continue;
			}
			else {
				ERROR = errUnknownCmd;
				continue;
			}
		}
		else if (tokens_count == 2 && strcmp(tokens[0], "enqueue") == 0) {
			char *last;
			double x = strtod(tokens[1], &last);
			if (*last != '\0') {
				ERROR = errInvalidOper;
				ERRSTR = "Couldn't convert argument to a decimal number.";
				continue;
			}

			if (x > 1e13) {
				ERROR = errInvalidOper;
				ERRSTR = "Decimal number is too big for 'double' type.";
				continue;
			}

			if (!enqueue(&queue, x)) continue;

			char buf[TOKEN_BUF_SIZE];
			stringify(buf, TOKEN_BUF_SIZE, &queue);
			printf("%s\n", buf);
		}
		else {
			ERROR = errUnknownCmd;
			continue;
		}
	}
}

bool queue_init(queue_t * qptr){
	qptr->arr = (double*)malloc(sizeof(double) * QUEUE_DEFAULT_ARRAY_SIZE);
	if (!qptr->arr) {
		ERROR = errFatal;
		ERRSTR = "Couldn't allocate memory for a queue.";
		return FAILURE;
	}

	qptr->arrsize = QUEUE_DEFAULT_ARRAY_SIZE;
	qptr->empty = TRUE;
	return SUCCESS;
}

bool enqueue(queue_t * qptr, double x){
	if (next(qptr->tail, qptr) == qptr->head && queue_extend(qptr) == FAILURE) {
		return FAILURE;
	}

	if (qptr->empty) {
		qptr->arr[0] = x;
		qptr->tail = 0;
		qptr->head = 0;
		qptr->empty = FALSE;
	} else {
		qptr->tail = next(qptr->tail, qptr);
		qptr->arr[qptr->tail] = x;
	}
	
	return SUCCESS;
}

bool dequeue(queue_t *qptr, double *dest) {
	if (qptr->empty) {
		ERROR = errInvalidOper;
		ERRSTR = "Can't dequeue from an empty queue.";
		return FAILURE;
	}

	*dest = qptr->arr[qptr->head];

	if (qptr->head == qptr->tail) {
		qptr->empty = TRUE;
	} else {
		qptr->head = next(qptr->head, qptr);
	}
  
	return SUCCESS;
}

bool stringify(char * dest, size_t dest_size, queue_t * qptr){
	if (qptr->empty) {
		sprintf_s(dest, dest_size, "Queue-0 []");
		return SUCCESS;
	}

	if (qptr->head == qptr->tail) {
		sprintf_s(dest, dest_size, "Queue-1 [%.3f]", qptr->arr[qptr->head]);
		return SUCCESS;
	}

	char *qlist = (char*)malloc(sizeof(char) * dest_size);
	if (!qlist) {
		ERROR = errTechnical;
		ERRSTR = "Couldn't allocate memory for a temporary buffer for queue stringifying.";
		return FAILURE;
	}
	qlist[0] = '\0';

	char buf[TOKEN_BUF_SIZE];
	size_t len = 1;
	for (size_t i = qptr->head; i != qptr->tail; i = next(i, qptr)) {
		sprintf_s(buf, TOKEN_BUF_SIZE, "%.3f, ", qptr->arr[i]);
		++len;

		strcat_s(qlist, sizeof(char) * dest_size, buf);
	}
	sprintf_s(buf, TOKEN_BUF_SIZE, "%.3f", qptr->arr[qptr->tail]);
	strcat_s(qlist, sizeof(char) * dest_size, buf);


	sprintf_s(dest, dest_size, "Queue-%d [%s]", (unsigned int)len, qlist);
	free(qlist);
	return SUCCESS;
}

bool queue_extend(queue_t * qptr){
	size_t newsize = qptr->arrsize + QUEUE_ARRAY_REALLOC_STEP;
	double* newarr = (double*)realloc(qptr->arr, sizeof(double) * newsize);
	if (!newarr) {
		ERROR = errTechnical;
		ERRSTR = "Couldn't allocate more memory for the queue.";
		return FAILURE;
	}

	if (!qptr->empty && qptr->tail < qptr->head) {
		for (int i = 0; i <= qptr->tail; ++i) {
			qptr->arr[(qptr->arrsize + i) % newsize] = qptr->arr[i];
		}
	}

	qptr->arrsize = newsize;
	return SUCCESS;
}

size_t next(size_t ind, queue_t * qptr){
	return (ind + 1) % qptr->arrsize;
}
