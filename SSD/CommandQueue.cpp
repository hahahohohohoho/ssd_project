#pragma once

#include <vector>
#include <sstream>
#include "DataArrayFile.cpp"

using namespace std;

static const int MAX_ITEM_SIZE = 10;

struct CommandQueueItem {
	string cmdName;
	string parameter1;
	string parameter2;
};

class CommandQueue {
private:
	string buffer[MAX_ITEM_SIZE];
	DataArrayFile file{ "buffer.txt" };

	vector<string> splitString(const string& input, char delim) {
		vector<string> tokens;
		istringstream iss(input);
		string token;

		while (getline(iss, token, delim)) {
			tokens.push_back(token);
		}

		return tokens;
	}

public:
	CommandQueue() {

	}

	void addItem(CommandQueueItem item) {
		int size = file.readFileLines(buffer, MAX_ITEM_SIZE);
		buffer[size++] =
			item.cmdName + " " + item.parameter1 + " " + item.parameter2;
		file.writeFileLines(buffer, size);
	}

	bool isFull() {
		int ret = file.readFileLines(buffer, MAX_ITEM_SIZE);
		return (ret == MAX_ITEM_SIZE);
	}

	void clear() {
		buffer[0] = "";
		file.writeFileLines(buffer, 1);
	}

	vector<CommandQueueItem> getItems() {
		vector<CommandQueueItem> ret;
		int size = file.readFileLines(buffer, MAX_ITEM_SIZE);
		for (int i = 0; i < size; ++i) {
			vector<string> tokens = splitString(buffer[i], ' ');
			ret.push_back({ tokens[0], tokens[1], tokens[2] });
		}
		return ret;
	}

	void removeItem(int index) {
		string newBuffer[MAX_ITEM_SIZE];
		int size = file.readFileLines(buffer, MAX_ITEM_SIZE);
		for (int i = 0; i < size; i++) {
			if (i == index)
				continue;
			newBuffer[i] = buffer[i];
		}
		file.writeFileLines(newBuffer, size);
	}
};
