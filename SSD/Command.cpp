#include <string>
#include "SSD.cpp"

using namespace std;

class Command {
protected:
	string title;
	string commandName;
	int requiredArgCnt;
public:
	Command(string title, string commandName, int requiredArgCnt)
		: title(title), commandName(commandName), requiredArgCnt(requiredArgCnt) {

	}

	string getTitle() {
		return title;
	}

	string getCommandName() {
		return commandName;
	}

	bool isCorrectArgCount(const int argc) const {
		return (requiredArgCnt == argc);
	}
	virtual void execute(ISSD* ssd, char* argv[]) = 0;
};

class ReadCommand : public Command {
public:
	ReadCommand()
		: Command("Read", CMD[READ], 3) {

	}

	void execute(ISSD* ssd, char* argv[]) override {
		ssd->read(stoi(argv[2]));
	}
};

class WriteCommand : public Command {
public:
	WriteCommand()
		: Command("Write", CMD[WRITE], 4) {

	}

	void execute(ISSD* ssd, char* argv[]) override {
		ssd->write(stoi(argv[2]), argv[3]);
	}
};

class EraseCommand : public Command {
public:
	EraseCommand()
		: Command("Erase", CMD[ERASE], 4) {

	}

	void execute(ISSD* ssd, char* argv[]) override {
		ssd->erase(stoi(argv[2]), stoi(argv[3]));
	}
};

class FlushCommand : public Command {
public:
	FlushCommand()
		: Command("Flush", CMD[FLUSH], 2) {

	}

	void execute(ISSD* ssd, [[maybe_unused]] char* argv[]) override {
		ssd->flush();
	}
};
