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
		commander = new CommandProcessor(
			new CommandFactory(
				ssd, new TCManager("tclist", TCManager::STDOUT_REDIRECTION_OFF)));
		log = LoggerSingleton::GetInstancePtr();
	}

	void start() {
		while (1) {
			try {
				processCommand();
			}
			catch (InvalidInputException e) {
				log->print(e.what());
				while (std::cin.get() != '\n');
			}
			catch (ExitException e) {
				log->print(e.what());
				break;
			}
			catch (exception e) {
				cout << e.what() << endl;
				break;
			}
		}
	}

	void processCommand() {
		commander->processCommand(getCommand());
	}

	string getCommand() {
		string command;
		cout << "\nCMD > ";
		cin >> command;
		log->print("Input Command : " + command);
		return command;
	}

private:
	CommandProcessor* commander;
	CommandFactory* command_factory;
	LoggerSingleton* log;
};