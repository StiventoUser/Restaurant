#include "DishInfo.h"

namespace Restaurant
{
	DishInfo::DishInfo()
	{
	}

	DishInfo::DishInfo(const std::string& dishName) : m_dishName(dishName)
	{
	}

	DishInfo::DishInfo(const DishInfo& other) : m_dishName(other.m_dishName)
	{
	}


	DishInfo::~DishInfo()
	{
	}

	DishInfo& DishInfo::operator=(const DishInfo& other)
	{
		m_dishName = other.m_dishName;

		return *this;
	}

	void DishInfo::setDishName(const std::string& dishName)
	{
		m_dishName = dishName;
	}

	const std::string& DishInfo::getDishName() const
	{
		return m_dishName;
	}
}