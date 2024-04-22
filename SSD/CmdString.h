#pragma once

#include <string>

const std::string CMD[5] = { "R","W","E","F" };
const enum {
	READ = 0,
	WRITE,
	ERASE,
	FLUSH
};