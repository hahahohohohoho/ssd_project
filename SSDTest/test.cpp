#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../SSD/SSD.cpp"
#include <stdexcept>
#include <fstream>
using namespace std;

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