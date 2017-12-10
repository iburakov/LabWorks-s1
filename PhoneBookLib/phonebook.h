#ifndef PHONEBOOK_H
#define PHONEBOOK_H
#include "binfile.h"

//	 All functions declared in "phonebook.h" and "binfile.h" support error handling by using these global variables:
//			error_t ERROR
//			char* ERRSTR

typedef struct {
	address_t addr;
	char* fname, lname, bdate, number;
} Record;

//			TO BE INLINED INTO phonebook_add() - possible optimizations noticed - done
////	Reads a record from the file and adds its headaddr to the hash-corresponding address chains:
////	Chain of the fname hash, lname hash, bdate hash and number hash.
//bool phonebook_index(BinFile* bfp, address_t headaddr);

//	Adds a record to the file.
//	Checks that fname+lname IS NOT PRESENT YET in a file. (performs a find)
//
//	Just forms the data, aquires free block, pushes it to write_chain(), and indexes it:
//	Evaluates 4 hashes, and adds the address address chains at 4 corresponding hashblocks
bool phonebook_add(BinFile* bfp, Record* rec);

//	Removes a record from the file.
//
//	First step: acquiring a headaddr and record data by the fname and lname: - USE OF phonebook_find() IS POSSIBLE THERE.
//			we'll always have to read the whole record, that's why we're able to invoke phonebook_find() there.
//			config is fcAddressesAndData
//		2) Evaluates fname, number and bdate hashes
//		3) Frees the chain by headaddr, and removes the headaddr from corresponding hashblocks.
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
//
//	Evaluating fname, lname, bdate, and num hashes.
//	Selecting the "main" hash (evaluating 4 hashes, gathering their addrchain from corresponding hashblocks and selecting the shortest)
//	Iterating over the shortest addrchain and checking for the rest criterias to be satisfied by iterating over the rest addrchains
//		1) First pass: counting common addresses
//	IF config == fcCount || count == 0 RETURN count;
//		2) Allocating memory for records
//		3) Second pass: gathering adresses, setting them to records
//	IF CONFIG == fcAddresses RETURN
//	Resolving addresses into records
size_t phonebook_find(BinFile* bfp, char* fname, char* lname, char* bdate, char* num, Record** destptr, FindConfig config);

//	Reads a record from the file at a given chain address. Writes it to dest.
bool phonebook_read(BinFile* bfp, address_t addr, Record* dest);

#endif PHONEBOOK_H