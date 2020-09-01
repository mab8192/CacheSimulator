#include <iostream>

#include "DirectMappedCache.cpp"
#include "FullyAssociativeCache.cpp"
#include "SetAssociativeCache.cpp"

using namespace std;

int main() {

	int addresses[] = { 1,65,132,19,0,64,131,1 };
	int length = sizeof(addresses) / sizeof(int);

	FullyAssociativeCache* fa = new FullyAssociativeCache(10, 8);
	DirectMappedCache* dm = new DirectMappedCache(4, 4);
	SetAssociativeCache* sa = new SetAssociativeCache(2, 2, 4);

	fa->write = false;
	dm->write = false;
	sa->write = true;

	for (int j = 0; j < 2; j++) {

		for (int i = 0; i < length; i++)
		{
			int address = addresses[i];

			// FULLY ASSOCIATIVE
			if (!fa->dataExistsInCache(address)) {
				if (j != 0) fa->misses++; // Only record misses after the first iteration
				fa->writeToCache(address);
			}
			else
			{
				if (j != 0) fa->hits++; // Only record hits after the first iteration
			}

			// DIRECT MAPPED
			if (!dm->dataExistsInCache(address)) {
				if (j != 0) dm->misses++; // Only record misses after the first iteration
				dm->writeToCache(address);
			}
			else
			{
				if (j != 0) dm->hits++; // Only record hits after the first iteration
			}

			// SET ASSOCIATIVE
			if (!sa->dataExistsInCache(address)) {
				if (j != 0) sa->misses++; // Only record misses after the first iteration
				sa->writeToCache(address);
			}
			else
			{
				if (j != 0) sa->hits++; // Only record hits after the first iteration
			}
		}

		if (j == 0) cout << "\nEND OF FIRST LOOP\n\n";
	}
	

	// Print Summary

	cout << endl << "FULLY ASSOCIATIVE CACHE PERFORMANCE" << endl;
	cout << "Misses: " << fa->misses << ", Hits: " << fa->hits << endl;
	cout << "Average CPI: " << ((float) fa->hits + fa->misses * (1 + 10 + fa->dataBlockSize)) / length << endl;

	cout << endl << "DIRECT MAPPED CACHE PERFORMANCE" << endl;
	cout << "Misses: " << dm->misses << ", Hits: " << dm->hits << endl;
	cout << "Average CPI: " <<  ((float) dm->hits + dm->misses * (1 + 10 + dm->dataBlockSize)) / length << endl;

	cout << endl << "SET ASSOCIATIVE CACHE PERFORMANCE" << endl;
	cout << "Misses: " << sa->misses << ", Hits: " << sa->hits << endl;
	cout << "Average CPI: " << ((float)sa->hits + sa->misses * (1 + 10 + sa->dataBlockSize)) / length << endl;
	
	// Free memory
	delete fa;
	delete dm;
	delete sa;

	cin.get();

	return 0;
}