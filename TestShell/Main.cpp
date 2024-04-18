#include "TestShell.cpp"
#include "SsdDriver.cpp"

#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>

int main(int argc, char* argv[]) {

	cout << 0 << endl;

	TestShell shell(new SSD_Driver("result.txt"));

	cout << 1 << endl;
	cout << argc << endl;
	cout << argv[1] << endl;

	if (argc > 1) {

		cout << 2 << endl;

		//string str = argv[1];

		string temp = "run_list.lst";
		std::ifstream file(argv[1]);

		std::string line;
		if (file.is_open()) {
			getline(file, line);
		}
		if (!file.is_open()) {
			std::cerr << "파일을 열 수 없습니다." << std::endl;
			return 1;  // 에러 코드 1과 함께 프로그램 종료
		}

		while (std::getline(file, line)) {  // 파일에서 한 줄씩 읽기
			std::cout << line << std::endl;  // 읽은 줄 출력
			shell.runScript(line);
		}

		file.close();  // 파일 닫기
	}
	else {
		cout << 3 << endl;

		shell.start();
	}

	return 0;
}