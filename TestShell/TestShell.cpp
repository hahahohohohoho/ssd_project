#pragma once
#include "ISSD.h"

class TestShell {
public:
	TestShell(ISSD* _ssd) : ssd(_ssd) {

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