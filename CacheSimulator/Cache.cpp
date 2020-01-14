#pragma once
#include <vector>
#include <iostream>

class Cache
{
public:
	int misses;
	int hits;

	int rows;
	int dataBlockSize;

	std::vector<bool> valid;
	std::vector<int> tags;

	bool toobig = false;

	Cache(int rows)
		: rows(rows), dataBlockSize(0), misses(0), hits(0)
	{

		for (int i = 0; i < rows; i++) {
			valid.push_back(false);
			tags.push_back(-1);
		}

	}

	bool dataExistsInCache(int address) {

	}

};