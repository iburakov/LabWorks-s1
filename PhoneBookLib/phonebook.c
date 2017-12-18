#include "..\App5\globals.h"
#include "..\App5\input.h"
#include "phonebook.h"

bool isnumber(char * str) {
	if (str[0] != '+') return FALSE;
	int i = 1;
	while (str[i] != '\0') {
		if (!isdigit(str[i++])) return FALSE;
	}
	return i != 1;
}

bool isdate(char * str) {
	if (isdigit(str[0]) && isdigit(str[1]) && str[2] == '.' &&
		isdigit(str[3]) && isdigit(str[4]) && str[5] == '.' &&
		isdigit(str[6]) && isdigit(str[7]) && isdigit(str[8]) && isdigit(str[9]) &&
		str[10] == '\0') {

		int day = (str[0] - '0') * 10 + str[1] - '0';
		int month = (str[3] - '0') * 10 + str[4] - '0';
		int year = str[6] * 1000 + str[7] * 100 + str[8] * 10 + str[9] - '0' * 1111;
		int feb = (isleap(year)) ? 29 : 28;
		int month_days[] = { 31, feb, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		return (month > 0 && month <= 12 && day > 0 && day <= month_days[month - 1] && year > 0);
	}
	else return FAILURE;
}

bool isleap(int year) {
	if (year % 400 == 0) return TRUE;
	else if (year % 100 == 0) return FALSE;
	else return (year % 4 == 0);
}

hash_t hash(char * str, int power) {
	hash_t hash = 0;
	int i = 0;
	while (str[i] != '\0') hash = ((((uint64_t)(hash + str[i++])) * 691 /* 113 */)) % power;
	return hash;
}

bool meets_criterias(BinFile* bfp, char ** criterias, size_t csize, address_t recaddr, bool * result) {
	char* recstr;
	if (!binfile_get_chain(bfp, recaddr, &recstr)) return FAILURE;
	
	for (int criti = 0; criti < csize; ++criti) {
		int i = 0;
		bool meets = FALSE;

		for (int fieldi = 0; fieldi < 4; ++fieldi) {
			if (strcmp(criterias[criti], recstr + i) == 0) {
				meets = TRUE;
				break;
			}
			if (fieldi != 3) {
				while (recstr[i] != '\0') ++i;
				++i;
			}			
		}

		if (!meets) {
			free(recstr);
			*result = FALSE;
			return SUCCESS;
		}
	}

	free(recstr);
	*result = TRUE;
	return SUCCESS;
}

bool phonebook_add(BinFile * bfp, char ** input) {
	size_t found = 0;
	if (!phonebook_find(bfp, input, 2, NULL, &found, fcCount)) return FAILURE;
	if (found) {
		ERROR = errTechnical;
		ERRSTR = "A record with the same key already exists.";
		return FAILURE;
	}

	size_t len[4] = { strlen(input[0]), strlen(input[1]), strlen(input[2]), strlen(input[3]) };
	size_t data_size = len[0] + len[1] + len[2] + len[3] + 4;
	char* data = malloc(data_size * sizeof(char));
	int soffset = 0;
	for (int s = 0; s < 4; s++) {
		for (int i = 0; i < len[s]; ++i) data[soffset + i] = input[s][i];
		soffset += len[s];
		data[soffset++] = '\0';
	}

	address_t newb;
	if (!binfile_get_free_block(bfp, &newb)) return FAILURE;
	if (!binfile_write_chain(bfp, newb, data, data_size)) return FAILURE;

	for (int i = 0; i < 4; ++i) {
		if (!binfile_addrchain_add(bfp, hash(input[i], bfp->hashtable_power), newb, TRUE));
	}

	free(data);
	return SUCCESS;
}

bool phonebook_remove(BinFile * bfp, char * fname, char * lname) {
	// finding a record to delete and acquiring its address and fields from the file
	char* mask[] = { fname, lname };
	size_t recs_size;
	Record* recs;
	if (!phonebook_find(bfp, mask, 2, &recs, &recs_size, fcAddressesAndData)) return FAILURE;

	if (!recs_size) {
		ERROR = errTechnical;
		ERRSTR = "";
		printf("No record to delete has been found with key \"%s %s\".\n", fname, lname);
		for (int i = 0; i < recs_size; ++i) free(recs[i].str);
		free(recs);
		return FAILURE;
	}

	if (recs_size > 1) {
		ERROR = errTechnical;
		ERRSTR = "";
		printf("Error in file structure! Found %d records to delete with key \"%s %s\".\n", recs_size, fname, lname);
		for (int i = 0; i < recs_size; ++i) free(recs[i].str);
		free(recs);
		return FAILURE;
	}

	printf("Are you sure you want to remove the following record? [y/n]\n");
	phonebook_print_recstr(recs->str);
	if (!user_agrees()) {
		ERROR = errNo;
		ERRSTR = "";
		for (int i = 0; i < recs_size; ++i) free(recs[i].str);
		free(recs);
		return FAILURE;
	}


	// evaluating hashes
	int power = bfp->hashtable_power;
	hash_t hashes[4] = { hash(fname, power), hash(lname, power), 0, 0 };

	int i = 0;
	while (recs->str[i] != '\0') ++i;
	++i;
	while (recs->str[i] != '\0') ++i;
	++i;
	hashes[2] = hash(recs->str + i, power);
	while (recs->str[i] != '\0') ++i;
	++i;
	hashes[3] = hash(recs->str + i, power);

	// removing addresses from index (corresponding hashtable blocks)
	for (int h = 0; h < 4; ++h) {
		if (!binfile_addrchain_remove(bfp, hashes[h], recs->addr)) {
			for (int i = 0; i < recs_size; ++i) free(recs[i].str);
			free(recs);
			return FAILURE;
		}
	}

	// freeing the chain on record's address
	if (!binfile_free_chain(bfp, recs->addr)) {
		for (int i = 0; i < recs_size; ++i) free(recs[i].str);
		free(recs);
		return FAILURE;
	}

	for (int i = 0; i < recs_size; ++i) free(recs[i].str);
	free(recs);
	return SUCCESS;
}

bool phonebook_find(BinFile * bfp, char ** input, size_t input_size, Record ** destptr, size_t * dest_size, FindConfig config) {
	address_t** chains = malloc(sizeof(address_t*) * input_size);

	if (!chains) {
		ERROR = errFatal;
		ERRSTR = "Couldn't allocate memory in phonebook_find()!";
		return FAILURE;
	}

	int* chain_sizes = malloc(sizeof(int) * input_size);

	if (!chain_sizes) {
		free(chains);
		ERROR = errFatal;
		ERRSTR = "Couldn't allocate memory in phonebook_find()!";
		return FAILURE;
	}

	// gathering hashblock chains and their sizes
	for (int i = 0; i < input_size; ++i) {
		if (!(chain_sizes[i] = binfile_get_chain(bfp, hash(input[i], bfp->hashtable_power), chains + i) / sizeof(address_t))) {
			for (int j = 0; j < i; ++j) free(chains[j]);
			free(chains);
			free(chain_sizes);
			return FAILURE;
		}
	}

	// finding the shortest chain i
	int32_t minsz = INT32_MAX;
	int shortest = 0;
	for (int i = 0; i < input_size; ++i) {
		if (chain_sizes[i] < minsz) {
			shortest = i;
			minsz = chain_sizes[i];
		}
	}

	// eliminating uncommon addersses from the shortest chain (finding union of 4 chains -> shortest)
	for (int i = 0; i < chain_sizes[shortest]; ++i) {
		address_t cur = chains[shortest][i];
		if (!cur) continue; //break? - if no blanks in chain guaranteed

		for (int c = 0; c < input_size; ++c) {
			if (c == shortest) continue;

			bool found = FALSE;
			for (int j = 0; j < chain_sizes[c]; ++j) {
				if (chains[c][j] == cur) {
					found = TRUE;
					break;
				}
				// if (!chains[c][j]) break; - if no blanks in chain guaranteed
			}
			if (!found) {
				chains[shortest][i] = 0;
				break;
			}
		}
	}

	// second pass of filtering: comparing with real records from file and counting the result
	size_t count = 0;
	for (int i = 0; i < chain_sizes[shortest]; ++i) {
		if (chains[shortest][i]) {
			bool meets;
			if (!meets_criterias(bfp, input, input_size, chains[shortest][i], &meets)) {
				for (int i = 0; i < input_size; ++i) free(chains[i]);
				free(chain_sizes);
				free(chains);
				return FAILURE;
			}
			if (meets) ++count;
			else chains[shortest][i] = 0;
		}
	}

	*dest_size = count;

	// gathering data if needed
	if (config != fcCount) {
		*destptr = malloc(sizeof(Record) * count);
		int to_set = 0;
		for (int i = 0; i < chain_sizes[shortest]; ++i) {
			if (chains[shortest][i]) {
				Record rec;
				rec.addr = chains[shortest][i];
				rec.str = NULL;
				if (config != fcAddresses) {
					// reading data from the record by address
					if (!binfile_get_chain(bfp, rec.addr, &rec.str)) {
						for (int i = 0; i < input_size; ++i) free(chains[i]);
						free(chain_sizes);
						free(chains);
						return FAILURE;
					}
				}

				(*destptr)[to_set++] = rec;
			}
		}
	}

	for (int i = 0; i < input_size; ++i) free(chains[i]);
	free(chain_sizes);
	free(chains);
	return SUCCESS;
}

void phonebook_print_recstr(char * str) {
	const space = 4;
	int ri = 0;
	int i = 0;

	// print fname
	while (str[i] != '\0') {
		if (i < REC_TABLE_KEY_SPACE_FILL - space) {
			putchar(str[i++]);
			++ri;
		}
		else ++i;
	}
	if (i < REC_TABLE_KEY_SPACE_FILL - space) {
		++ri;
		putchar(' ');
	}
	++i;

	// print lname
	while (str[i] != '\0') {
		if (i < REC_TABLE_KEY_SPACE_FILL - space) {
			putchar(str[i++]);
			++ri;
		}
		else ++i;
	}
	++i;

	// spacing
	if (++ri < i) {
		++ri;
		putchar('~');
	}
	while (++ri < REC_TABLE_KEY_SPACE_FILL) putchar(' ');

	// print bdate
	while (str[i] != '\0') putchar(str[i++]);
	putchar('\t');
	putchar('\t');
	++i;

	// print number
	while (str[i] != '\0') putchar(str[i++]);
	putchar('\n');
}

