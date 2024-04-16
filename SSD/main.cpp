#include "Application.cpp"

int main(int argc, char* argv[]) {
	Application app(new SSD());
	app.run(argc, argv);
	return 0;
}
