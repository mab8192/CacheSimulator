#pragma once

#include "Cache.cpp"

class FullyAssociativeCache :
	public Cache
{
public:

	bool write = false;

	std::vector<int> lru;

	int count = 0;

	FullyAssociativeCache(int rows, int dbs)
		: Cache(rows)
	{
		dataBlockSize = dbs;

		for (int i = 0; i < rows; i++) {
			lru.push_back(-1);
		}

		// rows (10) * [1 + (16 - 3)  + 8*dbs(8) + LRU (3)] = 810 bits
		int size = rows * ( 1 + 16 - ((int) ceil(log2(dbs))) + 8 * dbs + ((int)ceil(log2(rows))));

		printf("[FA] Size in bits is: %d\n", size);

		if (size > 840) toobig = true;
	}

	bool dataExistsInCache(int address) {

		// Dissect address
		int tag = floor(address / dataBlockSize);

		for (int i = 0; i < tags.size(); i++) {
			if (tags[i] == tag && valid[i]) {

				lru[i] = ++count;

				if (write) std::cout << "[FA] Hit!\tAddress: " << address << "\tTag: " << tag << std::endl;

				return true;
			}
		}

		if (write) std::cout << "[FA] Miss!\tAddress: " << address << "\tTag: " << tag << std::endl;

		return false;
	}

	void writeToCache(int address)
	{
		int tag = floor(address / dataBlockSize);
		
		int row = 0;
		int min = 1000000;

		// Find row with least recently used bit

		for (int i = 0; i < lru.size(); i++) {
			if (lru[i] < min) {
				row = i;
				min = lru[i];
			}
		}

		lru[row] = ++count;

		valid[row] = true;
		tags[row] = tag;
	}

	void printCacheState() {
		using namespace std;
		cout << "Valid\tTag\tLRU" << endl;

		for (int i = 0; i < rows; i++) {
			cout << valid[i] << "\t" << tags[i] << "\t" << lru[i] << endl;
		}
	}
};


