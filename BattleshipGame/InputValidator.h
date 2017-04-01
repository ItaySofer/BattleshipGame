#pragma once
class InputValidator
{
	char* path;
	
public:
	InputValidator(char* p) : path(p) {};

	bool isInputValid(); //Validates files exist and board is OK. Outputs propper messages.
};
