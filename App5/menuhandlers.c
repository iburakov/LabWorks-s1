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
	if (state->bfp->fp) {
		printf("Another file is opened right now. Close it? [y/n]\n");
		if (user_agrees()) {
			binfile_unload(state->bfp);
			printf("Current file has been successfully closed.\n");
		}
		else {
			ERROR = errNo;
			return FAILURE;
		}
	}
	printf("Enter a filename: ");
	char* filename;
	if (!tokenize_input(&filename, 1)) {
		ERROR = errTechnical;
		ERRSTR = "Invalid input.";
		return FAILURE;
	}

	if (binfile_load(state->bfp, filename)) {
		printf("File \"%s\" was loaded successfully.\n", filename);
		if (state->win) system("pause");
		free(filename);
		return SUCCESS;
	}
	else {
		free(filename);
		return FAILURE;
	}
}

bool menuhandler_file_rebuild(State * state) {
	printf("This command rearranges the file, moves some blocks around throwing all "
		"the rewritable blocks ""out of the file at all, thus reducing file's size. "
		"\n\nExecution of this command may be costy and should be done as a part of database maintenance. Not implemented.\n");

	if (state->win) system("pause");
	return SUCCESS;
}

bool menuhandler_file_close(State * state) {
	if (state->bfp->fp) {
		printf("Close the current file? [y/n]\n");
		if (user_agrees()) {
			if (!binfile_unload(state->bfp)) return FAILURE;
			printf("Current file has been successfully closed.\n");
			if (state->win) system("pause");
		}
	} else {
		printf("There's no file opened.\n");
		if (state->win) system("pause");
	}
	return SUCCESS;
}

bool menuhandler_records_find(State * state) {
	if (!state->bfp->fp) {
		printf("There's no file opened.\n");
		ERROR = errTechnical;
		return FAILURE;
	}

	printf(
		"Enter space-separated fields that found records must have. Leave blank and press Enter for search help.\n"
	);

	char** input = malloc(5 * sizeof(char*));
	if (!input) {
		ERROR = errFatal;
		ERRSTR = "Couldn't allocate memory in menuhandler_records_add()!";
		return FAILURE;
	}

	int tokensz = tokenize_input(input, 5);
	if (tokensz == 0) {
		ERROR = errTechnical;
		ERRSTR = "Specify some fields to be found in records.\n"
			"Examples: \n"
			"\tIlya \n"
			"\tIlya Burakov\n"
			"\tBurakov Ilya\n"
			"\tBurakov\n"
			"\t01.01.1999\n"
			"\t+78120433243\n"
			"\tAlice 12.10.2010\n";
		for (int i = 0; i < tokensz; ++i) free(input[i]);
		free(input);
		return FAILURE;
	}

	if (tokensz > 4) {
		ERROR = errTechnical;
		ERRSTR = "Too much criterias to meet: nothing will be found. Leave input blank next time to get search help.";
		for (int i = 0; i < tokensz; ++i) free(input[i]);
		free(input);
		return FAILURE;
	}

	Record* found;
	size_t foundsz;

	if (!phonebook_find(state->bfp, input, tokensz, &found, &foundsz, fcAddressesAndData)) return FAILURE;
	
	if (foundsz == 0) {
		printf("No records matching these criterias were found.\n");
	}
	else {
		printf("Found %d record%s:\n", foundsz, (foundsz % 10 == 1 && foundsz % 100 != 11) ? "" : "s");
		int to_print = foundsz;
		while (TRUE) {
			for (int i = 0; i < min(to_print, REC_TABLE_MAX_RECORDS_PRINTED); ++i) phonebook_print_recstr(found[foundsz - to_print + i].str);
			to_print -= min(to_print, REC_TABLE_MAX_RECORDS_PRINTED);
			if (to_print > 0) {
				printf("Print more? [y/n]\n");
				if (!user_agrees()) break;
			}
			else break;
		}
	}

	for (int i = 0; i < foundsz; ++i) free(found[i].str);
	free(found);
	if (state->win) system("pause");
	return SUCCESS;
}

bool menuhandler_records_add(State * state) {
	if (!state->bfp->fp) {
		printf("There's no file opened.\n");
		ERROR = errTechnical;
		return FAILURE;
	}
	
	printf(
		"Input format: \n"
		"<First name> <Last name> <Date of birth in DD.MM.YYYY format> <Phone number in international format>\n\n"
		"Please, provide data of the record to add:\n"
	);
	char** input = malloc(4 * sizeof(char*));
	if (!input) {
		ERROR = errFatal;
		ERRSTR = "Couldn't allocate memory in menuhandler_records_add()!";
		return FAILURE;
	}

	if (tokenize_input(input, 4) != 4) {
		ERROR = errTechnical;
		ERRSTR = "Invalid input. Check the input format and try again.";
		return FAILURE;
	}

	if (!isnumber(input[3])) {
		ERROR = errTechnical;
		ERRSTR = "Invalid phone number. Check the input format and try again.";
		return FAILURE;
	}

	if (!isdate(input[2])) {
		ERROR = errTechnical;
		ERRSTR = "Invalid date of birth. Check the input format and try again.";
		return FAILURE;
	}
	
	if (!phonebook_add(state->bfp, input)) {
		for (int i = 0; i < 4; ++i) free(input[i]);
		free(input);
		return FAILURE;
	}

	printf("A record has been added to the file successfully.\n");
	for (int i = 0; i < 4; ++i) free(input[i]);
	free(input);
	if (state->win) system("pause");
	return SUCCESS;
}

bool menuhandler_records_delete(State * state) {
	if (!state->bfp->fp) {
		printf("There's no file opened.\n");
		ERROR = errTechnical;
		return FAILURE;
	}

	printf(
		"Specify a key of the record you want to be deleted in format: \n"
		"<First name> <Last name>\n"
	);
	char** input = malloc(2 * sizeof(char*));
	if (!input) {
		ERROR = errFatal;
		ERRSTR = "Couldn't allocate memory in menuhandler_records_add()!";
		return FAILURE;
	}

	if (tokenize_input(input, 2) != 2) {
		ERROR = errTechnical;
		ERRSTR = "Invalid input. Check the input format and try again.";
		return FAILURE;
	}

	if (!phonebook_remove(state->bfp, input[0], input[1])) {
		for (int i = 0; i < 2; ++i) free(input[i]);
		free(input);
		return FAILURE;
	}

	printf("A record has been removed from the file successfully.\n");
	for (int i = 0; i < 2; ++i) free(input[i]);
	free(input);
	if (state->win) system("pause");
	return SUCCESS;
}