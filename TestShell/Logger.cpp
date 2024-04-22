#pragma once
#include <iostream>
#include <string>
#include <queue>
#include <ctime>
#include <fstream>
#include "Logger.h"
using namespace std;

#define FILE_SEPERATION_THRESHOLD_SIZE_BYTE	(10000)
#define FILE_COMPRESS_THRESHOLD_COUNT		(2)

#define MOVE_LOG_FILE(FROM, TO) ( string("move ") + (FROM) + string(" ") + (TO) )

LoggerSingleton* LoggerSingleton::instance = nullptr;

const static string baseFolder = ".\\log_testapp\\";
const static string latestFilePath = baseFolder + "latest.log";

LoggerSingleton::LoggerSingleton() {
	system((string("mkdir ") + baseFolder).c_str());
	currentLogSize = 0;
}
LoggerSingleton::~LoggerSingleton() {
	logFile.close();
}

string LoggerSingleton::print(string log, string func) {
	cout << log << endl;

	openFile();

	string logFormat = collectLog(log, func);

	separateLogFiles();
	compressLogFiles();

	return logFormat;
}

void LoggerSingleton::openFile(void) {
	if (!logFile.is_open()) {
		logFile.open(latestFilePath.c_str());
		if (!logFile.is_open()) {
			cout << "[LOGGER] LOG FILE OPEN FAIL" << endl;
			exit(1);
		}
		currentLogSize = 0;
	}
}

void LoggerSingleton::separateLogFiles(void) {
	if (currentLogSize > FILE_SEPERATION_THRESHOLD_SIZE_BYTE) {
		logFile.close();
		currentLogSize = 0;

		time_t now = time(NULL);
		tm timeinfo;
		localtime_s(&timeinfo, &now);
		char fileName[size("until_yymmdd_HHh_MMm_SSs.log")];
		strftime(fileName, sizeof(fileName),
			"until_%y%m%d_%Hh_%Mm_%Ss.log", &timeinfo);

		string cmd = MOVE_LOG_FILE(latestFilePath, baseFolder + string(fileName));
		system(cmd.c_str());

		logFileList.push(fileName);
	}
}
void LoggerSingleton::compressLogFiles(void) {
	while (logFileList.size() > FILE_COMPRESS_THRESHOLD_COUNT) {
		string logfileName = logFileList.front();
		logFileList.pop();

		string cmd = MOVE_LOG_FILE((baseFolder + logfileName), (baseFolder + logfileName + string(".zip")));
		system(cmd.c_str());
	}
}
string LoggerSingleton::collectLog(string log, string func) {
	time_t now = time(NULL);
	tm timeinfo;
	localtime_s(&timeinfo, &now);
	char timestamp[size("[yy.mm.dd hh:mm]")];
	strftime(timestamp, sizeof(timestamp),
		"[%y.%m.%d %H:%M]", &timeinfo);

	func.append("()");
	while (func.size() < 30) {
		func.append(" ");
	}

	string logFormat = string(timestamp) + " " + func.substr(0, 30) + ": " + log + "\n";
	logFile << logFormat;
	currentLogSize += logFormat.size();

	return logFormat;
}