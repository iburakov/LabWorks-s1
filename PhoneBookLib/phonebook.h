#ifndef PHONEBOOK_H
#define PHONEBOOK_H
#include "binfile.h"

typedef struct {
	char* fname, lname, bdate, number;
} Record;

//			TO BE INLINED INTO phonebook_add() - possible optimizations noticed
////	Reads a record from the file and adds its headaddr to the hash-corresponding address chains:
////	Chain of the fname hash, lname hash, bdate hash and number hash.
//bool phonebook_index(BinFile* bfp, address_t headaddr);

//	Adds a record to the file.
//
//	Just forms the data, pushes it to write_chain(), and indexes it.
bool phonebook_add(BinFile* bfp, Record* rec);

//	Removes a record from the file.
//
//	First step: acquiring a headaddr by the fname and lname: - USE OF phonebook_find() IS POSSIBLE THERE.
//		1) constructing Record
//		2) evaluating lname hash, going through adresses looking for the name concurrences. 
//		3) when found, remember the address. delete it from the lname hashblock addrchain.
//	Second step: cleaning up
//		1) Reads a number and bdate from the file (we have addr, right?)
//			we'll always have to read the whole record, that's why we're able to invoke phonebook_find() there.
//		2) Evaluates fname, number and bdate hashes
//		3) Frees the chain by headaddr, and removes the headaddr from corresponding hashblocks.
bool phonebook_remove(BinFile* bfp, char* fname, char* lname);

//	Finds a record in a file containing given strings (send NULL if some of them are not used)
//
//	@return FAILURE if not found, chain's head address otherwise.
//
//	See phonebook_remove's find for implementation. Move it here.
address_t phonebook_find(BinFile* bfp, char* fname, char* lname, char* bdate, char* num);

//	Reads a record from the file at a given chain address. Writes it to dest.
bool phonebook_read(BinFile* bfp, address_t addr, Record* dest);

#endif PHONEBOOK_H