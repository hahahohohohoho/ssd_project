#include "Application.cpp"

void makeCmdList(Application& app) {
	app.addCommand(new ReadCommand());
	app.addCommand(new WriteCommand());
	app.addCommand(new EraseCommand());
	app.addCommand(new FlushCommand());
}

int main(int argc, char* argv[]) {
	Application app(new SSD());
	makeCmdList(app);
	app.run(argc, argv);
	return 0;
}
