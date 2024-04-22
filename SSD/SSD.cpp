#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "DataArrayFile.cpp"
#include "CommandQueue.cpp"
#include "CmdString.h"

using namespace std;

class ISSD {
public:
	virtual void read(int lba) = 0;
	virtual void write(int lba, string data) = 0;
	virtual void erase(int lba, int size) = 0;
	virtual void flush(void) = 0;
};

static const int SSD_MAX_DATA_SIZE = 100;
static const int SSD_FIXED_DATA_LENGTH = 10;
static const string SSD_DEFAULT_DATA = "0x00000000";
static const int SSD_MAX_ERASE_SIZE = 10;

class SSD : public ISSD {
private:
	string mData[SSD_MAX_DATA_SIZE];
	CommandQueue cq;
	DataArrayFile nandFile{ "nand.txt" };
	DataArrayFile resultFile{ "result.txt" };

	bool isInvalidData(string data) {
		return (data.length() != SSD_FIXED_DATA_LENGTH || data.substr(0, 2) != "0x");
	}

	bool isInvalidLBA(int lba) {
		return (lba < 0 || lba >= SSD_MAX_DATA_SIZE);
	}

	bool isInvalidEraseSize(int size) {
		return (size < 0 || size > SSD_MAX_ERASE_SIZE);
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
		if (isInvalidLBA(lba)) {
			throw out_of_range("LBA is out of range");
		}
		string data = cq.findData(lba);
		if (!data.empty()) {
			resultFile.writeFileLines(&data, 1);
			return;
		}

		nandFile.readFileLines(mData, SSD_MAX_DATA_SIZE);
		resultFile.writeFileLines(&mData[lba], 1);
	}

	void flush() {
		nandFile.readFileLines(mData, SSD_MAX_DATA_SIZE);
		
		for (CommandQueueItem item : cq.getItems()) {
			int lba = stoi(item.parameter1);
			if (item.cmdName == CMD[WRITE]) {
				string data = item.parameter2;
				mData[lba] = data;
				continue;
			}

			if (item.cmdName == CMD[ERASE]) {
				int size = stoi(item.parameter2);
				for (int i = lba; i < lba + size; ++i) {
					mData[i] = SSD_DEFAULT_DATA;
				}
				continue;
			}
		}

		nandFile.writeFileLines(mData, SSD_MAX_DATA_SIZE);
		cq.clear();
	}

	void write(int lba, string data) {
		if (isInvalidData(data)) {
			throw invalid_argument("Invalid data");
		}
		if (isInvalidLBA(lba)) {
			throw invalid_argument("Invalid LBA");
		}
		
		cq.addItem({ CMD[WRITE], to_string(lba), data});

		if (cq.isFull()) {
			flush();
		}
	}

	void erase(int lba, int size) {
		if (isInvalidLBA(lba)) {
			throw invalid_argument("Invalid LBA");
		}
		if (isInvalidEraseSize(size)) {
			throw invalid_argument("Invalid erase size");
		}

		cq.addItem({ CMD[ERASE], to_string(lba), to_string(size) });

		if (cq.isFull()) {
			flush();
		}
	}
};
