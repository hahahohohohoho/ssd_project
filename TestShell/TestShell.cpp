#pragma once
#include "ISSD.h"
#include <iostream>
#include <string>

using namespace std;

class TestShell {
public:
	TestShell() {

	}

	string getCommand() {
		string str;
		cin >> str;
		return str;
	}

	void start() {
		while (1) {
			string command = getCommand();
			if (command == "fullread") {
				fullread();
			}
			else if (command == "fullwrite") {
				fullwrite();
			}
			else if (command == "read") {

			}
			else if (command == "exit") {
				terminateProcess();
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

	void terminateProcess() {
		delete this;
	}

	void fullread() {
		ssd->read(1);
	}

	void fullwrite() {
		ssd->write(1, "0xAAAAAAAA");
	}

private:
	bool isAlive = true;
	ISSD* ssd;
};