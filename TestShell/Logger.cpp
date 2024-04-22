#pragma once
#include <iostream>
#include <string>
#include <queue>
#include <ctime>
#include <fstream>
using namespace std;

const static string baseFolder = ".\\log\\";
const static string latestFilePath = baseFolder + "latest.log";

#define MOVE_LOG_FILE(FROM, TO) ( string("move ") + (FROM) + string(" ") + (TO) )

class LoggerSingleton {
public:
	static LoggerSingleton& GetIstance() {
		if (instance == nullptr) {
			instance = new LoggerSingleton();
		}

		return *instance;
	}

	string print(string log, string func = __builtin_FUNCTION()) {
		openFile();

		string logFormat = collectLog(log, func);

		separateLogFiles();
		compressLogFiles();

		return logFormat;
	}
private:

	ofstream logFile;
	size_t currentLogSize;
	queue<string> logFileList;

	static LoggerSingleton* instance;

	LoggerSingleton() {
		system((string("mkdir ") + baseFolder).c_str());
		currentLogSize = 0;
	}
	~LoggerSingleton() {
		logFile.close();
	}

	void openFile(void) {
		if (!logFile.is_open()) {
			logFile.open(latestFilePath.c_str());
			if (!logFile.is_open()) {
				cout << "FILE OPEN FAIL" << endl;
				exit(1);
			}
			currentLogSize = 0;
		}
	}

	void separateLogFiles(void) {
		if (currentLogSize > 10000) {
			logFile.close();
			currentLogSize = 0;

			time_t now = time(0);
			tm* timeinfo = localtime(&now);

			char fileName[size("until_yymmdd_HHh_MMm_SSs.log")];
			strftime(fileName, sizeof(fileName),
				"until_%y%m%d_%Hh_%Mm_%Ss.log", timeinfo);

			string cmd = MOVE_LOG_FILE(latestFilePath, baseFolder + string(fileName));
			system(cmd.c_str());

			logFileList.push(fileName);
		}
	}
	void compressLogFiles(void) {
		while (logFileList.size() > 2) {
			string logfileName = logFileList.front();
			logFileList.pop();

			string cmd = MOVE_LOG_FILE((baseFolder + logfileName), (baseFolder + logfileName + string(".zip")));
			system(cmd.c_str());
		}
	}
	string collectLog(string log, string func) {
		time_t now = time(0);
		tm* timeinfo = localtime(&now);
		char timestamp[size("[yy.mm.dd hh:mm]")];
		strftime(timestamp, sizeof(timestamp),
			"[%y.%m.%d %H:%M]", timeinfo);

		func.append("()");
		while (func.size() < 30) {
			func.append(" ");
		}

		string logFormat = string(timestamp) + " " + func.substr(0, 30) + ": " + log + "\n";
		logFile << logFormat;
		currentLogSize += logFormat.size();

		return logFormat;
	}
};
LoggerSingleton* LoggerSingleton::instance = nullptr;

