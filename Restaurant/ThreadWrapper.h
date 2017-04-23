#ifndef THREADWRAPPER_H
#define THREADWRAPPER_H

#include <future>
#include <functional>
#include <memory>
#include <utility>
#include <chrono>

namespace Restaurant
{
	template<typename ClassT>
	class ThreadWrapper
	{
	public:
		ThreadWrapper();
		~ThreadWrapper();

		std::shared_ptr<ClassT> getObject();

		bool isFinished() const;

		template<typename ...ArgsT>
		void execute(void(ClassT::* member)(ArgsT...), ArgsT&&... args);

	private: 
		std::shared_ptr<ClassT> m_ptr;
		std::future<void> m_future;
	};

	template<typename ClassT>
	ThreadWrapper<ClassT>::ThreadWrapper() : m_ptr(std::make_shared<ClassT>()) //ignore possible exceptions
	{
		Logger() << "Thread constructed.";
	}

	template<typename ClassT>
	ThreadWrapper<ClassT>::~ThreadWrapper()
	{
		m_future.get();
		Logger() << "Thread destroyed.";
	}
	template<typename ClassT>
	std::shared_ptr<ClassT> ThreadWrapper<ClassT>::getObject()
	{
		return m_ptr;
	}

	template<typename ClassT>
	inline bool ThreadWrapper<ClassT>::isFinished() const
	{
		using namespace std::chrono_literals;

		auto result = m_future.wait_for(0ms);

		return result == std::future_status::ready;
	}

	template<typename ClassT>
	template<typename ...ArgsT>
	void ThreadWrapper<ClassT>::execute(void(ClassT::* member)(ArgsT...), ArgsT&&... args)
	{
		m_future = std::async(std::launch::async, std::bind(member, m_ptr, std::forward<ArgsT>(args)...));
	}
}

#endif