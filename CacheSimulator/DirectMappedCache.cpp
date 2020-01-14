#include "Cache.cpp"

class DirectMappedCache :
	public Cache
{
public:

	bool write = false;

	DirectMappedCache(int rows, int dbs)
		: Cache(rows)
	{
		dataBlockSize = dbs; // Requires a total of 600 bits

		// rows(8)* [valid(1) + tag(16 - 3 - 3) + 8 * dbs(8)] = 600 bits
		int size = rows * (1 + 16 - ((int)ceil(log2(dbs))) - ((int)ceil(log2(rows))) + 8 * dbs);

		printf("[DM] Size in bits is: %d\n", size);

		if (size > 840) toobig = true;
	}

	bool dataExistsInCache(int address) {

		// Dissect address
		int offset = address % (int)pow(2, ceil(log2(dataBlockSize))); // address % 2^bits_in_offset
		int upper = address / (int)pow(2, ceil(log2(dataBlockSize))); // address / 2^bits_in_offset
		int row = upper % (int)pow(2, ceil(log2(rows))); // upper % 2^bits_in_row
		int tag = upper / (int)pow(2, ceil(log2(rows))); // upper / 2^bits_in_row

		if (valid[row] && tags[row] == tag)
		{
			if (write) std::cout << "[DM] Hit!\tAddress: " << address << "\tRow: " << row << "\tTag: " << tag << std::endl;
			return true;
		}

		if (write) std::cout << "[DM] Miss!\tAddress: " << address << "\tRow: " << row << "\tTag: " << tag << std::endl;

		return false;
	}

	void writeToCache(int address) 
	{
		int offset = address % (int)pow(2, ceil(log2(dataBlockSize))); // address % 2^bits_in_offset
		int upper = address / (int)pow(2, ceil(log2(dataBlockSize))); // address / 2^bits_in_offset
		int row = upper % (int)pow(2, ceil(log2(rows))); // upper % 2^bits_in_row
		int tag = upper / (int)pow(2, ceil(log2(rows))); // upper / 2^bits_in_row
		
		valid[row] = true;
		tags[row] = tag;
	}

	void printCacheState() {
		using namespace std;
		cout << "Row\tValid\tTag" << endl;

		for (int i = 0; i < rows; i++) {
			cout << i << "\t" << valid[i] << "\t" << tags[i] << endl;
		}
	}
};