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
		ssd->read(1);
	}

	void fullwrite() {
		ssd->write(1, "0xAAAAAAAA");
	}

private:
	ISSD* ssd;
};