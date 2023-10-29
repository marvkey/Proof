#include "Proofprch.h"
#include "CommandQueue.h"
//https://github.com/TheUnicum/RenderCommandQueue/blob/master/RenderCommandQueue/src/RenderCommandQueue.cpp
namespace Proof 
{
	CommandQueue::CommandQueue(uint32_t size)
		:m_Size(size)
	{
		m_Commands.resize(size);  // Allocate buffer
		m_CommandsPtr = m_Commands.data();
		memset(m_Commands.data(), 0, size);
	}
	CommandQueue::~CommandQueue()
	{
		m_Commands.clear();
		m_CommandsPtr = nullptr;
	}
	void* CommandQueue::Allocate(CommandFn fn, uint32_t size)
	{
		*(CommandFn*)m_CommandsPtr = fn; // storing the pointer to the function
		m_CommandsPtr += sizeof(CommandFn); // advancing by commdnafucntio

		*(uint32_t*)m_CommandsPtr = size; // storing the size of the fucntion
		m_CommandsPtr += sizeof(uint32_t); // advancing by the size of uint32

		void* memory = m_CommandsPtr;
		m_CommandsPtr += size; // advancing by the size of the function

		m_CommandCount++; // increasing the command coint
		return memory;
	}
	void CommandQueue::Execute()
	{
		byte* buffer = m_Commands.data();
		for (uint32_t i = 0; i < m_CommandCount; i++)
		{
			CommandFn function = *(CommandFn*)buffer;
			buffer += sizeof(CommandFn);

			uint32_t size = *(uint32_t*)buffer;// size of function
			buffer += sizeof(uint32_t);
			function(buffer);// fucntio is a pointer to the funciton pointer, the buffer is command data needed to perform the operations
			buffer += size;
		}
		m_CommandsPtr = m_Commands.data();
		m_CommandCount = 0;
	}
}
