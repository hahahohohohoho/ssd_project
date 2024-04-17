#include "TestShell.cpp"
#include "ExitStrategy.cpp"

int main() {

	TestShell shell(new SSD_Driver("result.txt"));
	shell.setExitStrategy(new RealExitStrategy());
	shell.start();


	return 0;
}