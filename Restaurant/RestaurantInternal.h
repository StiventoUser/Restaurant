#ifndef RESTAURANTINTERNAL_H
#define RESTAURANTINTERNAL_H

#include <condition_variable>
#include <mutex>
#include <string>
#include <cstdint>
#include <functional>
#include <chrono>

#include "DishInfo.h"

namespace Restaurant
{
	class RestaurantInternal
	{
	public:
		using CallbackFunc = std::function<void(void)>;
	public:
		enum class Signal : int8_t
		{
			DishRequested, DishFinished, DishDelivered
		};

	public:
		RestaurantInternal();
		~RestaurantInternal();

		template<typename T>
		void setSignalHandler(Signal signal, T func)
		{
			switch (signal)
			{
			case Signal::DishRequested:
				//TODO CHECK IS EXISTS
				m_newRequestCallback = [func] { func(); };
				break;
			case Signal::DishFinished:
				m_dishFinishedCallback = [func] { func(); };
				break;
			case Signal::DishDelivered:
				m_dishDeliveredCallback = [func] { func(); };
				break;
			default:
				break;
				//TODO ERROR
			}
		}

		const std::string& getRequestedDishName() const;
		void setRequstedDishName(const std::string dishName);

		const DishInfo& getDishInfo() const;
		void setDishInfo(const DishInfo& dishInfo);

		void closeRestaurant();
		bool isRestaurantClosed() const;

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
		
		CallbackFunc m_newRequestCallback;
		CallbackFunc m_dishFinishedCallback;
		CallbackFunc m_dishDeliveredCallback;
	};
}

#endif