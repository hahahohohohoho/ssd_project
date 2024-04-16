#pragma once
#include "ISSD.h"
#include <iostream>
#include <string>

using namespace std;

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
		cout << "TestShell is terminated." << endl;
	}

	void setSsd(ISSD* _ssd){
		ssd = _ssd;
	}

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