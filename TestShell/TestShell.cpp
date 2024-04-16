#pragma once
#include "ISSD.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <cstdio>

using namespace std;

class SSD_Driver : public ISSD {
public:
	explicit SSD_Driver(string outputName) : outputName(outputName) {

	}
	string read(int lba) override {
		string cmd = "ssd.exe read " + to_string(lba);
		system(cmd.c_str());

		ifstream fp(outputName.c_str());
		string line;
		if(fp.is_open())
			getline(fp, line);

		fp.close();

		return line;
	}
	void write(int lba, string value) override {
		string cmd = "ssd.exe write " + to_string(lba) + " " + value;
		cout << cmd << endl;
		system(cmd.c_str());
	}

	string outputName;
};


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
			read(lba);
		}
	}

	void fullwrite(string value) {
		for (int lba = 0; lba < 100; lba++) {
			write(lba, value);
		}
	}

	void help() {
		cout << "- write {no} {data} : {no}번 LBA에 {data}를 기록\n"
			<< "-- {data} : 16진수 \n"
			<< "-- ex. write 3 0xAAAABBBB\n"
			<< "- read {no} : {no}번 LBA를 읽음\n"
			<< "- exit : shell이 종료\n"
			<< "- help : 각 명령어의 사용 방법을 출력\n";
	}

private:
	IExitStrategy* exitStrategy;
	bool isAlive = true;
	ISSD* ssd;
};