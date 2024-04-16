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
	void readFileLines(string buf[], int size) {
		ifstream inFile(filePath);
		for (int i = 0; i < size; ++i) {
			getline(inFile, buf[i]);
		}
		inFile.close();
	}
};

class ISSD {
public:
	virtual void read(int lba) = 0;
	virtual void write(int lba, string data) = 0;
};

class SSD : public ISSD {
private:
	string mData[105];
public:
	SSD() {
		for (int i = 0; i < 100; ++i) {
			mData[i] = "0x00000000";
		}
	}

	void loadDataFromFile() {
		ifstream inFile("nand.txt");
		string line;
		int index = 0;
		while (getline(inFile, line) && line != "") {
			mData[index++] = line;
		}
		inFile.close();
	}

	void read(int lba) {
		loadDataFromFile();
		ofstream outFile("result.txt");
		outFile << mData[lba] << std::endl;
		outFile.close();
	}
  
	void write(int address, string data) {
		loadDataFromFile();

		mData[address] = data;

		ofstream outFile("nand.txt");
		for (int i = 0; i < 100; i++) {
			outFile << mData[i] << "\n";
		}
	}
  
};

