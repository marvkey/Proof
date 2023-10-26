#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <cstring>
namespace Proof
{
	typedef void(*CommandFn)(void*);
	class CommandQueue 
	{
	public:
		//10mb allocated begin size
		// resizabe
		CommandQueue(uint32_t size = 10 * 1024 * 1024); //10 mb data
		~CommandQueue();

		void* Allocate(CommandFn fn, uint32_t size);
		template<class FuncT>
		static void Submit(FuncT&& func)
		{
			//https://www.youtube.com/watch?v=WeqxJeme_88
			//(1:14:02)
			auto renderCommandQueue = [](void* ptr)
			{
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();

				pFunc->~FuncT();
			};
			auto storageBuffer = Allocate(renderCommandQueue, sizeof(func));
			new (storageBuffer)FuncT(std::forward<FuncT>(func));
		};
		void Execute();
	private:
		std::vector<uint8_t> m_Commands;
		uint8_t* m_CommandsPtr;
		uint32_t m_Size = 0;
		uint32_t m_CommandCount = 0;
	};

}
