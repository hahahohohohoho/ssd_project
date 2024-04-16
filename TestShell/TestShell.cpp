#pragma once
#include "ISSD.h"

class TestShell {
public:
	TestShell(ISSD* _shell) : shell(_shell) {

	}

	void fullread() {
		shell->read();
	}

	void fullwrite() {
		shell->write(1, "0xAAAAAAAA");
	}

private:
	ISSD* shell;
};