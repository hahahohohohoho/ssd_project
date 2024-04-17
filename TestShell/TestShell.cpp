#pragma once
#include <string>
#include <vector>
#include <iostream>

#include "ISSD.h"
#include "CustomException.cpp"


using namespace std;

class TestShell {
public:

	explicit TestShell(ISSD* _ssd) : ssd(_ssd) {

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
					read(lba);
				}
				else if (command == "write") {
					int lba = getLba();
					string value = getValue();
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
				else if (command == "testapp2") {
					testapp2();
				}
				else {
					cout << "Invalid Command" << endl;
					help();
				}
			}
			catch (InvalidInputException e) {
				cout << e.what() << endl;
				help();
			}
			catch (ExitException e) {
				cout << e.what() << endl;
				break;
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
		string input_value;
		cin >> input_value;
		if (input_value.size() != 10)
			throw InvalidInputException("Invalid Value");

		if (input_value.substr(0, 2) != "0x")
			throw InvalidInputException("Invalid Value");

		for (char ch_value : input_value.substr(2, 10)) {
			if ('0' <= ch_value && ch_value <= '9') continue;
			if ('A' <= ch_value && ch_value <= 'F') continue;
			throw InvalidInputException("Invalid Value");
		}

		return input_value;
	}

	void terminateProcess() {
		throw ExitException("Exit Program");
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

	void testapp2() {
		cout << "[TestApp2] Write Aging Test" << endl;

		cout << "\nStep1 > write lba(0~5) value(0xAAAABBBB)" << endl;
		for (int lba = 0; lba <= 5; lba++) {
			for (int i = 0; i < 30; i++) {
				write(lba, "0xAAAABBBB");
			}
		}

		cout << "\nStep2 > overwrite lba(0~5) value(0x12345678)" << endl;
		for (int lba = 0; lba <= 5; lba++) {
			write(lba, "0x12345678");
		}

		cout << "\nStep3 > compare lba(0~5) value(0x12345678)" << endl;
		for (int lba = 0; lba <= 5; lba++) {
			string value = ssd->read(lba);
			cout << "Read Value(lba:" << lba << ") :" << value << endl;
			if (value != "0x12345678")
			{
				cout << "\n[DONE] TEST FAILED!" << endl;
				return;
			}
		}
		cout << "\n[DONE] TEST SUCCESS!" << endl;
	}

	void help() {
		std::locale::global(std::locale("en_US.UTF-8"));
		std::cout.imbue(std::locale());

		cout << "- write {no} {data} : {data} was recorded in LBA {no}\n"
			<< "-- {data} : hexadecimal \n"
			<< "-- ex. write 3 0xAAAABBBB\n"
			<< "- read {no} : Read LBA {no} times\n"
			<< "- fullwrite {value} : 0~99 LBA Write\n"
			<< "- fullread : 0~99 LBA Read\n"
			<< "- testapp1 : fullread/write test\n"
			<< "- testapp2 : Write Aging Test\n"
			<< "- exit : shell exits\n"
			<< "- help : Displays how to use each command\n";
	}



private:
	ISSD* ssd;
}
;