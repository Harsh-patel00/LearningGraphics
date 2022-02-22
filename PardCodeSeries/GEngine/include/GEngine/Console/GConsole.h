#pragma once
#include <iostream>
#include <sstream>


#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"


class GConsole
{
public:
	void static LOG(const std::string&);
	void static KEYLOG(const char&);
	void static SUCCESS(const std::string&);
	void static LOGERROR(const std::string&);
	void static WARNING(const std::string&);

	
};