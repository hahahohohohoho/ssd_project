#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <fstream>
#include <string>

#include "../SSD/SSD.cpp"

using namespace std;
using namespace testing;

class FileManager {
private:
	const string TEST_NAND_FILE_PATH = "nand.txt";
	const string TEST_RESULT_FILE_PATH = "result.txt";

	ofstream mNandOs;
	ifstream mNandIs;
	ifstream mResultIs;
public:
	FileManager() {
		mNandOs.open(TEST_NAND_FILE_PATH);
		mNandIs.open(TEST_NAND_FILE_PATH);
		mResultIs.open(TEST_RESULT_FILE_PATH);
	}

	~FileManager() {
		mNandOs.close();
		mNandIs.close();
		mResultIs.close();
	}

	void writeNand(string content) {
		mNandOs << content << "Wn";
	}

	string readNand(int address) {
		int lineNumber = 0;
		string line;
		while (getline(mNandIs, line)) {
			if (lineNumber == address) {
				return line;
			}
			lineNumber++;
		}
		return "";
	}

	string readResult(int address) {
		int lineNumber = 0;
		string line;
		while (getline(mResultIs, line)) {
			if (lineNumber == address) {
				return line;
			}
			lineNumber++;
		}
		return "";
	}
};

TEST(TestSSDwrite, normal) {
	SSD ssd;
	FileManager fileManager;

	ssd.write(3, "0x00000001");

	EXPECT_EQ("0x00000001", fileManager.readNand(3));
}

TEST(TestSSD, ReadEmptySpace) {
	std::ofstream outFile("nand.txt");
	outFile << "" << std::endl;
	outFile.close();

	SSD ssd;
	ssd.read(0);

	std::ifstream inFile("result.txt");
	string line;
	getline(inFile, line);
	inFile.close();

	EXPECT_THAT("0x00000000", Eq(line));
}

TEST(TestSSD, ReadZeroLBA) {
	std::ofstream outFile("nand.txt");
	outFile << "0x12345678" << std::endl;
	outFile.close();

	SSD ssd;
	ssd.read(0);

	std::ifstream inFile("result.txt");
	string line;
	getline(inFile, line);
	inFile.close();

	EXPECT_THAT("0x12345678", Eq(line));
}

TEST(TestSSD, ReadOneLBA) {
	std::ofstream outFile("nand.txt");
	outFile << "0x12345678" << endl;
	outFile << "0xABCDEFAB" << endl;
	outFile.close();

	SSD ssd;
	ssd.read(1);

	std::ifstream inFile("result.txt");
	string line;
	getline(inFile, line);
	inFile.close();

	EXPECT_THAT("0xABCDEFAB", Eq(line));
}
