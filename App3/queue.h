#ifndef QUEUE_H
#define QUEUE_H

/**
	Run a command line interpreter for queue operating.
	A handler for part 1 of the task.

	@return Exit code.
*/
int run_queue_interpreter();

typedef struct {
	size_t head; // index of the current head element
	size_t tail; // index of the current tail element
	bool empty;
	double *arr; // main queue array
	size_t arrsize; // size of the main array
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
	Pops an element from the queue and writes it to dest.

	@param qptr A pointer to the queue structure to pop from.
	@param dest An address where read value should be written.
	@return Whether the operation was successful or not (SUCCESS or FAILURE).
*/
bool dequeue(queue_t *qptr, double *dest);

/**
	Prints a string representation of the queue to the destination buffer.

	@param dest Destination buffer.
	@param dest_size Size of the destination buffer.
	@param qptr A queue to stringify.
	@return Whether the operation was successful or not (SUCCESS or FAILURE).
*/
bool queue_stringify(char *dest, size_t dest_size, queue_t *qptr);

/**
	Reallocate queue's array to gain more memory.
	
	@param qptr A pointer to the queue structure to extend.
	@return Whether the operation was successful or not (SUCCESS or FAILURE).
*/
bool queue_extend(queue_t *qptr);

/**
	Get the array index which should be next after the index specified. 

	@param ind Index to increment.
	@param qptr A pointer to the queue structure caller is asking about.
	@return An incremented index.
*/
size_t queue_next(size_t ind, queue_t *qptr);

#endif // !QUEUE_H