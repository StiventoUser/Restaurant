#include "Restaurant.h"

#include <vector>
#include <thread>

#include "Logger.h"

namespace Restaurant
{
	RestaurantCore::RestaurantCore()
	{
		try
		{
			m_internal = std::make_unique<RestaurantInternal>();
			m_cook = std::make_shared<ThreadWrapper<Cook>>();
			m_waiter = std::make_shared<ThreadWrapper<Waiter>>();
		}
		catch (const std::exception&)
		{
			Logger() << "Can't construct the restaurant.";
			throw;
		}

		Logger() << "Initialized.";
	}


	RestaurantCore::~RestaurantCore()
	{
		if (m_isOpened)
			close();

		Logger() << "Restaurant destroyed.";
	}
	void RestaurantCore::open()
	{
		Logger() << "OPENED";
		m_isOpened = true;

		m_cook->execute(&Cook::work, std::weak_ptr<RestaurantInternal>(m_internal));
		m_waiter->execute(&Waiter::work, std::weak_ptr<RestaurantInternal>(m_internal));

		std::vector<std::string> dishes = { "Neapolitan pizza", "Sushi", "Peking duck", "Lasagna",
											"Butter garlic crab", "Fajitas", "Parma ham", "Ratatouille", 
											"Spaghetti alla Carbonara", "Roast Chicken with Potatoes and Vegetables" };

		for(auto& dish : dishes)
		{
			Logger() << "We have a new request!";

			{
				auto lock = m_internal->lockData(RestaurantInternal::DataWaitTime);

				if (!lock.owns_lock())
				{
					Logger() << "The data is inaccessible.";
					Logger() << "Aborting...";
					abort();
				}

				m_internal->setRequstedDishName(dish);
			}

			m_internal->notify(RestaurantInternal::Signal::DishRequested);

			if (!m_internal->waitFor(RestaurantInternal::Signal::DishDelivered, RestaurantInternal::SignalWaitTime))
			{
				Logger() << "Is anybody working?";
				Logger() << "Trying to close the restaurant...";

				{
					auto lock = m_internal->lockData(RestaurantInternal::DataWaitTime);

					if (lock.owns_lock())
					{
						m_internal->closeRestaurant();

						Logger() << "The restaurant is closed.";
					}
					else
					{
						Logger() << "Fail.";
						Logger() << "Aborting...";
						abort();
					}
				}

				if (waitForThreads(RestaurantInternal::ThreadShutdownWaitTime))
				{
					Logger() << "The restaurant is empty.";
					return;
				}
				else
				{
					Logger() << "Someone doesn't want to leave the restaurant.";
					Logger() << "Aborting!";
					abort();
				}
			}	
		}

		Logger() << "The restaurant is closing...";
		{
			{
				auto lock = m_internal->lockData(RestaurantInternal::DataWaitTime);

				if (lock.owns_lock())
				{
					m_internal->closeRestaurant();

					Logger() << "The restaurant is closed.";
				}
				else
				{
					Logger() << "Fail.";
					Logger() << "Aborting...";
					abort();
				}
			}

			if (waitForThreads(RestaurantInternal::ThreadShutdownWaitTime))
			{
				Logger() << "The restaurant is empty.";
				return;
			}
			else
			{
				Logger() << "Someone doesn't want to leave the restaurant.";
				Logger() << "Aborting!";
				abort();
			}
		}
	}
	void RestaurantCore::close()
	{
		Logger() << "CLOSED";
		m_isOpened = false;
	}
	
	bool RestaurantCore::waitForThreads(std::chrono::duration<long double> maxWaitTime, std::chrono::duration<long double> waitInterval)
	{
		while (maxWaitTime.count() > 0)
		{
			Logger() << "Checking threads...";

			if (m_cook->isFinished() && m_waiter->isFinished())
				return true;

			maxWaitTime = maxWaitTime - waitInterval;
			std::this_thread::sleep_for(waitInterval);
		}

		return m_cook->isFinished() && m_waiter->isFinished();
	}
}