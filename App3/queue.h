#ifndef QUEUE_H
#define QUEUE_H

/**
	Run a command line interpreter for queue operating.
	A handler for part 1 of the task.

	@return Exit code.
*/
int run_queue_interpreter();

typedef struct {
	size_t head;
	size_t tail;
	double *arr;
	size_t arrsize;
} queue_t;

/**
	Initializes queue_t structure with initial values, allocates memory.

	@param qptr Pointer to a query structure to initialize.
	@return Whether the operation was successful or not (SUCCESS or FAILURE).
*/
bool queue_init(queue_t *qptr);

/**
	Puts an element to the queue.
	
	@param qptr A pointer to the target queue structure.
	@param x An element to enqueue.
	@return Whether the operation was successful or not (SUCCESS or FAILURE).
*/
bool enqueue(queue_t *qptr, double x);

/**
	Pops an element from the queue.

	@param qptr A pointer to the queue structure to pop from.
	@return A value of the element that was popped.
*/
bool dequeue(queue_t *qptr);

/**
	Prints a string representation of the queue to the destination buffer.

	@param dest Destination buffer.
	@param dest_size Size of the destination buffer.
	@param qptr A queue to stringify.
	@return Whether the operation was successful or not (SUCCESS or FAILURE).
*/
bool stringify(char *dest, size_t dest_size, queue_t *qptr);

#endif // !QUEUE_H