#include "binfile.h"

const char FILE_SIGNATURE[] = { 'L','A','B','5','.','B','I','N' };

bool binfile_read_uint32(BinFile * bfp, uint32_t * dest) {
	return fread_s(dest, sizeof(uint32_t), sizeof(uint32_t), 1, bfp->fp) == 1;
}

bool binfile_load(BinFile * bfp, char* filename) {
	errno_t error = fopen_s(&(bfp->fp), filename, "rb+");
	if (!bfp->fp) {
		ERROR = errTechnical;
		ERRSTR = "";
		if (error == ENOENT) {
			printf("File \"%s\" not found.\n", filename);
			return FAILURE;
		}
		else {
			char errbuf[TOKEN_SIZE];
			strerror_s(errbuf, TOKEN_SIZE, error);
			printf("File \"%s\" can't be read. %s.\n", filename, errbuf);
			return FAILURE;
		}
	}

	char signbuf[FILE_SIGNATURE_SIZE];
	size_t read = fread_s(signbuf, FILE_SIGNATURE_SIZE, sizeof(char), FILE_SIGNATURE_SIZE, bfp->fp);
	if (read != FILE_SIGNATURE_SIZE) {
		ERROR = errTechnical;
		ERRSTR = "File is corrupted: couldn't read a signature.";
		if (!binfile_unload(bfp)) {
			printf("During handling the exception \"%s\", another one occurred.\n", ERRSTR);
			return FAILURE;
		}
		return FAILURE;
	}

	if (memcmp(signbuf, FILE_SIGNATURE, FILE_SIGNATURE_SIZE) != 0) {
		ERROR = errTechnical;
		ERRSTR = "File has wrong format or corrupted: different signature.";
		if (!binfile_unload(bfp)) {
			printf("During handling the exception \"%s\", another one occurred.\n", ERRSTR);
			return FAILURE;
		}
		return FAILURE;
	}

	if (!binfile_read_uint32(bfp, &bfp->block_size) || !binfile_read_uint32(bfp, &bfp->hashtable_power)) {
		binfile_unload(bfp);
		ERROR = errTechnical;
		ERRSTR = "File is corrupted: coulnd't read its header.";
		return FAILURE;
	}

	return SUCCESS;
}

bool binfile_unload(BinFile * bfp) {
	if (!bfp->fp) {
		ERROR = errTechnical;
		ERRSTR = "Couldn't close a file: nothing is opened yet.";
		return FAILURE;
	}

	strcpy_s(bfp->signature, FILE_SIGNATURE_SIZE, "\0\0\0\0\0\0\0");
	bfp->block_size = 0;
	bfp->hashtable_power = 0;
	if (fclose(bfp->fp) == EOF) {
		ERROR = errFatal;
		ERRSTR = "Couldn't close a file properly!";
		return FAILURE;
	}
	bfp->fp = NULL;
	return SUCCESS;
}

size_t binfile_get_offset(BinFile * bfp, address_t addr) {
	return (addr == FREE_STORE_ADDR) ? 0x10 : 0x50 + (addr) * bfp->block_size;
}

bool binfile_get_block(BinFile * bfp, address_t addr, Block * dest){
	size_t offset = binfile_get_offset(bfp, addr);
	fseek(bfp->fp, offset, SEEK_SET);
	
	if (!binfile_read_uint32(bfp, &dest->next)) {
		ERROR = errTechnical;
		printf("Corruption! Couldn't read a block at address %d, offset %d\n", addr, offset);
		return FAILURE;
	}

	size_t read = fread_s(dest->data, sizeof(char) * BLOCK_DATA_SIZE(bfp), sizeof(char), sizeof(char) * BLOCK_DATA_SIZE(bfp), bfp->fp);
	if (read != sizeof(char) * BLOCK_DATA_SIZE(bfp)) {
		ERROR = errTechnical;
		printf("Corruption! Couldn't read a block at address %d, offset %d\n", addr, offset);
		return FAILURE;
	}
	return SUCCESS;
}

size_t binfile_get_chain(BinFile * bfp, address_t headaddr, char ** destptr) {
	Block cb;
	cb.data_size = BLOCK_DATA_SIZE(bfp);
	cb.data = (char*)malloc(sizeof(char) * cb.data_size);
	if (!cb.data) {
		ERROR = errTechnical;
		ERRSTR = "Could not allocate memory in RAM for a block!";
		return FAILURE;
	}
	
	size_t dest_size = 0;
	address_t curaddr = headaddr;
	do {
		if (!binfile_get_block(bfp, curaddr, &cb)) {
			free(cb.data);
			return FAILURE;
		}
		curaddr = cb.next;
		dest_size += cb.data_size;
	} while (cb.next);

	*destptr = (char*)malloc(dest_size * sizeof(char));
	if (!*destptr) {
		ERROR = errTechnical;
		ERRSTR = "";
		printf("Could not allocate memory in RAM for data of a chain with head at %d\n", headaddr);
		return FAILURE;
	}

	curaddr = headaddr;
	for (int i = 0; i < dest_size / cb.data_size; ++i) {
		if (!binfile_get_block(bfp, curaddr, &cb)) return FAILURE;
		errno_t err = memcpy_s(*destptr + i*cb.data_size, cb.data_size, cb.data, cb.data_size);
		curaddr = cb.next;
	}
	free(cb.data);
	return dest_size;
}

bool binfile_addrchain_add(BinFile * bfp, address_t chainaddr, address_t val) {
	char* chaindata_raw;
	size_t chaindata_size;
	if (!(chaindata_size = binfile_get_chain(bfp, chainaddr, &chaindata_raw))) return FAILURE;

	assert(chaindata_size % sizeof(address_t) == 0);
	address_t* chaindata = (address_t*)chaindata_raw;
	int first_null_i = -1;
	for (int i = 0; i < chaindata_size / sizeof(address_t); ++i) {
		if (chaindata[i] == val) {
			free(chaindata);
			return SUCCESS;
		}
		if (first_null_i == -1 && chaindata[i] == 0) first_null_i = i;
	}
	if (first_null_i != -1) chaindata[first_null_i] = val;
	else {
		if (!(chaindata = realloc(chaindata, chaindata_size + sizeof(address_t)))) {
			ERROR = errFatal;
			ERRSTR = "Couldn't reallocate memory in binfile_addchain_add() to add new block to the chain!";
			return FAILURE;
		}
		chaindata[chaindata_size / sizeof(address_t)] = val;
		chaindata_size += sizeof(address_t);
	}

	if (!binfile_write_chain(bfp, chainaddr, chaindata, chaindata_size)) {
		free(chaindata);
		return FAILURE;
	}

	free(chaindata);
	return SUCCESS;
}

bool binfile_addrchain_remove(BinFile * bfp, address_t chainaddr, address_t val) {
	char* chaindata_raw;
	size_t chaindata_size;
	if (!(chaindata_size = binfile_get_chain(bfp, chainaddr, &chaindata_raw))) return FAILURE;

	assert(chaindata_size % sizeof(address_t) == 0);
	address_t* chaindata = (address_t*)chaindata_raw;

	int written_i = -1;
	int last_non_zero_i = -1;
	int actual_size = 0;
	for (int i = 0; i < chaindata_size / sizeof(address_t); ++i) {
		if (chaindata[i] == val) {
			if (written_i != -1) {
				ERROR = errFatal;
				ERRSTR = "";
				printf("File is corrupted: multiple address entries in address chain at %d", chainaddr);
				free(chaindata);
				return FAILURE;
			}

			chaindata[i] = 0;
			written_i = i;
		}
		else if (chaindata[i] != 0) {
			last_non_zero_i = i;
		}
		actual_size = i + 1;
	}

	if (last_non_zero_i != -1) {
		chaindata[written_i] = chaindata[last_non_zero_i];
		chaindata[last_non_zero_i] = 0;
	}

	if (!binfile_write_chain(bfp, chainaddr, chaindata_raw, actual_size)) return FAILURE;

	free(chaindata);
	return SUCCESS;
}

bool binfile_addrchain_test(BinFile * bfp, address_t chainaddr, address_t val, bool* result) {
	char* chaindata_raw;
	size_t chaindata_size;
	if (!(chaindata_size = binfile_get_chain(bfp, chainaddr, &chaindata_raw))) return FAILURE;

	*result = FALSE;

	assert(chaindata_size % sizeof(address_t) == 0);
	address_t* chaindata = (address_t*)chaindata_raw;
	for (int i = 0; i < chaindata_size / sizeof(address_t); ++i) {
		if (chaindata[i] == val) {
			*result = TRUE;
			break;
		}
	}
	free(chaindata);
	return SUCCESS;
}

bool binfile_free_block(BinFile * bfp, address_t addr) {
	fseek(bfp->fp, binfile_get_offset(bfp, addr), SEEK_SET);
	address_t next = 0;
	fwrite(&next, sizeof(address_t), 1, bfp->fp);
	return binfile_addrchain_add(bfp, FREE_STORE_ADDR, addr);
}

bool binfile_free_chain(BinFile * bfp, address_t addr) {
	Block cb;
	cb.data_size = BLOCK_DATA_SIZE(bfp);
	cb.data = (char*)malloc(sizeof(char) * cb.data_size);
	if (!cb.data) {
		ERROR = errTechnical;
		ERRSTR = "Could not allocate memory in RAM for a block!";
		return FAILURE;
	}

	address_t curaddr = addr;
	do {
		if (!binfile_get_block(bfp, curaddr, &cb) || !binfile_free_block(bfp, curaddr)) {
			free(cb.data);
			return FAILURE;
		}
		curaddr = cb.next;
	} while (cb.next);

	free(cb.data);
	return SUCCESS;
}

bool binfile_get_free_block(BinFile * bfp, address_t * addr){
	size_t start_ofs = ftell(bfp->fp);

	char* chaindata_raw;
	size_t chaindata_size;
	if (!(chaindata_size = binfile_get_chain(bfp, FREE_STORE_ADDR, &chaindata_raw))) return FAILURE;

	assert(chaindata_size % sizeof(address_t) == 0);
	address_t* chaindata = (address_t*)chaindata_raw;
	bool found = FALSE;
	for (int i = 0; i < chaindata_size / sizeof(address_t); ++i) {
		if (chaindata[i] != 0) {
			*addr = chaindata[i];
			if (!binfile_addrchain_remove(bfp, FREE_STORE_ADDR, chaindata[i])) return FAILURE; // possible optimization: pass i, not arr[i]. wait for i, not for arr[i].
			found = TRUE;
			break;
		}
	}

	if (!found) {
		fseek(bfp->fp, 0, SEEK_END);
		assert((ftell(bfp->fp) - 0x50) % bfp->block_size == 0);
		*addr = (ftell(bfp->fp) - 0x50) / bfp->block_size;
		for (int i = 0; i < bfp->block_size; ++i) fputc('\0', bfp->fp);
	}
	
	fseek(bfp->fp, start_ofs, SEEK_SET);
	free(chaindata);
	return SUCCESS;
}

bool binfile_write_chain(BinFile * bfp, address_t headaddr, char * data, long long data_size) {
	address_t curaddr = headaddr;
	long long written = 0;

	fseek(bfp->fp, binfile_get_offset(bfp, headaddr), SEEK_SET);
	address_t nextaddr;
	if (!binfile_read_uint32(bfp, &nextaddr)) {
		ERROR = errTechnical;
		ERRSTR = "";
		printf("Corruption! While writing to block with address of %d couldn't gather next address from it.", curaddr);
		return FAILURE;
	}

	while (written < data_size - BLOCK_DATA_SIZE(bfp)) {
		if (!nextaddr) {
			if (!binfile_get_free_block(bfp, &nextaddr)) return FAILURE;

			fseek(bfp->fp, -(int)sizeof(address_t), SEEK_CUR);
			fwrite(&nextaddr, sizeof(address_t), 1, bfp->fp);
		}
		
		fseek(bfp->fp, 0, SEEK_CUR);
		fwrite(data + written, sizeof(char), BLOCK_DATA_SIZE(bfp), bfp->fp);
		written += BLOCK_DATA_SIZE(bfp);
		
		curaddr = nextaddr;
		fseek(bfp->fp, binfile_get_offset(bfp, curaddr), SEEK_SET);
		if (!binfile_read_uint32(bfp, &nextaddr)) return FAILURE;
	} 

	fseek(bfp->fp, binfile_get_offset(bfp, curaddr) + sizeof(address_t), SEEK_SET);
	fwrite(data + written, sizeof(char), data_size - written, bfp->fp);
	for (int i = 0; i < BLOCK_DATA_SIZE(bfp) - (data_size - written); ++i) fputc('\0', bfp->fp);

	if (nextaddr) {
		if (!binfile_free_chain(bfp, nextaddr)) return FAILURE;

		nextaddr = 0;
		fseek(bfp->fp, binfile_get_offset(bfp, curaddr), SEEK_SET);
		fwrite(&nextaddr, sizeof(address_t), 1, bfp->fp);
	}

	return SUCCESS;
}