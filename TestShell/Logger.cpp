#pragma once
#include <iostream>
#include <string>
#include <queue>
#include <ctime>
#include <fstream>
using namespace std;

class Logger {
public:
	Logger() {
		logFile.open("latest.log");
		currentLogSize = 0;
	}
	~Logger() {
		logFile.close();
	}
	string print(string log, string func = __builtin_FUNCTION()) {
		time_t now = time(0);
		tm* timeinfo = localtime(&now);
		char timestamp[size("[yy.mm.dd hh:mm]")];
		strftime(timestamp, sizeof(timestamp),
			"[%y.%m.%d %H:%M]", timeinfo);

		func.append("()");
		if (func.size() < 30) {
			while (func.size() < 30) {
				func.append(" ");
			}
		}

		string logFormat = string(timestamp) + " " + func + " " + log + "\n";
		logFile << logFormat;
		currentLogSize += logFormat.size();

		if (currentLogSize > 10000) {
			logFile.close();
			currentLogSize = 0;

			time_t now = time(0);
			tm* timeinfo = localtime(&now);

			char fileName[size("until_yymmdd_HHh_MMm_SSs.log")];
			strftime(fileName, sizeof(fileName),
				"until_%y%m%d_%Hh_%Mm_%Ss.log", timeinfo);
			
			string cmd = "ren latest.log " + string(fileName);
			system(cmd.c_str());

			logFileList.push(fileName);

			logFile.open("latest.log");
		}

		if (logFileList.size() > 2) {
			// Compress LogFiles

			while (logFileList.size() > 2) {
				string logfileName = logFileList.front();
				logFileList.pop();

				string cmd = "ren " + logfileName + ": " + logfileName + ".zip";
				system(cmd.c_str());
			}

		}

		return logFormat;
	}

private:
	ofstream logFile;
	string LogBuffer;
	int currentLogSize;
	queue<string> logFileList;
};