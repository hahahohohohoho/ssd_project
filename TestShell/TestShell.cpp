#pragma once
#include "ISSD.h"
#include "IExitStrategy.h"

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class TestShell {
public:

	TestShell(ISSD* _ssd) : ssd(_ssd) {

	}

	TestShell(ISSD* _ssd, IExitStrategy* _exitStartegy) : ssd(_ssd), exitStrategy(_exitStartegy) {

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

	string getCommand() {
		string str;
		cin >> str;
		return str;
	}

	void terminateProcess() {
		exitStrategy->exitProgram();
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

	void help() {

		std::locale::global(std::locale("en_US.UTF-8"));
		std::cout.imbue(std::locale());

		cout << "- write {no} {data} : {data} was recorded in LBA {no}\n"
			<< "-- {data} : hexadecimal \n"
			<< "-- ex. write 3 0xAAAABBBB\n"
			<< "- read {no} : Read LBA {no} times\n"
			<< "- exit : shell exits\n"
			<< "- help : Displays how to use each command\n";
	}

private:
	IExitStrategy* exitStrategy;
	ISSD* ssd;
};