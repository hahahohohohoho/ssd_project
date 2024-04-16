#pragma once
#include "ISSD.h"

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

	void fullread() {
		for (int lba = 0; lba < 100; lba++) {
			printf("Read %d : %s \n", lba, ssd->read(lba).c_str());
		}
	}

	void fullwrite() {
		for (int lba = 0; lba < 100; lba++) {
			ssd->write(lba, "0xDEADCODE");
		}
	}

private:
	ISSD* ssd;
};