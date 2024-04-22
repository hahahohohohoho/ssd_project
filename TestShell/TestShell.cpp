#pragma once
#include <string>
#include <vector>
#include <iostream>

#include "ISSD.h"
#include "CustomException.cpp"
#include "TCManager.cpp"
#include "Logger.h"

using namespace std;

class TestShell {
public:
	explicit TestShell(ISSD* _ssd) : ssd(_ssd) {
		tcManager = new TCManager("tclist", TCManager::STDOUT_REDIRECTION_OFF);
		log = LoggerSingleton::GetInstancePtr();
	}

	void start() {
		while (1) {
			try {
				processCommand(getCommand());
			}
			catch (InvalidInputException e) {
				log->print(e.what());
				while (std::cin.get() != '\n');
				help();
			}
			catch (ExitException e) {
				log->print(e.what());
				break;
			}
		}
	}

	string getCommand() {
		string command;
		cout << "\nCMD > ";
		cin >> command;
		log->print("Input Command : " + command);
		return command;
	}
	
	void processCommand(string command) {
		if (command == "fullread") {
			fullread();
		}
		else if (command == "fullwrite") {
			getValue();
			fullwrite(this->value);
		}
		else if (command == "read") {
			getLba();
			read(this->start_lba);
		}
		else if (command == "write") {
			getLba();
			getValue();
			write(this->start_lba, this->value);
		}
		else if (command == "erase") {
			getLba();
			getSize();
			erase(this->start_lba, this->size);
		}
		else if (command == "erase_range") {
			getLba();
			getEndLba();
			erase_range(this->start_lba, this->end_lba);
		}
		else if (command == "exit") {
			terminateProcess();
		}
		else if (command == "help") {
			help();
		}
		else {
			if (tcManager->run(command) < 0) {
				throw InvalidInputException("Invalid Command");
			}
		}
	}

	bool isNumber(const std::string& str) {
		for (char c : str) {
			if (!std::isdigit(c)) {
				return false;
			}
		}
		return true;
	}

	void getLba() {
		string str_lba;
		cin >> str_lba;

		if (!isNumber(str_lba))
			throw InvalidInputException("Invalid Lba");

		this->start_lba = stoi(str_lba);
		if (0 > this->start_lba || this->start_lba > 99)
			throw InvalidInputException("Invalid Lba");
	}

	void getEndLba() {
		string end_lba;
		cin >> end_lba;

		if (!isNumber(end_lba))
			throw InvalidInputException("Invalid Lba");

		this->end_lba = stoi(end_lba);
		if (0 > this->end_lba || this->end_lba > 99)
			throw InvalidInputException("Invalid Lba");
	}

	void getSize() {
		string size;
		cin >> size;

		if (!isNumber(size))
			throw InvalidInputException("Invalid Size");

		this->size = stoi(size);
		if (this->size < 0 || this->size > 10)
			throw InvalidInputException("Invalid Size");
	}

	void getValue() {
		cin >> this->value;
		if (this->value.size() != 10 || this->value.substr(0, 2) != "0x")
			throw InvalidInputException("Invalid Value");

		for (char ch_value : this->value.substr(2, 10)) {
			if ('0' <= ch_value && ch_value <= '9') continue;
			if ('A' <= ch_value && ch_value <= 'F') continue;
			throw InvalidInputException("Invalid Value");
		}
	}

	void terminateProcess() {
		throw ExitException("Exit Program");
	}

	void erase(int lba, int size) {
		ssd->erase(lba, size);
		log->print("LBA : " + to_string(lba) + ", SIZE : " + value);
	}

	void erase_range(int start_lba, int end_lba) {
		int start = start_lba;
		while (end_lba >= start) {
			int rem = end_lba - start + 1;
			int size = (rem > 10) ? (10) : (rem);
			ssd->erase(start, size);
			start += 10;
		}
	}

	void write(int lba, string value) {
		ssd->write(lba, value);
		log->print("LBA : " + to_string(lba) + ", VALUE : " + value);
	}

	string read(int lba) {
		string value = ssd->read(lba);
		log->print("LBA : " + to_string(lba) + ", VALUE : " + value);
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

	void help() {
		log->print("[ CMD ]");
		log->print("- write {no} {data} : {data} was recorded in LBA {no}");
		log->print("-- {data} : hexadecimal ");
		log->print("-- ex. write 3 0xAAAABBBB");
		log->print("- read {no} : Read LBA {no} times");
		log->print("- fullwrite {value} : 0~99 LBA Write");
		log->print("- fullread : 0~99 LBA Read");
		log->print("- erase {lba} {size}");
		log->print("- erase_range {start lba} {end lba}");
		log->print("- exit : shell exits");
		log->print("- help : Displays how to use each command");

		tcManager->printTestCases();

	}

private:
	ISSD* ssd;
	TCManager* tcManager;
	int start_lba, end_lba;
	int size;
	string value;

	LoggerSingleton* log;
};