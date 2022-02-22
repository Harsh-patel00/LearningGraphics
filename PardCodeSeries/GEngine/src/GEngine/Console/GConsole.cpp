#include "GEngine\Console\GConsole.h"

void GConsole::LOG(const std::string& log)
{
	std::wclog << RESET << log.c_str() << '\n';
}

void GConsole::KEYLOG(const char& key)
{
	std::wclog << MAG << "Key Down : " << key << RESET << '\n';
}

void GConsole::SUCCESS(const std::string& successLog)
{
	std::wclog << GRN << successLog.c_str() << RESET << '\n';
}

void GConsole::LOGERROR(const std::string& errorLog)
{
	std::wclog << RED << errorLog.c_str() << RESET << '\n';
}

void GConsole::WARNING(const std::string& warningLog)
{
	std::wclog << YEL << warningLog.c_str() << RESET << '\n';
}
