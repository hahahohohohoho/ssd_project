#pragma once
#include "ISSD.h"

#include <string>
#include <vector>
#include <iostream>

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
				fullwrite("0x01234567");
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

	void write(int LBA, string value) {
		ssd->write(LBA, value);
	}

	void read(int LBA) {
		cout << ssd->read(LBA) << endl;
	}

	void fullread() {
		for (int lba = 0; lba < 100; lba++) {
			read(lba);
		}
	}

	void fullwrite(string value) {
		for (int lba = 0; lba < 100; lba++) {
			write(lba, value);
		}
	}

private:
	ISSD* ssd;
};