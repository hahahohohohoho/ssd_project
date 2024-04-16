#include <iostream>
#include <fstream>
#include <string>

class SSD {
public:
	void read(int lba) {
		std::ofstream outFile("result.txt");
		outFile << "0x00000000" << std::endl;
		outFile.close();
	}
};