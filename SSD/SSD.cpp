#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class SSD {
public:
	void read(int lba) {
		ifstream inFile("nand.txt");
		string line;
		getline(inFile, line);
		inFile.close();

		if (line == "") {
			line = "0x00000000";
		}

		ofstream outFile("result.txt");
		outFile << line << std::endl;
		outFile.close();
	}
};