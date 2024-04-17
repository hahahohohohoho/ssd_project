#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include "DataArrayFile.cpp"

using namespace std;

class ISSD {
public:
	virtual void read(int lba) = 0;
	virtual void write(int lba, string data) = 0;
};

static const int SSD_MAX_DATA_SIZE = 100;

class SSD : public ISSD {
private:
	string mData[SSD_MAX_DATA_SIZE];
	DataArrayFile nandFile{ "nand.txt" };
	DataArrayFile resultFile{ "result.txt" };
public:
	SSD() {
		for (int i = 0; i < SSD_MAX_DATA_SIZE; ++i) {
			mData[i] = "0x00000000";
		}
		if (!nandFile.isCreatedFile()) {
			nandFile.writeFileLines(mData, SSD_MAX_DATA_SIZE);
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
		nandFile.readFileLines(mData, SSD_MAX_DATA_SIZE);
		if (mData[lba] == "")
			mData[lba] = "0x00000000";
		resultFile.writeFileLines(&mData[lba], 1);
	}
  
	void write(int address, string data) {
		loadDataFromFile();

		mData[address] = data;

		ofstream outFile("nand.txt");
		for (int i = 0; i < 100; i++) {
			outFile << mData[i] << "\n";
		}
		outFile.close();
	}
};

