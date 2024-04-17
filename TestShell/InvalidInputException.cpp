#pragma once
#include <iostream>

using namespace std;

class InvalidInputException : public exception {
public:
	explicit InvalidInputException(char const* message) : exception(message) {

	}
};