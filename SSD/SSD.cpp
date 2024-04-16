#pragma once

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class ISSD {
public:
	virtual void read(int lba) = 0;
	virtual void write(int lba, string data) = 0;
};

class SSD : public ISSD {
private:
	string data[105];
public:
	SSD() {
		for (int i = 0; i < 100; ++i) {
			data[i] = "0x00000000";
		}
	}

	void loadDataFromFile() {
		ifstream inFile("nand.txt");
		string line;
		int index = 0;
		while (getline(inFile, line) && line != "") {
			data[index++] = line;
		}
		inFile.close();
	}

	void read(int lba) {
		loadDataFromFile();
		ofstream outFile("result.txt");
		outFile << data[lba] << std::endl;
		outFile.close();
	}
  
  void write(int address, std::string data) {
  }
  
};

