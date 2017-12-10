#include "menuhandlers.h"

bool menuhandler_exit(State * state) {
	if (state->menu->parent) {
		state->menu = state->menu->parent;
		return SUCCESS;
	}

	ERROR = errExit;
	ERRSTR = "Bye!";
	return FAILURE;
}

bool menuhandler_file_open(State * state) {
	printf("*opens a file*\n");
	system("pause");
	return SUCCESS;
}

bool menuhandler_file_rebuild(State * state) {
	printf("*rebuilds a file*\n");
	system("pause");
	return SUCCESS;
}

bool menuhandler_file_close(State * state) {
	printf("*closes a file*\n");
	system("pause");
	return SUCCESS;
}

bool menuhandler_records_find(State * state) {
	printf("*finds a record*\n");
	system("pause");
	return SUCCESS;
}

bool menuhandler_records_add(State * state) {
	printf("*adds a record*\n");
	system("pause");
	return SUCCESS;
}

bool menuhandler_records_delete(State * state) {
	printf("*deletes a record*\n");
	system("pause");
	return SUCCESS;
}