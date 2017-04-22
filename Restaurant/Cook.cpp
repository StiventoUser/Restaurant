#include "Cook.h"

#include <chrono>

#include "Logger.h"


namespace Restaurant
{
	Cook::Cook()
	{
		Logger() << "Cook constructed.";
	}


	Cook::~Cook()
	{
		Logger() << "Cook destroyed.";
	}
	void Cook::work(std::weak_ptr<RestaurantInternal> internal)
	{
		using namespace std::chrono_literals;

		m_internal = internal;

		while (true)//TODO
		{
			if (auto ptr = m_internal.lock())
			{
				Logger() << "Cook is waiting...";
				if (!ptr->waitFor(RestaurantInternal::Signal::DishRequested, 10.0s))
				{
					Logger() << "Cook is resting...";
					
					if (ptr->isRestaurantClosed())
					{
						Logger() << "Cook is going to go home.";
						return;
					}

					continue;
				}

				auto lock = ptr->lockData(10.0s);

				if (!lock.owns_lock())
				{
					Logger() << "Cook can't get the request!";
					Logger() << "Aborting...";
					abort();
				}

				Logger() << "The dish '" << ptr->getRequestedDishName() << "' is preparing...";
				std::this_thread::sleep_for(2.0s);
				Logger() << "The dish is prepared.";

				ptr->setDishInfo(DishInfo(ptr->getRequestedDishName()));

				ptr->notify(RestaurantInternal::Signal::DishFinished);
			}
		}
	}

	void Cook::dishRequested()
	{
	}
}