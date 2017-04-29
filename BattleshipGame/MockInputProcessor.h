#pragma once
#include "InputProcessor.h"

class MockInputProcessor : public InputProcessor {

	std::string staticBoardFilePath;

public:

	MockInputProcessor(int argc, char* argv[]) : InputProcessor(argc, argv), staticBoardFilePath(argv[1]) {};

	std::string getBoardFilePath() const override {
		return staticBoardFilePath;
	}
};