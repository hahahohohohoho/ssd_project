#pragma once
#include "ISSD.h"
#include <iostream>
#include <string>

using namespace std;

class IExitStrategy {
public:
	virtual void exitProgram(int status) = 0;
};

class RealExitStrategy : public IExitStrategy {
public:
	void exitProgram(int status) override {
		std::exit(status);
	}
};

class TestExitStrategy : public IExitStrategy {
public:
	void exitProgram(int status) override {
		throw std::runtime_error("Program exit called with status " + std::to_string(status));
	}
};


class TestShell {
public:
	TestShell() {

	}

	TestShell(ISSD* _ssd) : ssd(_ssd) {

	}

	void setSsd(ISSD* _ssd) {
		ssd = _ssd;
	}

	void setExitStrategy(IExitStrategy* strategy) {
		exitStrategy = strategy;
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
	void terminateProcess() {
		exitStrategy->exitProgram(0);
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
	IExitStrategy* exitStrategy;
	bool isAlive = true;
	ISSD* ssd;
};