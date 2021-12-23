#include "GEngine\Console\GConsole.h"

void GConsole::LOG(const std::string& log)
{
	std::cout << RESET << log << '\n';
}

void GConsole::KEYLOG(const char& key)
{
	std::cout << MAG << "Key Down : " << key << RESET << '\n';
}

void GConsole::SUCCESS(const std::string& successLog)
{
	std::cout << GRN << successLog << RESET << '\n';
}

void GConsole::LOGERROR(const std::string& errorLog)
{
	std::cout << RED << errorLog << RESET << '\n';
}

void GConsole::WARNING(const std::string& warningLog)
{
	std::cout << YEL << warningLog << RESET << '\n';
}
