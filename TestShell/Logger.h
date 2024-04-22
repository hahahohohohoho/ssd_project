#pragma once
#include <iostream>
#include <string>
#include <queue>
#include <ctime>
#include <fstream>
using namespace std;

class LoggerSingleton {
public:
	static LoggerSingleton& GetInstanceRef() {
		if (instance == nullptr) {
			instance = new LoggerSingleton();
		}

		return *instance;
	}
	static LoggerSingleton* GetInstancePtr() {
		if (instance == nullptr) {
			instance = new LoggerSingleton();
		}

		return instance;
	}

	string print(string log, string func = __builtin_FUNCTION());
private:

	ofstream logFile;
	size_t currentLogSize;
	queue<string> logFileList;

	static LoggerSingleton* instance;

	LoggerSingleton();
	~LoggerSingleton();

	void openFile(void);

	void separateLogFiles(void);
	void compressLogFiles(void);
	string collectLog(string log, string func);
};

