#ifndef COOK_H
#define COOK_H

#include <memory>
#include <chrono>

#include "RestaurantInternal.h"

using namespace std::chrono_literals;

namespace Restaurant
{
	class Cook
	{
	public:
		Cook();
		~Cook();

		void work(std::weak_ptr<RestaurantInternal> internal);

	private:
		const std::chrono::duration<long double> CookingTime = 2.0s;
	private:
		std::weak_ptr<RestaurantInternal> m_internal;
	};
}

#endif