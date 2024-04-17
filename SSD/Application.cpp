#include "SSD.cpp"

class Application {
private:
	ISSD* ssd;
public:
	Application(ISSD* ssd) : ssd(ssd) {

	}
	void run(int argc, char* argv[]) {
		if (argc < 3 || argc > 4) {
			cout << "[Error] Need more parameter!" << endl;
			return;
		}
		
		if (argv[1][0] == 'R') {
			try {
				ssd->read(stoi(argv[2]));
			}
			catch (exception &e) {
				cout << e.what() << endl;
				cout << "[Error] Read Fail!" << endl;
			}
			return;
		}

		if (argv[1][0] == 'W') {
			try {
				ssd->write(stoi(argv[2]), argv[3]);
			}
			catch (exception &e) {
				cout << e.what() << endl;
				cout << "[Error] Write Fail!" << endl;
			}
			return;
		}

		cout << "[Error] Invalid parameters!" << endl;
	}
};
