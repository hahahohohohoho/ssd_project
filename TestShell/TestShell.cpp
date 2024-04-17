#pragma once
#include "ISSD.h"
#include "IExitStrategy.h"

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class InvalidInputException : public exception {
public:
	explicit InvalidInputException(char const* message) : exception(message) {

	}
};

class TestShell {
public:

	TestShell(ISSD* _ssd) : ssd(_ssd) {

	}

	TestShell(ISSD* _ssd, IExitStrategy* _exitStartegy) : ssd(_ssd), exitStrategy(_exitStartegy) {

	}

	void start() {
		while (1) {
			try {
				string command = getCommand();
				if (command == "fullread") {
					fullread();
				}
				else if (command == "fullwrite") {
					string value = getValue();
					fullwrite(value);
				}
				else if (command == "read") {
					int lba = getLba();
					//if (!checkLba(lba)) continue;
					read(lba);
				}
				else if (command == "write") {
					int lba = getLba();
					string value = getValue();
					//cin >> lba >> value;
					//if (!checkLba(lba) || (!checkValue(value))) continue;

					write(lba, value);
				}
				else if (command == "exit") {
					terminateProcess();
				}
				else if (command == "help") {
					help();
				}
				else {
					cout << "Invalid Command" << endl;
					help();
				}
			}
			catch (InvalidInputException e) {
				cout << e.what() << endl;
			}
		}
	}

	string getCommand() {
		string str;
		cout << "\nCMD > ";
		cin >> str;
		return str;
	}
	int getLba() {
		int lba;
		cin >> lba;
		if (0 > lba || lba > 99) throw InvalidInputException("Invalid Lba");

		return lba;
	}
	string getValue() {
		string value;
		cin >> value;
		for (int i = 0; i < value.size(); i++) {
			if ('0' <= value[i] && value[i] <= '9') continue;
			if ('A' <= value[i] && value[i] <= 'F') continue;
			throw InvalidInputException("Invalid Lba");
		}
		return value;
	}

	//bool checkLba(int lba) {
	//	if (0 <= lba && lba <= 99) {
	//		cout << "invalid lba" << endl;
	//		return true;
	//	}
	//	return false;
	//}

	//bool checkValue(string value) {
	//	for (int i = 0; i < value.size(); i++) {
	//		if ('0' <= value[i] && value[i] <= '9') continue;
	//		if ('A' <= value[i] && value[i] <= 'F') continue;
	//		cout << "invalid value" << endl;
	//		return false;
	//	}
	//	return true;
	//}

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
			<< "- fullwrite { value } : 0~99 LBA Write Test\n"
			<< "- fullread : 0~99 LBA Read Test\n"
			<< "- exit : shell exits\n"
			<< "- help : Displays how to use each command\n";
	}

private:
	IExitStrategy* exitStrategy;
	ISSD* ssd;
}
;