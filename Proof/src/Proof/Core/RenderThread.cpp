#include "Proofprch.h"
#include "RenderThread.h"

#include <Windows.h>

#include "Proof/Renderer/Renderer.h"

/*
`InitializeCriticalSection` is a Windows API function used for initializing a critical section object. 
A critical section is a synchronization primitive used for managing access to shared resources in a multithreaded program. Critical sections are often used to protect sections of code from being executed by more than one thread simultaneously. They provide a way to achieve mutual exclusion, ensuring that only one thread can execute the protected code at any given time.

Here's a brief overview of how `InitializeCriticalSection` works:

1. **Initialization**: When you call `InitializeCriticalSection`, it creates a critical section object and initializes its internal data structures. 
This function prepares the critical section for use by other threads.

2. **Thread Synchronization**: Once initialized, multiple threads can use the critical section to synchronize their access to a shared resource. 
Threads can enter the critical section, execute a protected code block, and then exit it.

3. **Blocking**: If a thread attempts to enter a critical section while another thread is already inside it (i.e., the critical section is currently held), the second thread will be blocked (suspended) until the first thread exits the critical section.

4. **Exiting the Critical Section**: When a thread completes its work inside the critical section, it calls `DeleteCriticalSection` to release the critical section. This allows other threads to enter the section.

Example code in C/C++:

```cpp
CRITICAL_SECTION cs; // Declare a critical section object

// Initialize the critical section
InitializeCriticalSection(&cs);

// ...

// Enter the critical section
EnterCriticalSection(&cs);

// ... (protected code block)

// Exit the critical section
LeaveCriticalSection(&cs);

// ...

// Delete the critical section when no longer needed
DeleteCriticalSection(&cs);
```

Critical sections are commonly used for protecting shared data structures, resources, 
or code that should only be accessed by one thread at a time to prevent data corruption and race conditions in multithreaded applications. 
It's important to pair the `InitializeCriticalSection` function with `DeleteCriticalSection` to properly release resources when they are no longer needed.
*/

/**
	`InitializeCriticalSection` and `std::thread::join` serve different purposes and have different use cases. 
	Additionally, `std::mutex` is related to the concept of synchronization, just like `InitializeCriticalSection`. Let's break down the differences and use cases for each of these synchronization mechanisms:

1. **`InitializeCriticalSection` vs. `std::mutex`**:
   - `InitializeCriticalSection` is a Windows-specific mechanism for creating and managing critical sections for synchronization. 
   It's primarily used for synchronization within Windows applications.
   - `std::mutex` is part of the C++ Standard Library and provides a cross-platform way to create and manage mutexes for synchronization in C++ programs.
   - If you're writing a Windows-specific application and using Windows API functions, you might use `InitializeCriticalSection`. 
   If you're writing a C++ program that needs synchronization, especially if it's cross-platform, you would typically prefer `std::mutex`.

2. **`std::thread::join` vs. `std::mutex`**:
   - `std::thread::join` is a method used to block the calling thread until a specified thread has finished executing. 
   It's not about creating synchronization primitives but rather waiting for a specific thread to complete its execution.
   - `std::mutex` is a synchronization primitive that is used to protect shared resources or code sections from concurrent access by multiple threads. 
   It allows threads to coordinate access to shared data structures by locking and unlocking the mutex.

In summary:
- `InitializeCriticalSection` and `std::mutex` both deal with synchronization, but the former is Windows-specific, while the latter is part of the C++ Standard Library and cross-platform.
- `std::thread::join` is about thread management and waiting for a specific thread to finish.

If you want to synchronize access to shared resources among multiple threads in a C++ program, 
you would typically use `std::mutex` or other synchronization primitives provided by the C++ Standard Library. 
`std::thread::join` is used when you want to wait for a thread to complete its execution before proceeding in the calling thread.
*/
namespace Proof
{

	struct RenderThreadData
	{
		CRITICAL_SECTION m_CriticalSection;
		CONDITION_VARIABLE m_ConditionVariable;

		RenderThread::State m_State = RenderThread::State::Idle;
	};

	RenderThread::RenderThread(ThreadingPolicy coreThreadingPolicy)
		: m_RenderThread("Render Thread"), m_ThreadingPolicy(coreThreadingPolicy)
	{
		m_Data = new RenderThreadData();

		if (m_ThreadingPolicy == ThreadingPolicy::MultiThreaded)
		{
			InitializeCriticalSection(&m_Data->m_CriticalSection);
			InitializeConditionVariable(&m_Data->m_ConditionVariable);
		}
	}

	RenderThread::~RenderThread()
	{

		if (m_ThreadingPolicy == ThreadingPolicy::MultiThreaded)
			DeleteCriticalSection(&m_Data->m_CriticalSection);
	}

	void RenderThread::Run()
	{
		m_IsRunning = true;
		if (m_ThreadingPolicy == ThreadingPolicy::MultiThreaded)
			m_RenderThread.Dispatch(Renderer::RenderThreadFunc, this);
	}

	void RenderThread::Terminate()
	{
		m_IsRunning = false;
		Pump();
		if (m_ThreadingPolicy == ThreadingPolicy::MultiThreaded)
			m_RenderThread.Join();
	}

	void RenderThread::Wait(State waitForState)
	{
		if (m_ThreadingPolicy == ThreadingPolicy::SingleThreaded)
			return;

		EnterCriticalSection(&m_Data->m_CriticalSection);
		while (m_Data->m_State != waitForState)
		{
			// This releases the CS so that another thread can wake it
			SleepConditionVariableCS(&m_Data->m_ConditionVariable, &m_Data->m_CriticalSection, INFINITE);
		}
		LeaveCriticalSection(&m_Data->m_CriticalSection);
	}

	void RenderThread::WaitAndSet(State waitForState, State setToState)
	{
		if (m_ThreadingPolicy == ThreadingPolicy::SingleThreaded)
			return;

		EnterCriticalSection(&m_Data->m_CriticalSection);
		while (m_Data->m_State != waitForState)
		{
			/*
				SleepConditionVariableCS is a Windows API function used for thread synchronization. 
				It allows one thread to wait until a specific condition is met, signaled by another thread. 
				This is achieved using a critical section and a condition variable to protect shared data and control access to it. 
				The function releases the critical section, waits for the condition to be signaled, and reacquires the critical section before continuing. 
				It's a way to efficiently coordinate threads based on specific conditions
			*/
			SleepConditionVariableCS(&m_Data->m_ConditionVariable, &m_Data->m_CriticalSection, INFINITE);
		}
		m_Data->m_State = setToState;
		WakeAllConditionVariable(&m_Data->m_ConditionVariable);
		LeaveCriticalSection(&m_Data->m_CriticalSection);
	}

	void RenderThread::Set(State setToState)
	{
		if (m_ThreadingPolicy == ThreadingPolicy::SingleThreaded)
			return;

		EnterCriticalSection(&m_Data->m_CriticalSection);
		m_Data->m_State = setToState;
		WakeAllConditionVariable(&m_Data->m_ConditionVariable);
		LeaveCriticalSection(&m_Data->m_CriticalSection);
	}

	void RenderThread::NextFrame()
	{
		m_AppThreadFrame++;
		Renderer::SwapQueues();
	}

	void RenderThread::BlockUntilRenderComplete()
	{
		if (m_ThreadingPolicy == ThreadingPolicy::SingleThreaded)
			return;

		Wait(State::Idle);
	}

	void RenderThread::Kick()
	{
		if (m_ThreadingPolicy == ThreadingPolicy::MultiThreaded)
		{
			Set(State::Kick);
		}
		else
		{
			Renderer::WaitAndRender(this);
		}
	}

	void RenderThread::Pump()
	{
		NextFrame();
		Kick();
		BlockUntilRenderComplete();
	}
}