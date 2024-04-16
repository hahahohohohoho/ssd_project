#pragma once

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class SSD {
private:
	string data[105];
public:
	void read(int lba) {
		ifstream inFile("nand.txt");
		string line;
		getline(inFile, line);
		inFile.close();

		for (int i = 0, j = 0; i < line.length(); i+=10) {
			data[j++] = line.substr(i, i + 10);
		}

		if (line == "") {
			data[lba] = "0x00000000";
		}

		ofstream outFile("result.txt");
		outFile << data[lba] << std::endl;
		outFile.close();
	}
};