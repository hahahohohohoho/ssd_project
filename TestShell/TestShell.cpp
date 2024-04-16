#pragma once
#include "ISSD.h"

#include <string>
#include <vector>
#include <iostream>

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

	void start() {
		while (1) {
			string command = getCommand();
			if (command == "fullread") {
				fullread();
			}
			else if (command == "fullwrite") {
				fullwrite("0x01234567");
			}
			else if (command == "read") {
			}
			else if (command == "exit") {
				terminateProcess();
			}
			else if (command == "help") {
				help();
			}
		}
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

	void terminateProcess() {
		exitStrategy->exitProgram(0);
	}

	void write(int LBA, string value) {
		ssd->write(LBA, value);
	}

	void read(int LBA) {
		cout << ssd->read(LBA) << endl;
	}

	void fullread() {
		for (int lba = 0; lba < 100; lba++) {
			ssd->read(lba);
		}
	}

	void fullwrite(string value) {
		for (int lba = 0; lba < 100; lba++) {
			ssd->write(lba, value);
		}
	}

	void help() {
		cout << "asdf";
	}

private:
	IExitStrategy* exitStrategy;
	bool isAlive = true;
	ISSD* ssd;
};