#include "Waiter.h"
#include "Logger.h"

namespace Restaurant
{
	Waiter::Waiter()
	{
		Logger() << "Waiter is ready to work.";
	}

	Waiter::~Waiter()
	{
		Logger() << "Waiter has left the restaurant.";
	}
	void Waiter::work(std::weak_ptr<RestaurantInternal> internal)
	{
		m_internal = internal;

		while (true)
		{
			if (auto ptr = m_internal.lock())
			{
				Logger() << "Waiter is waiting...";
				if (!ptr->waitFor(RestaurantInternal::Signal::DishFinished, RestaurantInternal::SignalWaitTime))
				{
					Logger() << "Waiter is resting...";

					auto lock = ptr->lockData(RestaurantInternal::DataWaitTime);

					if (ptr->isRestaurantClosed())
					{
						Logger() << "Waiter is going to go home.";
						return;
					}

					continue;
				}

				{
					auto lock = ptr->lockData(RestaurantInternal::DataWaitTime);

					if (!lock.owns_lock())
					{
						Logger() << "Waiter can't get the dish!";
						Logger() << "Aborting...";
						abort();
					}

					Logger() << "The dish '" << ptr->getDishInfo().getDishName() << "' is delivering...";
					std::this_thread::sleep_for(DeliveringTime);
					Logger() << "The dish has been delivered.";

					ptr->notify(RestaurantInternal::Signal::DishDelivered);
				}

				Logger() << "Waiter is returing...";
				std::this_thread::sleep_for(DeliveringTime);
			}
			else
			{
				Logger() << "Does internal exist?";
				return;
			}
		}
	}
}