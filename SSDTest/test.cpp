#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <fstream>
#include <string>

#include "../SSD/Application.cpp"


using namespace std;
using namespace testing;

class MockSSD : public ISSD {
public:
	MOCK_METHOD(void, read, (int), (override));
	MOCK_METHOD(void, write, (int, string), (override));
};

class TestSSDFixture : public Test {
private:
	const string TEST_NAND_FILE_PATH = "nand.txt";
	const string TEST_RESULT_FILE_PATH = "result.txt";

	void initNand() {
		ofstream mNandOs(TEST_NAND_FILE_PATH);
		for (int i = 0; i < 100; i++) {
			mNandOs << "0x00000000" << "\n";
		}
		mNandOs.close();
	}
	void initResult() {
		ofstream mResultOs(TEST_RESULT_FILE_PATH);
		mResultOs << "0x00000000";
		mResultOs.close();
	}
public:
	void SetUp() override {
		initNand();
		initResult();
	}

	void TearDown() override {
		initNand();
		initResult();
	}

	void writeNand(int address, string data) {
		string nandData[100];

		ifstream nandIs("nand.txt");
		string line;
		int index = 0;
		while (getline(nandIs, line) && line != "") {
			nandData[index++] = line;
		}
		nandIs.close();

		nandData[address] = data;

		ofstream nandOs(TEST_NAND_FILE_PATH);
		for (int i = 0; i < 100; i++) {
			nandOs << nandData[i] << "\n";
		}
		nandOs.close();
	}

	string readNand(int address) {
		ifstream nandIs(TEST_NAND_FILE_PATH);
		int lineNumber = 0;
		string line;
		while (getline(nandIs, line)) {
			if (lineNumber == address) {
				nandIs.close();
				return line;
			}
			lineNumber++;
		}
		nandIs.close();
		return "";
	}

	string readResult() {
		ifstream resultIs(TEST_RESULT_FILE_PATH);
		string line;
		getline(resultIs, line);
		resultIs.close();
		return line;
	}
};

TEST_F(TestSSDFixture, WriteOneData) {
	SSD ssd;

	ssd.write(3, "0x00000001");

	EXPECT_EQ("0x00000001", readNand(3));
}

TEST_F(TestSSDFixture, WriteTwoData) {
	SSD ssd;

	ssd.write(3, "0x00000001");
	ssd.write(5, "0x00000020");

	EXPECT_EQ("0x00000001", readNand(3));
	EXPECT_EQ("0x00000020", readNand(5));
}

TEST_F(TestSSDFixture, WriteThreeData) {
	SSD ssd;

	ssd.write(0, "0x00000001");
	ssd.write(5, "0x00000020");
	ssd.write(99, "0x000ABCDE");

	EXPECT_EQ("0x00000001", readNand(0));
	EXPECT_EQ("0x00000020", readNand(5));
	EXPECT_EQ("0x000ABCDE", readNand(99));
}

TEST_F(TestSSDFixture, WriteInvalidDataLength) {
	SSD ssd;

	EXPECT_THROW(ssd.write(3, "0x01"), invalid_argument);
	EXPECT_THROW(ssd.write(3, "0x0001"), invalid_argument);
	EXPECT_THROW(ssd.write(3, "0x000001"), invalid_argument);
}

TEST_F(TestSSDFixture, WriteInvalidDataFormat) {
	SSD ssd;

	EXPECT_THROW(ssd.write(3, "0000000001"), invalid_argument);
	EXPECT_THROW(ssd.write(3, "xx00000001"), invalid_argument);
	EXPECT_THROW(ssd.write(3, "x000000001"), invalid_argument);
}

TEST_F(TestSSDFixture, WriteInvalidAddress) {
	SSD ssd;

	EXPECT_THROW(ssd.write(-1, "0x00000001"), invalid_argument);
	EXPECT_THROW(ssd.write(100, "0x00000001"), invalid_argument);
	EXPECT_THROW(ssd.write(101, "0x00000001"), invalid_argument);
}

TEST_F(TestSSDFixture, ReadEmptySpace) {
	SSD ssd;

	ssd.read(0);

	EXPECT_THAT(readResult(), "0x00000000");
}

TEST_F(TestSSDFixture, ReadZeroLBA) {
	SSD ssd;
	writeNand(0, "0x12345678");

	ssd.read(0);

	EXPECT_THAT(readResult(), "0x12345678");
}

TEST_F(TestSSDFixture, ReadOneLBA) {
	SSD ssd;
	writeNand(0, "0x12345678");
	writeNand(1, "0xABCDEFAB");

	ssd.read(1);

	EXPECT_THAT(readResult(), "0xABCDEFAB");
}

TEST_F(TestSSDFixture, ReadWithInvalidLBA) {
	SSD ssd;

	EXPECT_THROW(ssd.read(-1), out_of_range);
	EXPECT_THROW(ssd.read(100), out_of_range);
}

TEST(TestSSD, AppInvalidArgument) {
	MockSSD ssd;
	Application app(&ssd);

	EXPECT_CALL(ssd, read(_))
		.Times(0);
	EXPECT_CALL(ssd, write(_,_))
		.Times(0);

	char* cmd[10] = { "SSD.exe", "E", "0","0x00000000" };
	app.run(4, cmd);
}

TEST(TestSSD, AppLessArgument) {
	MockSSD ssd;
	Application app(&ssd);

	EXPECT_CALL(ssd, read(_))
		.Times(0);
	EXPECT_CALL(ssd, write(_, _))
		.Times(0);

	char* cmd[10] = { "SSD.exe", "R" };
	app.run(2, cmd);
}

TEST(TestSSD, AppArgumentPassing) {
	MockSSD ssd;
	Application app(&ssd);

	EXPECT_CALL(ssd, read(0))
		.Times(1);
	EXPECT_CALL(ssd, write(0, "0x00000000"))
		.Times(1);

	char* cmd1[10] = { "SSD.exe", "R", "0","0x00000000" };
	app.run(4, cmd1);
	char* cmd2[10] = { "SSD.exe", "W", "0","0x00000000" };
	app.run(4, cmd2);
}

TEST(TestSSD, FileReadEmpty) {
	std::ofstream outFile("text.txt");
	outFile << "" << endl;
	outFile.close();

	DataArrayFile file("text.txt");
	string buf[1];
	file.readFileLines(buf, 1);
	EXPECT_THAT(buf[0], Eq(""));

	std::ofstream removeFile("text.txt");
	remove("text.txt");
	removeFile.close();
}

TEST(TestSSD, FileReadTwoLines) {
	std::ofstream outFile("text.txt");
	outFile << "ABCD" << endl;
	outFile << "EFGH" << endl;
	outFile.close();

	DataArrayFile file("text.txt");
	string buf[2];
	file.readFileLines(buf, 2);
	EXPECT_THAT(buf[0], Eq("ABCD"));
	EXPECT_THAT(buf[1], Eq("EFGH"));

	std::ofstream removeFile("text.txt");
	remove("text.txt");
	removeFile.close();
}

TEST(TestSSD, FileWriteTwoLine) {
	DataArrayFile file("text.txt");
	string buf[2] = { "ABCD", "EFGH" };
	file.writeFileLines(buf, 2);

	string line;
	ifstream inFile("text.txt");
	for (int i = 0; i < 2; ++i) {
		getline(inFile, line);
		EXPECT_THAT(line, Eq(buf[i]));
	}
	inFile.close();

	std::ofstream removeFile("text.txt");
	remove("text.txt");
	removeFile.close();
}


