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

		std::string line;
		if (!file.is_open()) {
			std::cerr << "파일을 열 수 없습니다." << std::endl;
			return;
		}

		TCManager* tcManager = new TCManager("tclist", TCManager::STDOUT_REDIRECTION_ON);

		while (std::getline(file, line)) {  // 파일에서 한 줄씩 읽기
			std::cout << line << "   ---   Run...";
			if (tcManager->run(line) == TCResult::PASS) {
				cout << "Pass\n";
			}
			else {
				cout << "FAIL!\n";
				break;
			}
		}
		file.close();  // 파일 닫기
	}
};