#include "FullyAssociativeCache.cpp"

class SetAssociativeCache :
	public Cache
{
public:
	std::vector<FullyAssociativeCache*> cache;

	int count = 0;

	bool write = false;

	SetAssociativeCache(int rows, int ways, int dbs)
		: Cache(rows)
	{
		dataBlockSize = dbs;

		for (int i = 0; i < rows; i++) {
			cache.push_back(new FullyAssociativeCache(ways, dbs));
		}

		// rows(4) * [ 2 * [1 + (16 - 3)  + 8*dbs(8) + LRU (2)]] = 648 bits
		int size = rows * ways * (1 + 16 - ((int)ceil(log2(dbs))) + 8*dbs + ((int)ceil(log2(ways))));

		printf("[SA] Size in bits is: %d\n", size);

		if (size > 840) toobig = true;

	}

	bool dataExistsInCache(int address) {

		// Dissect address
		int offset = address % (int)pow(2, ceil(log2(dataBlockSize))); // address % 2^bits_in_offset
		int upper = address / (int)pow(2, ceil(log2(dataBlockSize))); // address / 2^bits_in_offset
		int row = upper % (int)pow(2, ceil(log2(rows))); // upper % 2^bits_in_row
		int tag = upper / (int)pow(2, ceil(log2(rows))); // upper / 2^bits_in_row

		for (int i = 0; i < rows; i++) {
			if (cache[i]->dataExistsInCache(address)) {

				if (write) std::cout << "[SA] Hit!\tAddress: " << address << "\tRow: " << row << "\tTag: " << tag << std::endl;

				return true;

			}
		}

		if (write) std::cout << "[SA] Miss!\tAddress: " << address << "\tRow: " << row << "\tTag: " << tag << std::endl;

		return false;
	}

	void writeToCache(int address)
	{
		int offset = address % (int)pow(2, ceil(log2(dataBlockSize))); // address % 2^bits_in_offset
		int upper = address / (int)pow(2, ceil(log2(dataBlockSize))); // address / 2^bits_in_offset
		int row = upper % (int)pow(2, ceil(log2(rows))); // upper % 2^bits_in_row
		int tag = upper / (int)pow(2, ceil(log2(rows))); // upper / 2^bits_in_row

		cache[row]->writeToCache(address);
	}

	void printCacheState() {
		using namespace std;

		cout << "Row\tFA Cache" << endl;

		for (int i = 0; i < rows; i++) {
			cout << "Row " << i << endl;
			cache[i]->printCacheState();
		}
	}
};
