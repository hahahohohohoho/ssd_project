#pragma once

#include <fstream>
#include <string>
#include <cstdio>

#include "TCManager.cpp"

using namespace std;

class Runner {
public:
	void start(string fileName) {
		std::ifstream file(fileName);

		if (!file.is_open()) {
			std::cerr << "파일을 열 수 없습니다." << std::endl;
			return;
		}

		TCManager* tcManager = new TCManager("tclist", TCManager::STDOUT_REDIRECTION_ON);

		std::string line;
		while (std::getline(file, line)) {
			if (tcManager->run(line) == TCResult::PASS) {
				cout << "Pass\n";
			}
			else {
				cout << "FAIL!\n";
				break;
			}
		}
		file.close();
	}
};