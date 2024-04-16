#pragma once
class IExitStrategy {
public:
	virtual void exitProgram(int status) = 0;
};