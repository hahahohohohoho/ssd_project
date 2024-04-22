#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include "TestShell.cpp"
#include "SsdDriver.cpp"
#include "TCManager.cpp"
#include "TCResult.h"
#include "Logger.h"

int main(int argc, char* argv[]) {
	LoggerSingleton& log = LoggerSingleton::GetInstanceRef();

	if (argc > 1) {
		std::ifstream file(argv[1]);

		std::string line;
		if (!file.is_open()) {
			std::cerr << "파일을 열 수 없습니다." << std::endl;
			log.print("파일을 열 수 없습니다.");
			return 1;
		}

		TCManager* tcManager = new TCManager("tclist", TCManager::STDOUT_REDIRECTION_ON);

		while (std::getline(file, line)) {  // 파일에서 한 줄씩 읽기
			std::cout << line << "   ---   Run...";
			log.print(line + string("   ---   Run..."));
			if (tcManager->run(line) == TCResult::PASS) {
				cout << "Pass\n";
				log.print("Pass");
			}
			else {
				cout << "FAIL!\n";
				log.print("FAIL!");
				break;
			}
		}
		file.close();  // 파일 닫기
	}
	else {
		TestShell shell(new SSD_Driver("result.txt"));
		shell.start();
	}

	return 0;
}