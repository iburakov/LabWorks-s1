#ifndef BINFILE_H
#define BINFILE_H
#include "..\App5\globals.h"

// All sizes defined in bytes
extern const char FILE_SIGNATURE[];
#define FILE_SIGNATURE_SIZE 8
#define BLOCK_DATA_SIZE(binfile) (binfile->block_size - 4)

// The free store overridden block ID to use like binfile_get_chain(FREE_STORE_ADDR)
#define FREE_STORE_ADDR -1


typedef int32_t address_t, hash_t;

typedef struct {
	FILE* fp;

	char signature[FILE_SIGNATURE_SIZE];
	int32_t block_size;
	int32_t hashtable_power;
} BinFile;

typedef struct {
	address_t next;
	char* data;
	size_t data_size;
} Block;

//	Reads a uint32_t from the file at current position
//	WARNING: A PURE FUNCTION WITH NO ERROR HANDLING
bool binfile_read_uint32(BinFile* bfp, uint32_t* dest);

//	Initializes bfp and reads header data from it.
bool binfile_load(BinFile* bfp, char* filename);

bool binfile_unload(BinFile* bfp);

//	Calculates a raw offset in the binary file of the data block with given address
//	WARNING: A PURE FUNCTION WITH NO ERROR HANDLING
size_t binfile_get_offset(BinFile* bfp, address_t addr);

//	Reads from binfile a raw block and copies its data to the destination Block structure
//	Pass FREE_STORE_ADDR as address to get the head of the free store chain
//
//	ASSUMES, THAT DATA BUFFER IN Block HAS BEEN ALLOCATED ALREADY
bool binfile_get_block(BinFile* bfp, address_t addr, Block* dest);


/*
	Reads a chain of blocks (chain of length 1 either), ALLOCATES the buffer of appropriate size,
	make *destptr point to it, and writes read data to the allocated buffer.

	MAKE SURE THAT YOU'VE FREED *destptr IN A CALLER FUNCTION AFTER USE
		 
	@param	destptr	A pointer to the holder of a pointer to the read data.
	@return			The amount of bytes written to the destination buffer.

	---
	Possible optimizations: it iterates over the blocks now in two passes. First is for size estimating,
	and the second one is for copying data. We could use realloc or linked list of blocks.
*/
size_t binfile_get_chain(BinFile* bfp, address_t headaddr, char** destptr);

/*
	Adds an address to the address collection chain (like in the free store or hashblocks)
	Ignores the call if it's already there.
*/
bool binfile_addrchain_add(BinFile* bfp, address_t chainaddr, address_t val);

//	Removes an address from the address collection chain (like in the free store or hashblocks)
//	Debug assertion is raised if it's not presented there. Ignores on release.
bool binfile_addrchain_remove(BinFile* bfp, address_t chainaddr, address_t val);

//	Tests an address for existance in the address collection chain (like in the free store or hashblocks)
bool binfile_addrchain_test(BinFile* bfp, address_t chainaddr, address_t val, bool* result);

//	Adds a block's address to the free store chain, so makes it rewritable.
//	BY FREEING A BLOCK YOU DISCARD THE DATA CONTAINED IN IT!
bool binfile_free_block(BinFile* bfp, address_t addr);

//	Adds all of the chain's block addresses to the free store chain, so destroys the chain and makes all the past members rewritable.
//	BY FREEING A CHAIN YOU DISCARD THE DATA CONTAINED IN IT!
bool binfile_free_chain(BinFile* bfp, address_t addr);

//	Aquires an address of a block that can be safely rewritten. Puts it to the *addr variable.
bool binfile_get_free_block(BinFile* bfp, address_t* addr);

//	Writes (rewrites) the block on headaddr with data. Extends or cuts the chain if needed. 
//	Appends written data with zeros till the end of the last block - important to addrchains to clean up the garbage.
bool binfile_write_chain(BinFile* bfp, address_t headaddr, char* data, long long data_size);

#endif