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
				string value;
				cin >> value;
				fullwrite(value);
			}
			else if (command == "read") {
				int lba;
				cin >> lba;
				read(lba);
			}
			else if (command == "write") {
				int lba;
				string value;
				cin >> lba >> value;
				write(lba, value);
			}
			else if (command == "exit") {
				terminateProcess();
			}
			else if (command == "help") {
				help();
			}
			else if (command == "testapp1") {
				testapp1();
			}

			else {
				cout << "invalid command" << endl;
				help();
			}
		}
	}

	string getCommand() {
		string str;
		cout << "\nCMD > ";
		cin >> str;
		return str;
	}

	void terminateProcess() {
		exitStrategy->exitProgram();
	}

	void write(int LBA, string value) {
		ssd->write(LBA, value);
	}

	string read(int LBA) {
		string value = ssd->read(LBA);
		cout << value << endl;

		return value;
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

	bool testapp1() {
		string writeValue = "0x01234567";
		fullwrite(writeValue);

		for (int LBA = 0; LBA < 100; ++LBA) {
			string value = read(LBA).substr(0,10);
			if (value != writeValue) {
				cout << "testapp1 Failed - LBA : " << LBA << ", value : " << value << endl;
				return false;
			}
		}

		cout << "testapp1 success" << endl;
		return true;
	}

	void help() {
		std::locale::global(std::locale("en_US.UTF-8"));
		std::cout.imbue(std::locale());

		cout << "- write {no} {data} : {data} was recorded in LBA {no}\n"
			<< "-- {data} : hexadecimal \n"
			<< "-- ex. write 3 0xAAAABBBB\n"
			<< "- read {no} : Read LBA {no} times\n"
			<< "- fullwrite {value} : 0~99 LBA Write Test\n"
			<< "- fullread : 0~99 LBA Read Test\n"
			<< "- exit : shell exits\n"
			<< "- help : Displays how to use each command\n";
	}

private:
	IExitStrategy* exitStrategy;
	ISSD* ssd;
};