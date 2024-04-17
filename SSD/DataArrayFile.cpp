#pragma once

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class DataArrayFile {
private:
	string filePath;
public:
	DataArrayFile(string filePath)
		: filePath(filePath) {

	}

	bool isCreatedFile() {
		ifstream inFile(filePath);
		bool ret = inFile.is_open();
		inFile.close();
		return ret;
	}

	void readFileLines(string buf[], int size) {
		ifstream inFile(filePath);
		for (int i = 0; i < size; ++i) {
			getline(inFile, buf[i]);
		}
		inFile.close();
	}

	void writeFileLines(string buf[], int size) {
		ofstream outFile(filePath);
		for (int i = 0; i < size; ++i) {
			outFile << buf[i] << endl;
		}
		outFile.close();
	}
};
