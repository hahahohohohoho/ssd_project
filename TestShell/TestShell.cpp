#pragma once
#include "ISSD.h"
#include <iostream>
#include <string>

class TestShell {
public:
	TestShell() {

	}

	void start() {
		while (1) {
			string str;
			cin >> str;
			if (str == "fullread") {
				fullread();
			}
			else if (str == "fullwrite") {
				fullwrite();
			}
			else if (str == "read") {

			}
			else if (str == "exit") {
				break;
			}
		}
	}

	void setSsd(ISSD* _ssd){
		ssd = _ssd;
	}

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