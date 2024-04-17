#include "TestShell.cpp"
#include "ExitStrategy.cpp"
#include "SsdDriver.cpp"

int main() {

	TestShell shell(new SSD_Driver("result.txt"), new RealExitStrategy());
	shell.start();

	return 0;
}