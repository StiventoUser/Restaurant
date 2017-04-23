#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <memory>
#include <chrono>

#include "RestaurantInternal.h"
#include "ThreadWrapper.h"
#include "Cook.h"
#include "Waiter.h"

using namespace std::chrono_literals;

namespace Restaurant
{
	class RestaurantCore
	{
	public:
		RestaurantCore();
		~RestaurantCore();

		void open();
		void close();

	private:
		bool RestaurantCore::waitForThreads(std::chrono::duration<long double> maxWaitTime, std::chrono::duration<long double> waitInterval = 1.0s);
	private:
		bool m_isOpened = false;
		std::shared_ptr<RestaurantInternal> m_internal;
		std::shared_ptr<ThreadWrapper<Cook>> m_cook;
		std::shared_ptr<ThreadWrapper<Waiter>> m_waiter;

		int m_currentOrder = 0;
	};
}

#endif