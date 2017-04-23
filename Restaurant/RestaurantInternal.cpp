#include "RestaurantInternal.h"
#include "Logger.h"

namespace Restaurant
{
	const std::chrono::duration<long double> RestaurantInternal::SignalWaitTime = 5.0s;
	const std::chrono::duration<long double> RestaurantInternal::DataWaitTime = 10.0s;
	const std::chrono::duration<long double> RestaurantInternal::ThreadShutdownWaitTime = 15.0s;

	RestaurantInternal::RestaurantInternal()
	{
		Logger() << "Internal constructed.";
	}


	RestaurantInternal::~RestaurantInternal()
	{
		Logger() << "Internal destroyed.";
	}

	const std::string& RestaurantInternal::getRequestedDishName() const
	{
		return m_requestedDishName;
	}

	void RestaurantInternal::setRequstedDishName(const std::string dishName)
	{
		m_requestedDishName = dishName;
	}

	const DishInfo& RestaurantInternal::getDishInfo() const
	{
		return m_finishedDishInfo;
	}

	void RestaurantInternal::setDishInfo(const DishInfo& dishInfo)
	{
		m_finishedDishInfo = dishInfo;
	}

	void RestaurantInternal::closeRestaurant()
	{
		m_isRestaurantClosed = true;
	}

	bool RestaurantInternal::isRestaurantClosed() const
	{
		return m_isRestaurantClosed;
	}

	std::unique_lock<std::timed_mutex> RestaurantInternal::lockData(std::chrono::duration<long double> maxWaitTime)
	{
		return std::unique_lock<std::timed_mutex>(m_dataMutex, maxWaitTime);
	}
	void RestaurantInternal::notify(Signal signal)
	{
		{
			std::lock_guard<std::mutex> lock(m_signalMutex);
			m_currentSignal = signal;
		}

		m_signalVariable.notify_all();
	}
	bool RestaurantInternal::waitFor(Signal signal, std::chrono::duration<long double> maxWaitTime)
	{
		std::unique_lock<std::mutex> lock(m_signalMutex);
		auto result = m_signalVariable.wait_for(lock, maxWaitTime, [this, signal] { return this->m_currentSignal == signal; });

		return result;
	}
}