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

public:
	CommandQueue() {

	}

	void addItem(CommandQueueItem item) {
		vector<CommandQueueItem> items = getItems();
		reverse(items.begin(), items.end());

		if (item.cmdName == CMD[ERASE]) {
			int itemFirstLba = stoi(item.parameter1);
			int itemLastLba = itemFirstLba + stoi(item.parameter2) - 1;
			for (auto iter = items.begin(); iter != items.end();) {
				if (iter->cmdName == CMD[WRITE]) {
					int writeLba = stoi(iter->parameter1);
					if (writeLba >= itemFirstLba && writeLba <= itemLastLba) {
						iter = items.erase(iter);
						continue;
					}
				}
				else if (iter->cmdName == CMD[ERASE]) {
					int eraseFirstLba = stoi(iter->parameter1);
					int eraseLastLba = eraseFirstLba + stoi(iter->parameter2) - 1;
					if (eraseFirstLba >= itemFirstLba && eraseLastLba <= itemLastLba) {
						iter = items.erase(iter);
						continue;
					}
				}
				iter++;
			}
		}
		else if (item.cmdName == CMD[WRITE]) {
			int itemLba = stoi(item.parameter1);

			for (auto iter = items.begin(); iter != items.end();) {
				if (iter->cmdName == CMD[WRITE]) {
					int writeLba = stoi(iter->parameter1);
					if (writeLba == itemLba) {
						iter = items.erase(iter);
						continue;
					}
				}
				iter++;
			}
		}

		reverse(items.begin(), items.end());
		items.push_back(item);
		setItems(items);
	}

	string findData(int lba) {
		vector<CommandQueueItem> items = getItems();
		reverse(items.begin(), items.end());

		for (auto iter = items.begin(); iter != items.end(); iter++) {
			if (iter->cmdName == CMD[WRITE] && stoi(iter->parameter1) == lba) {
				return iter->parameter2;
			}
			else if (iter->cmdName == CMD[ERASE] 
				&& lba >= stoi(iter->parameter1) && lba < stoi(iter->parameter1) + stoi(iter->parameter2)) {
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
