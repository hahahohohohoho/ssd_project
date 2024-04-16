#pragma once
#include "ISSD.h"
#include <string>
#include <vector>

class TestShell {
public:
	TestShell(ISSD* _ssd) : ssd(_ssd) {

	}

	void write(int LBA, string value) {
		ssd->write(LBA, value);
	}

	string read(int LBA) {
		return ssd->read(LBA);
	}

	vector<string> fullread() {
		vector<string> read_arr;
		for (int lba = 0; lba < 100; lba++) {
			read_arr.push_back(ssd->read(lba));
		}
		return read_arr;
	}

	void fullwrite(string value) {
		for (int lba = 0; lba < 100; lba++) {
			ssd->write(lba, value);
		}
	}

private:
	ISSD* ssd;
};