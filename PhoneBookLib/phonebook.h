#ifndef PHONEBOOK_H
#define PHONEBOOK_H
#include "binfile.h"

//	 All functions declared in "phonebook.h" and "binfile.h" support error handling by using these global variables:
//			error_t ERROR
//			char* ERRSTR

#define REC_TABLE_KEY_SPACE_FILL 30
#define REC_TABLE_MAX_RECORDS_PRINTED 50

typedef struct {
	address_t addr;
	char* str;
} Record;


bool isnumber(char* str);

bool isdate(char* str);

bool isleap(int year);

hash_t hash(char* str, int power);

bool meets_criterias(BinFile* bfp, char** criterias, size_t csize, address_t recaddr, bool* result);

//	Adds a record to the file.
//	Checks that fname+lname IS NOT PRESENT YET in a file. (performs a find)
bool phonebook_add(BinFile * bfp, char ** input);

//	Removes a record from the file.
bool phonebook_remove(BinFile* bfp, char* fname, char* lname);

//	Enumeration for nedded information from phonebook_find(), so it can skip redundant job
typedef enum {
	fcCount = 1,
	fcAddresses,
	fcAddressesAndData,
} FindConfig;

//	Finds records in a file containing given strings (send NULL if some of them are not used)
//	ALLOCATES the buffer at *destptr.
//
//	MAKE SURE YOU FREE THE *destptr BUFFER IN A CALLER AFTER USE
//
//	@return A number of records found (0 if nothing)
bool phonebook_find(BinFile* bfp, char ** input, size_t input_size, Record** destptr, size_t *dest_size, FindConfig config);

////	Reads a record from the file at a given chain address. Writes it to dest.
//bool phonebook_read(BinFile* bfp, address_t addr, Record* dest);

void phonebook_print_recstr(char* str);

#endif PHONEBOOK_H