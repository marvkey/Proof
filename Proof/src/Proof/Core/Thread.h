#pragma once

#include <string>
#include <thread>

namespace Proof
{

	class Thread
	{
	public:
		Thread() = default;
		Thread(const std::string& name)
			: m_Name(name) {}

		template<typename Fn, typename... Args>
		void Dispatch(Fn&& func, Args&&... args)
		{
			m_Thread = std::thread(func, std::forward<Args>(args)...);
			SetName(m_Name);
		}

		void SetName(const std::string& name) { m_Name = name; }

		void Join() { m_Thread.join(); }
	private:
		std::string m_Name;
		std::thread m_Thread;
	};
}