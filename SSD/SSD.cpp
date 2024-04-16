#pragma once

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class SSD {
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
  
	void write(int address, std::string data) {

	}
};
