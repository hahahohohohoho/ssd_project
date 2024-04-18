#pragma once
#include<string>
#include<iostream>
#include <fstream>
#include <vector>
using namespace std;

class TestCase {
public:
    TestCase(string _cmd, string _name) : cmd(_cmd), name(_name) {

    }

    string getCmd() {
        return cmd;
    }

    string getName() {
        return name;
    }

private:
    string cmd;
    string name;
};

class TCManager {
public:
    TCManager(string filename) {
        vector<string> tclist = readLinesFromFile(filename);
        int tcid = 0;
        for (string tcname : tclist) {
            string cmd = "tc" + to_string(tcid);
            testcases.push_back(TestCase{ cmd, tcname });
            tcid++;
        }
	}

    vector<string> readLinesFromFile(const string& filename) {
        ifstream infile(filename);
        vector<string> lines;

        if (infile) {
            std::string line;
            while (std::getline(infile, line)) {
                lines.push_back(line);
            }

            infile.close();
        }
        else {
            cerr << "[TCManager][ERROR] file open failed." << endl;
        }

        return lines;
    }

    int run(string cmd) {
        
        for (TestCase tc : testcases) {
            if (cmd == tc.getCmd()) {
                string cmd = tc.getName() + ".exe";
                system(cmd.c_str());
                return 0;
            }
        }

        return -1;
    }

    void printTestCases() {
        cout << "\n[ TestCases ]" << endl;
        for (TestCase tc : testcases) {
            cout << "- " << tc.getCmd() << " : " << tc.getName() << endl;
        }
    }



private:
    vector<TestCase> testcases;
};