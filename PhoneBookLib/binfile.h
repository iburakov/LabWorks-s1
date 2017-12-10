#ifndef BINFILE_H
#define BINFILE_H
#include "..\App5\globals.h"

extern error_t pbERROR;
extern char* pbERRSTR;

// All sizes defined in bytes
#define FILE_SIGNATURE "LAB5.BIN"
#define FILE_SIGNATURE_SIZE 8

// The free store overridden block ID to use like binfile_get_chain(FREE_STORE)
#define FREE_STORE 0


typedef uint32_t address_t, hash_t;

typedef struct {
	FILE* fp;

	char signature[FILE_SIGNATURE_SIZE];
	uint32_t block_size;
	uint32_t hashtable_power;
} BinFile;

typedef struct {
	address_t next;
	char* data;
	size_t data_size;
} Block;


//	Initializes bfp and reads header data from it.
bool binfile_load(BinFile* bfp);

//	Calculates a raw offset in the binary file of the data block with given address
//	@return	FAILURE on failure.
size_t binfile_get_offset(BinFile* bfp, address_t addr);

//	Calculates a raw offset in the binary file of the block in a hashtable that is responsible for a given hash.
//	@return	FAILURE on failure.
size_t binfile_get_hashblock_offset(BinFile* bfp, hash_t hash);

//	Reads from binfile a raw block and copies its data the destination Block
//	Pass FREE_STORE as address to get the head of the free store chain
bool binfile_get_block(BinFile* bfp, address_t addr, Block* dest);


/*
	Reads a chain of blocks (chain of length 1 either), ALLOCATES the buffer of appropriate size,
	make *destptr point to it, and writes read data to the allocated buffer.

	MAKE SURE THAT YOU'VE FREED *destptr IN A CALLER FUNCTION AFTER USE
		 
	@param	destptr	A pointer to the holder of a pointer to the read data.
	@return			The amount of bytes written to the destination buffer.
*/
size_t binfile_get_chain(BinFile* bfp, address_t headaddr, char** destptr);

/*
	Adds an address to the address collection chain (like in the free store or hashblocks)
	Ignores the call if it's already there.

	Loads the chain to RAM from the file.
	Iterates over ALL its elements to check the existance, and
	Writes val to a zero address_t in the stream.
	(Re)writes back the chain at a given head. Cutting and extending should have been implemented there already.
*/
bool binfile_addrchain_add(BinFile* bfp, address_t chainaddr, address_t val);

//	Removes an address from the address collection chain (like in the free store or hashblocks)
//	Debug assertion is raised if it's not presented there. Ignores on release.
//
//	Operates like adding.
bool binfile_addrchain_remove(BinFile* bfp, address_t chainaddr, address_t val);

//	Tests an address for existance in the address collection chain (like in the free store or hashblocks)
//
//	Iterates over chain DIRECTLY IN A FILE and immediately returns TRUE if found or FALSE at the end of the chain.
bool binfile_addrchain_test(BinFile* bfp, address_t chainaddr, address_t val);

//	Adds a block's address to the free store chain, so makes it rewritable.
//	BY FREEING A BLOCK YOU DISCARD THE DATA CONTAINED IN IT!
bool binfile_free_block(BinFile* bfp, address_t addr);

//	Adds all of the chain's block addresses to the free store chain, so destroys the chain and makes all the past members rewritable.
//	BY FREEING A CHAIN YOU DISCARD THE DATA CONTAINED IN IT!
bool binfile_free_chain(BinFile* bfp, address_t addr);

//	Aquires an address of a block that can be safely rewritten. Puts it to the *addr variable.
//
//	First, checks the free store. If it've found a block there, returs block's address,
//	and removes it from the free store, so the block is protected from writing again.
//	If the free store is empty, appends a new block to the end of the file and returns its address.
bool binfile_get_free_block(BinFile* bfp, address_t* addr);

//	Writes (rewrites) the block on headaddr with data. Extends or cuts the chain if needed. 
//	Appends written data with zeros till the end of the last block - important to addrchains to clean up the garbage.
//
//	Extension: estimate whether the remaining data will fit into the current block or not.
//	If another block is needed, get_ another _free_block(), write aquired address to block's "next", write the data and fseek() to the block.
//
//	Cutting: If all data has been written already, but there are still some next blocks in the chain, call binfile_free_chain() 
//	for the first unwanted block. Set next of the previous to the NULL.
bool binfile_write_chain(BinFile* bfp, address_t headaddr, char* data, size_t data_size);

//////	ALLOCATION OF A FREE BLOCK SHOULD BE HANDLED BY CALLER OF binfile_write_chain() NOW: ADDRESS MAY BE NEEDED FURTHER
////	Sets up a chain and writes data to it.
////
////	Aquires an address of ready-for-safe-writing block and writes data to it, 
////	because writing to a block supports extension to a chain.
//bool binfile_add_chain(BinFile* bfp, char* data, size_t data_size);

#endif