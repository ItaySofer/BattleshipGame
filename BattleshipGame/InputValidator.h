#pragma once
class InputValidator
{
	char* path;
	
public:
	InputValidator(char* p) : path(p) {};

	bool validateInput(); //Validates files exist and board is OK. Outputs propper messages.
};
