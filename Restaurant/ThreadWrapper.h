#ifndef THREADWRAPPER_H
#define THREADWRAPPER_H

#include <thread>
#include <functional>
#include <memory>
#include <utility>

namespace Restaurant
{
	template<typename ClassT>
	class ThreadWrapper
	{
	public:
		ThreadWrapper();
		~ThreadWrapper();

		std::shared_ptr<ClassT> getObject();

		template<typename ...ArgsT>
		void execute(void(ClassT::* member)(ArgsT...), ArgsT&&... args);

	private: 
		std::shared_ptr<ClassT> m_ptr;
		std::unique_ptr<std::thread> m_thread;
	};

	template<typename ClassT>
	ThreadWrapper<ClassT>::ThreadWrapper() : m_ptr(std::make_shared<ClassT>()) //ignore exception
	{
		Logger() << "Thread constructed.";
	}

	template<typename ClassT>
	ThreadWrapper<ClassT>::~ThreadWrapper()
	{
		Logger() << "Thread destroyed.";
		m_thread->join();
	}
	template<typename ClassT>
	std::shared_ptr<ClassT> ThreadWrapper<ClassT>::getObject()
	{
		return m_ptr;
	}

	template<typename ClassT>
	template<typename ...ArgsT>
	void ThreadWrapper<ClassT>::execute(void(ClassT::* member)(ArgsT...), ArgsT&&... args)
	{
		m_thread = std::make_unique<std::thread>(std::thread(std::bind(member, m_ptr.get(), std::forward<ArgsT>(args)...)));
	}
}

#endif