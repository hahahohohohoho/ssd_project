#pragma once

#include <vector>
#include <sstream>
#include "DataArrayFile.cpp"
#include "CmdString.h"

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

	bool isIncluded(int srcStart, int srcSize, int dstStart, int dstSize) {
		int srcEnd = srcStart + srcSize - 1;
		int dstEnd = dstStart + dstSize - 1;
		if (dstStart <= srcStart && srcEnd <= dstEnd) {
			return true;
		}
		return false;
	}

	bool isRemovable(CommandQueueItem newItem, CommandQueueItem item) {
		if (newItem.cmdName == CMD[ERASE]) {
			if (item.cmdName == CMD[WRITE] && isIncluded(stoi(item.parameter1), 1,
				stoi(newItem.parameter1), stoi(newItem.parameter2))) {
				return true;
			}
			if (item.cmdName == CMD[ERASE] && isIncluded(stoi(item.parameter1), stoi(item.parameter2),
				stoi(newItem.parameter1), stoi(newItem.parameter2))) {
				return true;
			}
		}
		else if (newItem.cmdName == CMD[WRITE]) {
			if (item.cmdName == CMD[WRITE] && stoi(item.parameter1) == stoi(newItem.parameter1)) {
				return true;
			}
		}
		return false;
	}

public:
	CommandQueue() {

	}

	void addItem(CommandQueueItem item) {
		vector<CommandQueueItem> items = getItems();

		for (auto iter = items.rbegin(); iter != items.rend();) {
			if (isRemovable(item, *iter)) {
				iter = decltype(iter)(items.erase(next(iter).base()));
				continue;
			}
			iter++;
		}

		items.push_back(item);
		setItems(items);
	}

	string findData(int lba) {
		vector<CommandQueueItem> items = getItems();

		for (auto iter = items.rbegin(); iter != items.rend(); iter++) {
			if (iter->cmdName == CMD[WRITE] && stoi(iter->parameter1) == lba) {
				return iter->parameter2;
			}

			if (iter->cmdName == CMD[ERASE] 
				&& isIncluded(lba, 1, stoi(iter->parameter1), stoi(iter->parameter2))) {
				return "0x00000000";
			}
		}

		return "";
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

	void setItems(vector<CommandQueueItem> items) {
		for (auto iter = items.begin(); iter != items.end(); iter++) {
			buffer[distance(items.begin(), iter)] = 
				iter->cmdName + " " + iter->parameter1 + " " + iter->parameter2;
		}
		file.writeFileLines(buffer, items.size());
	}
};
