#include "TestShell.cpp"
#include "SsdDriver.cpp"

int main() {

	TestShell shell(new SSD_Driver("result.txt"));
	shell.start();

	return 0;
}