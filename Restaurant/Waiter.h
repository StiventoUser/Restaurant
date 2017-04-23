#ifndef WAITER_H
#define WAITER_H

#include <memory>
#include <chrono>

#include "RestaurantInternal.h"

using namespace std::chrono_literals;

namespace Restaurant
{
	class Waiter
	{
	public:
		Waiter();
		~Waiter();

		void work(std::weak_ptr<RestaurantInternal> internal);

	private:
		const std::chrono::duration<long double> DeliveringTime = 0.5s;
	private:
		std::weak_ptr<RestaurantInternal> m_internal;
	};
}

#endif