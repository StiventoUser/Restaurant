#ifndef COOK_H
#define COOK_H

#include <memory>

#include "RestaurantInternal.h"

namespace Restaurant
{
	class Cook
	{
	public:
		Cook();
		~Cook();

		void work(std::weak_ptr<RestaurantInternal> internal);

		void dishRequested();

	private:
		std::weak_ptr<RestaurantInternal> m_internal;
	};
}

#endif