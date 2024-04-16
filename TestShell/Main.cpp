#include "TestShell.cpp"
#include "ExitStrategy.cpp"

int main() {

	TestShell shell;
	shell.setExitStrategy(new RealExitStrategy());
	shell.start();


	return 0;
}