#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <memory>

#include "RestaurantInternal.h"
#include "ThreadWrapper.h"
#include "Cook.h"
#include "Waiter.h"

namespace Restaurant
{
	class RestaurantCore
	{
	public:
		RestaurantCore();
		~RestaurantCore();

		void open();
		void close();

		void dishDelivered();

	private:
		const int MaxOrdersCount = 10;
	private:
		std::shared_ptr<RestaurantInternal> m_internal;
		std::shared_ptr<ThreadWrapper<Cook>> m_cook;
		std::shared_ptr<ThreadWrapper<Waiter>> m_waiter;

		int m_currentOrder = 0;
	};
}

#endif