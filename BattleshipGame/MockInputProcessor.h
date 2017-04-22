#pragma once
#include "InputProcessor.h"

class MockInputProcessor : public InputProcessor {

	std::string staticBoardFilePath;

public:

	MockInputProcessor(std::string p) : InputProcessor(""), staticBoardFilePath(p) {};

	std::string getBoardFilePath() override {
		return staticBoardFilePath;
	}
};