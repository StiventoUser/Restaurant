#ifndef DISHINFO_H
#define DISHINFO_H

#include <string>

namespace Restaurant
{
	class DishInfo
	{
	public:
		DishInfo();
		DishInfo(const std::string& dishName);
		DishInfo(const DishInfo& other);
		~DishInfo();

		DishInfo& operator=(const DishInfo& other);

		const std::string& getDishName() const;
		void setDishName(const std::string& dishName);

	private:
		std::string m_dishName;
	};
}

#endif