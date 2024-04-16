#include "SSD.cpp"

class Application {
private:
	ISSD* ssd;
public:
	Application(ISSD* ssd) : ssd(ssd) {

	}
	void run(int argc, char* argv[]) {
		if (argc < 3 || argc > 4) {
			cout << "need more parameter!" << endl;
			return;
		}
		
		if (argv[1][0] == 'R') { 
			ssd->read(stoi(argv[2]));
			return;
		}

		if (argv[1][0] == 'W') {
			ssd->write(stoi(argv[2]), argv[3]);
			return;
		}

		cout << "Invalid parameters!" << endl;
	}
};
