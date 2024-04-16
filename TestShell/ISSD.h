#pragma once
#include <string>
using namespace std;

class ISSD {
public:
	virtual string read() = 0;
	virtual void write(int lba, string value) = 0;
};