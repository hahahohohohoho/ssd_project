#pragma once
#include "IExitStrategy.h"
#include <iostream>
#include <string>

class RealExitStrategy : public IExitStrategy {
public:
	void exitProgram() override {
		std::exit(0);
	}
};

class TestExitStrategy : public IExitStrategy {
public:
	void exitProgram() override {
		throw std::runtime_error("Program exit called");
	}
};