#include "gtest/gtest.h"
#include "gmock/gmock.h"


#include <iostream>
#include <fstream>
#include <string>

#include "../SSD/SSD.cpp"

using namespace std;
using namespace testing;

TEST(TestCaseName, TestName) {
	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
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
