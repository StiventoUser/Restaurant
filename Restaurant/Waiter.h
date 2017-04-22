#ifndef WAITER_H
#define WAITER_H

#include <memory>
#include "RestaurantInternal.h"

namespace Restaurant
{
	class Waiter
	{
	public:
		Waiter();
		~Waiter();

		void work(std::weak_ptr<RestaurantInternal> internal);

		void dishFinished();

	private:
		std::weak_ptr<RestaurantInternal> m_internal;
	};
}

#endif