#include "Application.cpp"
#include "CmdString.h"

void makeCmdList(Application& app) {
	app.addCommand(new ReadCommand("Read", CMD[READ], 3));
	app.addCommand(new WriteCommand("Write", CMD[WRITE], 4));
	app.addCommand(new EraseCommand("Erase", CMD[ERASE], 4));
	app.addCommand(new FlushCommand("Flush", CMD[FLUSH], 2));
}

int main(int argc, char* argv[]) {
	Application app(new SSD());
	makeCmdList(app);
	app.run(argc, argv);
	return 0;
}
