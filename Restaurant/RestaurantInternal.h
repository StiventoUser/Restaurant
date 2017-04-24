#ifndef RESTAURANTINTERNAL_H
#define RESTAURANTINTERNAL_H

#include <condition_variable>
#include <mutex>
#include <string>
#include <cstdint>
#include <functional>
#include <chrono>

#include "DishInfo.h"

using namespace std::chrono_literals;

namespace Restaurant
{
	class RestaurantInternal
	{
	public:
		enum class Signal : int8_t
		{
			DishRequested, DishFinished, DishDelivered
		};

		static const std::chrono::duration<long double> SignalWaitTime;
		static const std::chrono::duration<long double> DataWaitTime;
		static const std::chrono::duration<long double> ThreadShutdownWaitTime;

	public:
		RestaurantInternal();
		~RestaurantInternal();

		const std::string& getRequestedDishName() const;
		void setRequstedDishName(const std::string dishName);

		const DishInfo& getDishInfo() const;
		void setDishInfo(const DishInfo& dishInfo);

		bool isRestaurantClosed() const;
		void closeRestaurant();

		std::unique_lock<std::timed_mutex> lockData(std::chrono::duration<long double> maxWaitTime);

		void notify(Signal signal);
		bool waitFor(Signal signal, std::chrono::duration<long double> maxWaitTime);

	private:
		std::string m_requestedDishName;
		DishInfo m_finishedDishInfo;

		bool m_isRestaurantClosed = false;

		std::timed_mutex m_dataMutex;

		std::condition_variable m_signalVariable;
		std::mutex m_signalMutex;

		Signal m_currentSignal;
	};
}

#endif