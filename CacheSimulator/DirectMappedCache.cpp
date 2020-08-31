#include "Cache.cpp"

class DirectMappedCache :
	public Cache
{
public:

	bool write = false;

	/// <summary>
	/// Construct a new direct-mapped cache.
	/// </summary>
	/// <param name="rows">The number of sets.</param>
	/// <param name="dbs">The size of blocks in bytes.</param>
	DirectMappedCache(int sets, int dbs)
		: Cache(sets)
	{
		dataBlockSize = dbs;

		int size = rows * (1 + 16 - ((int)ceil(log2(dbs))) - ((int)ceil(log2(rows))) + 8 * dbs);

		printf("[DM] Size in bits is: %d\n", size);
	}

	bool dataExistsInCache(int address) {

		// Dissect address
		int offset = address % (int)pow(2, ceil(log2(dataBlockSize))); // address % 2^bits_in_offset
		int upper = address / (int)pow(2, ceil(log2(dataBlockSize))); // address / 2^bits_in_offset
		int row = upper % (int)pow(2, ceil(log2(rows))); // upper % 2^bits_in_row
		int tag = upper / (int)pow(2, ceil(log2(rows))); // upper / 2^bits_in_row

		if (valid[row] && tags[row] == tag)
		{
			if (write) std::cout << "[DM] Hit!\tAddress: " << address << "\tTag: " << tag << "\tSet: " << row << "\tOffset: " << offset << std::endl;
			return true;
		}

		if (write) std::cout << "[DM] Miss!\tAddress: " << address << "\tTag: " << tag << "\tSet: " << row << "\tOffset: " << offset << std::endl;

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