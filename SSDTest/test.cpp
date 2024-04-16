#include "gtest/gtest.h"
#include "gmock/gmock.h"


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