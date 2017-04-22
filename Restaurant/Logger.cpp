#include "Logger.h"

#include <chrono>
using namespace std::chrono;

time_point<system_clock> Logger::beginPoint = system_clock::now();
std::mutex Logger::m_mutex;

Logger::Logger() : m_lock(m_mutex)
{
	auto now = system_clock::now();
	auto ms = duration_cast<milliseconds>((now - beginPoint)).count();

	std::cout << ms / 1000.0 << ") ";
}


Logger::~Logger()
{
	std::cout << std::endl;
}
