#ifndef LOGGER_H
#define LOGGER_H

#include <mutex>
#include <iostream>
#include <chrono>

class Logger
{
public:
	Logger();
	~Logger();

	template<typename T>
	Logger& operator<<(const T& value)
	{
		std::cout << value;

		return *this;
	}

private:
	static std::mutex m_mutex;
	static std::chrono::time_point<std::chrono::system_clock> beginPoint;
	
	std::unique_lock<std::mutex> m_lock;
};

#endif