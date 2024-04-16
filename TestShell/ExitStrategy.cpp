#pragma once
#include "IExitStrategy.h"
#include <iostream>
#include <string>

class RealExitStrategy : public IExitStrategy {
public:
	void exitProgram(int status) override {
		std::exit(status);
	}
};

class TestExitStrategy : public IExitStrategy {
public:
	void exitProgram(int status) override {
		throw std::runtime_error("Program exit called with status " + std::to_string(status));
	}
};