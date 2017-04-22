#include "Restaurant.h"

#include <vector>

#include "Logger.h"

namespace Restaurant
{
	RestaurantCore::RestaurantCore()
	{
		m_internal = std::make_unique<RestaurantInternal>();
		m_cook = std::make_shared<ThreadWrapper<Cook>>();
		m_waiter = std::make_shared<ThreadWrapper<Waiter>>();

		m_internal->setSignalHandler(RestaurantInternal::Signal::DishRequested, std::bind(&Cook::dishRequested, m_cook->getObject()));
		m_internal->setSignalHandler(RestaurantInternal::Signal::DishFinished, std::bind(&Waiter::dishFinished, m_waiter->getObject()));
		m_internal->setSignalHandler(RestaurantInternal::Signal::DishDelivered, std::bind(&RestaurantCore::dishDelivered, this));

		Logger() << "Initialized.";
	}


	RestaurantCore::~RestaurantCore()
	{
		Logger() << "Restaurant destroyed.";
	}
	void RestaurantCore::open()
	{
		using namespace std::chrono_literals;

		Logger() << "OPENED";

		m_cook->execute(&Cook::work, std::weak_ptr<RestaurantInternal>(m_internal));
		m_waiter->execute(&Waiter::work, std::weak_ptr<RestaurantInternal>(m_internal));

		std::vector<std::string> dishes = { "Neapolitan pizza", "Sushi", "Peking duck", "Lasagna",
											"Butter garlic crab", "Fajitas", "Parma ham", "Ratatouille", 
											"Spaghetti alla Carbonara", "Roast Chicken with Potatoes and Vegetables" };

		for (int i = 0; i < MaxOrdersCount; ++i)
		{
			Logger() << "We have a new request!";

			{
				auto lock = m_internal->lockData(10.0s);

				if (!lock.owns_lock())
				{
					Logger() << "The data is inaccessible.";
					Logger() << "Aborting...";
					abort();
				}

				m_internal->setRequstedDishName(dishes[i]);
			}

			m_internal->notify(RestaurantInternal::Signal::DishRequested);

			if (!m_internal->waitFor(RestaurantInternal::Signal::DishDelivered, 10.0s))
			{
				Logger() << "Is anybody working?";
				Logger() << "Trying to close the restaurant...";

				auto lock = m_internal->lockData(10.0s);

				if (lock.owns_lock())
				{
					m_internal->closeRestaurant();

					Logger() << "Success.";
					return;
				}
				else
				{
					Logger() << "Fail.";
					Logger() << "Aborting...";
					abort();
				}
			}	
		}

		Logger() << "The restaurant is closing...";
		{
			auto lock = m_internal->lockData(10.0s);

			if (lock.owns_lock())
			{
				m_internal->closeRestaurant();

				Logger() << "Success.";
				return;
			}
			else
			{
				Logger() << "Fail.";
				Logger() << "Aborting...";
				abort();
			}
		}
	}
	void RestaurantCore::close()
	{
		Logger() << "CLOSED";
	}
	void RestaurantCore::dishDelivered()
	{
	}
}