#include "TestShell.cpp"
#include "ExitStrategy.cpp"
#include "SsdDriver.cpp"

int main() {

	TestShell shell;
	shell.setExitStrategy(new RealExitStrategy());
	shell.setSsd(new SSD_Driver("result_dummy.txt"));
	shell.start();

	return 0;
}