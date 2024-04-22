#include <map>
#include <memory>
#include "ShellCommand.cpp"
#include "TCManager.cpp"

class CommandFactory {
public:
    CommandFactory(ISSD* _ssd, TCManager* _tcManager) : ssd(_ssd), tcManager(_tcManager){ }

    std::unique_ptr<ShellCommand> createCommand(const std::string& command) {
        
        std::unique_ptr<ShellCommand> retCommand;

        if (command == "read") {
            getLba();
            retCommand = std::make_unique<ReadCommand>(this->start_lba);
        }
        else if (command == "write") {
            getLba();
            getValue();
            retCommand = std::make_unique<WriteCommand>(this->start_lba, this->value);
        }
        else if (command == "fullread") {
            retCommand = std::make_unique<FullReadCommand>();
        }
        else if (command == "fullwrite") {
            getValue();
            retCommand = std::make_unique<FullWriteCommand>(this->value);
        }
        else if (command == "erase") {
            getLba();
            getSize();
            retCommand = std::make_unique<EraseCommand>(this->start_lba, this->size);
        }
        else if (command == "erase_range") {
            getLba();
            getEndLba();
            retCommand = std::make_unique<EraseRangeCommand>(this->start_lba, this->end_lba);
        }
        else if (command == "help") {
            retCommand = std::make_unique<HelpCommand>();
        }
        else if (command == "exit") {
            retCommand = std::make_unique<ExitCommand>();
        }
        else {
            if (tcManager->run(command) < 0)
                retCommand = nullptr;
        }

        if (retCommand) {
            retCommand->setISSD(ssd);
            retCommand->setTCManager(tcManager);
        }

        return retCommand;
    }

private:
    TCManager* tcManager;
    ISSD* ssd;
    int start_lba, end_lba;
    int size;
    string value;

    bool isNumber(const std::string& str) {
        for (char c : str) {
            if (!std::isdigit(c)) {
                return false;
            }
        }
        return true;
    }

    void getValue() {
        cin >> this->value;
        if (this->value.size() != 10 || this->value.substr(0, 2) != "0x")
            throw InvalidInputException("Invalid Value");

        for (char ch_value : this->value.substr(2, 10)) {
            if ('0' <= ch_value && ch_value <= '9') continue;
            if ('A' <= ch_value && ch_value <= 'F') continue;
            throw InvalidInputException("Invalid Value");
        }
    }

    void getSize() {
        string size;
        cin >> size;

        if (!isNumber(size))
            throw InvalidInputException("Invalid Size");

        this->size = stoi(size);
        if (this->size < 0 || this->size > 10)
            throw InvalidInputException("Invalid Size");
    }

    void getLba() {
        string str_lba;
        cin >> str_lba;

        if (!isNumber(str_lba))
            throw InvalidInputException("Invalid Lba");

        this->start_lba = stoi(str_lba);
        if (0 > this->start_lba || this->start_lba > 99)
            throw InvalidInputException("Invalid Lba");
    }

    void getEndLba() {
        string end_lba;
        cin >> end_lba;

        if (!isNumber(end_lba))
            throw InvalidInputException("Invalid Lba");

        this->end_lba = stoi(end_lba);
        if (0 > this->end_lba || this->end_lba > 99)
            throw InvalidInputException("Invalid Lba");
    }
};

class CommandProcessor {
private:
    CommandFactory* commandFactory;

public:
    CommandProcessor(CommandFactory* _commandFactory) : commandFactory(_commandFactory) { }
    void processCommand(const std::string& command) {
        std::unique_ptr<ShellCommand> cmd = commandFactory->createCommand(command);
        if (cmd) {
            cmd->execute();
        }
        else {
            throw InvalidInputException("Invalid Command");
        }
    }
};