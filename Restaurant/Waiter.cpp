#include "Waiter.h"
#include "Logger.h"

namespace Restaurant
{
	Waiter::Waiter()
	{
		Logger() << "Waiter constructed.";
	}


	Waiter::~Waiter()
	{
		Logger() << "Waiter destroyed.";
	}
	void Waiter::work(std::weak_ptr<RestaurantInternal> internal)
	{
		using namespace std::chrono_literals;

		m_internal = internal;

		while (true)//TODO
		{
			if (auto ptr = m_internal.lock())
			{
				Logger() << "Waiter is waiting...";
				if (!ptr->waitFor(RestaurantInternal::Signal::DishFinished, 10.0s))
				{
					Logger() << "Waiter is resting...";

					if (ptr->isRestaurantClosed())
					{
						Logger() << "Waiter is going to go home.";
						return;
					}

					continue;
				}

				{
					auto lock = ptr->lockData(10.0s);

					if (!lock.owns_lock())
					{
						Logger() << "Waiter can't get the dish!";
						Logger() << "Aborting...";
						abort();
					}

					Logger() << "The dish '" << ptr->getDishInfo().getDishName() << "' is delivering...";
					std::this_thread::sleep_for(0.5s);
					Logger() << "The dish is delivered.";

					ptr->notify(RestaurantInternal::Signal::DishDelivered);
				}

				Logger() << "Waiter is returing...";
				std::this_thread::sleep_for(0.5s);
			}
		}
	}

	void Waiter::dishFinished()
	{
	}
}