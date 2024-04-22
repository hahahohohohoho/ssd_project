#include "TestShell.cpp"
#include "SsdDriver.cpp"
#include "TCManager.cpp"
#include "Runner.cpp"

#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include "TCResult.h"

int main(int argc, char* argv[]) {
	if (argc > 1) {
		Runner runner;
		runner.start(argv[1]);
	}
	else {
		TestShell shell(new SSD_Driver("result.txt"));
		shell.start();
	}

	return 0;
}