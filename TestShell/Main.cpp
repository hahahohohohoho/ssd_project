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
			std::cerr << "������ �� �� �����ϴ�." << std::endl;
			log.print("������ �� �� �����ϴ�.");
			return 1;
		}

		TCManager* tcManager = new TCManager("tclist", TCManager::STDOUT_REDIRECTION_ON);

		while (std::getline(file, line)) {  // ���Ͽ��� �� �پ� �б�
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
		file.close();  // ���� �ݱ�
	}
	else {
		TestShell shell(new SSD_Driver("result.txt"));
		shell.start();
	}

	return 0;
}