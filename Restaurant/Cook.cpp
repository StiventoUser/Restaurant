#include "Cook.h"

#include "Logger.h"


namespace Restaurant
{
	Cook::Cook()
	{
		Logger() << "Cook is ready to work.";
	}


	Cook::~Cook()
	{
		Logger() << "Cook has left the restaurant.";
	}

	void Cook::work(std::weak_ptr<RestaurantInternal> internal)
	{
		m_internal = internal;

		while (true)
		{
			if (auto ptr = m_internal.lock())
			{
				Logger() << "Cook is waiting...";
				if (!ptr->waitFor(RestaurantInternal::Signal::DishRequested, RestaurantInternal::SignalWaitTime))
				{
					Logger() << "Cook is resting...";

					auto lock = ptr->lockData(RestaurantInternal::DataWaitTime);
					
					if (ptr->isRestaurantClosed())
					{
						Logger() << "Cook is going to go home.";
						return;
					}

					continue;
				}

				auto lock = ptr->lockData(RestaurantInternal::DataWaitTime);

				if (!lock.owns_lock())
				{
					Logger() << "Cook can't get the request!";
					Logger() << "Aborting...";
					abort();
				}

				Logger() << "The dish '" << ptr->getRequestedDishName() << "' is preparing...";
				std::this_thread::sleep_for(CookingTime);
				Logger() << "The dish has been prepared.";

				ptr->setDishInfo(DishInfo(ptr->getRequestedDishName()));

				ptr->notify(RestaurantInternal::Signal::DishFinished);
			}
			else
			{
				Logger() << "Does internal exist?";
				return;
			}
		}
	}
}