#include <iostream>
#include "ISSD.h"
#include "CustomException.cpp"
#include "TCManager.cpp"

using namespace std;

class ShellCommand {

public:
    virtual void execute() = 0;

    void setISSD(ISSD* _ssd) {
        ssd = _ssd;
    }

    void setTCManager(TCManager* _tcManager) {
        tcManager = _tcManager;
    }

protected:
    ISSD* ssd;
    TCManager* tcManager;

    string read(int lba) {
        string value = ssd->read(lba);
        cout << value << endl;

        return value;
    }

    void write(int lba, string value) {
        ssd->write(lba, value);
    }
};

class ReadCommand : public ShellCommand {
private:
    int lba;

public:
    ReadCommand(int lba) : lba(lba) {}
    virtual void execute() {
        read(lba);
    }
};

class WriteCommand : public ShellCommand {
private:
    int lba;
    string value;

public:
    WriteCommand(int lba, string value) : lba(lba), value(value) {}

    virtual void execute() {
        for (int lba = 0; lba < 100; lba++) {
            write(lba, value);
        }
    }
};

class FullReadCommand : public ShellCommand {
public:
    virtual void execute() {
        for (int lba = 0; lba < 100; lba++) {
            read(lba);
        }
    }
};

class FullWriteCommand : public ShellCommand {
private:
    string value;

public:
    FullWriteCommand(string value) : value(value) {}

    virtual void execute() {
        for (int lba = 0; lba < 100; lba++) {
            write(lba, value);
        }
    }
};

class EraseCommand : public ShellCommand {
private:
    int lba;
    int size;

public:
    EraseCommand(int lba, int size) : lba(lba), size(size) {}

    virtual void execute() {
        ssd->erase(lba, size);
    }
};

class EraseRangeCommand : public ShellCommand {
private:
    int start_lba;
    int end_lba;

public:
    EraseRangeCommand(int start_lba, int end_lba) : start_lba(start_lba), end_lba(end_lba) {}

    virtual void execute() {
        int start = start_lba;
        while (end_lba >= start) {
            int rem = end_lba - start + 1;
            int size = (rem > 10) ? (10) : (rem);
            ssd->erase(start, size);
            start += 10;
        }
    }
};

class ExitCommand : public ShellCommand {
public:
    virtual void execute() {
        throw ExitException("Exit Program");
    }
};

class HelpCommand : public ShellCommand {
public:
    virtual void execute() {
        cout
            << "[ CMD ]\n"
            << "- write {no} {data} : {data} was recorded in LBA {no}\n"
            << "-- {data} : hexadecimal \n"
            << "-- ex. write 3 0xAAAABBBB\n"
            << "- read {no} : Read LBA {no} times\n"
            << "- fullwrite {value} : 0~99 LBA Write\n"
            << "- fullread : 0~99 LBA Read\n"
            << "- erase {lba} {size}\n"
            << "- erase_range {start lba} {end lba}\n"
            << "- exit : shell exits\n"
            << "- help : Displays how to use each command\n";
        tcManager->printTestCases();
    }
};
