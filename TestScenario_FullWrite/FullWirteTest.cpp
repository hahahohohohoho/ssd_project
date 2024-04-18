#include <string>
#include <iostream>
#include "../TestShell/SsdDriver.cpp"

using namespace std;

void fullWriteTest() {
	SSD_Driver* ssd = new SSD_Driver("result.txt");

	string writeValue = "0x01234567";
	
	for (int lba = 0; lba < 100; lba++)
		ssd->write(lba, writeValue);

	for (int lba = 0; lba < 100; ++lba) {
		string value = ssd->read(lba).substr(0, 10);
		if (value != writeValue) {
			cout << "FullWrite Test Failed - LBA : " << lba << ", value : " << value << endl;
			return;
		}
	}

	cout << "[DONE] FullWrite Test Success" << endl;
}

int main() {
	cout << "[FullWrite Test] Start" << endl;
	fullWriteTest();
}