#include "globals.h"
#include "queue.h"

int run_queue_interpreter(){
	queue_t queue;

	return 0;
}

bool queue_init(queue_t * qptr){
	qptr->arr = (double*)malloc(sizeof(double) * QUEUE_DEFAULT_ARRAY_SIZE);
	if (!qptr->arr) {
		printf("Error! Couldn't allocate memory for a queue.\n");
		return FAILURE;
	}

	qptr->arrsize = QUEUE_DEFAULT_ARRAY_SIZE;
	qptr->head = 0;
	qptr->tail = 0;
	return SUCCESS;
}

bool enqueue(queue_t * qptr, double x){
	// TODO
}

bool dequeue(queue_t *qptr){
	// TODO
}

bool stringify(char * dest, size_t dest_size, queue_t * qptr){
	// TODO
}
