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
static const int SSD_FIXED_DATA_LENGTH = 10;
static const string SSD_DEFAULT_DATA = "0x00000000";

class SSD : public ISSD {
private:
	string mData[SSD_MAX_DATA_SIZE];
	DataArrayFile nandFile{ "nand.txt" };
	DataArrayFile resultFile{ "result.txt" };

	bool isInvalidData(string data) {
		if (data.length() != SSD_FIXED_DATA_LENGTH || data.substr(0, 2) != "0x") {
			return true;
		}
		return false;
	}

	bool isInvalidLba(int lba) {
		if (lba < 0 || lba > (SSD_MAX_DATA_SIZE - 1)) {
			return true;
		}
		return false;
	}
public:
	SSD() {
		for (int i = 0; i < SSD_MAX_DATA_SIZE; ++i) {
			mData[i] = SSD_DEFAULT_DATA;
		}
		if (!nandFile.isCreatedFile()) {
			nandFile.writeFileLines(mData, SSD_MAX_DATA_SIZE);
		}
	}

	void read(int lba) {
		nandFile.readFileLines(mData, SSD_MAX_DATA_SIZE);
		resultFile.writeFileLines(&mData[lba], 1);
	}

	void write(int lba, string data) {
		if (isInvalidData(data)) {
			throw invalid_argument("Invalid data");
		}
		if (isInvalidLba(lba)) {
			throw invalid_argument("Invalid LBA");
		}
  
		nandFile.readFileLines(mData, SSD_MAX_DATA_SIZE);
		mData[lba] = data;
		nandFile.writeFileLines(mData, SSD_MAX_DATA_SIZE);
	}
};

