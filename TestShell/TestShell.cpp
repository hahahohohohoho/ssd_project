#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <exception>

#include "ISSD.h"
#include "CustomException.cpp"
#include "TCManager.cpp"
#include "CommandFactory.cpp"
#include "Logger.h"

using namespace std;

class TestShell {
public:

	explicit TestShell(ISSD* ssd) {
		TCManager tcManager{ "tclist", TCManager::STDOUT_REDIRECTION_OFF };
		commander = new CommandProcessor(new CommandFactory(ssd, &tcManager));
		log = LoggerSingleton::GetInstancePtr();
	}

	void start() {
		while (1) {
			try {
				commander->processCommand(getCommand());
			}
			catch (InvalidInputException e) {
				cout << e.what() << endl;
				log->print(e.what());
				while (std::cin.get() != '\n');
			}
			catch (ExitException e) {
				cout << e.what() << endl;
				log->print(e.what());
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
		log->print("CMD > " + command);
		return command;
	}

private:
	CommandProcessor* commander;
	LoggerSingleton* log;
};