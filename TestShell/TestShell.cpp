#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <exception>

#include "ISSD.h"
#include "CustomException.cpp"
#include "TCManager.cpp"
#include "CommandFactory.cpp"

using namespace std;

class TestShell {
public:
	explicit TestShell(ISSD* ssd) { 
		TCManager tcManager{ "tclist", TCManager::STDOUT_REDIRECTION_OFF };
		commander = new CommandProcessor(new CommandFactory(ssd, &tcManager));
	}

	void start() {
		while (1) {
			try {
				commander->processCommand(getCommand());
			}
			catch (InvalidInputException e) {
				cout << e.what() << endl;
				while (std::cin.get() != '\n');
			}
			catch (ExitException e) {
				cout << e.what() << endl;
				break;
			}
			catch (exception e) {
				cout << e.what() << endl;
				break;
			}
		}
	}

	string getCommand() {
		string command;
		cout << "\nCMD > ";
		cin >> command;
		return command;
	}

private:
	CommandProcessor* commander;
};