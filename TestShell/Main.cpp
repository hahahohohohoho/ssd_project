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
			std::cerr << "������ �� �� �����ϴ�." << std::endl;
			return 1;  // ���� �ڵ� 1�� �Բ� ���α׷� ����
		}

		while (std::getline(file, line)) {  // ���Ͽ��� �� �پ� �б�
			std::cout << line << std::endl;  // ���� �� ���
			shell.runScript(line);
		}

		file.close();  // ���� �ݱ�
	}
	else {
		cout << 3 << endl;

		shell.start();
	}

	return 0;
}