#pragma once

#define PF_ENABLE_PROFILING 1

#if PF_ENABLE_PROFILING

	#if 1
			#include <tracy/Tracy.hpp>
			#include <cstring>


			#define PF_HIDDEN_PROFILE_PICK(_0, _1, NAME, ...) NAME
			#define PF_HIDDEN_PROFILE_AUTO() ZoneScoped
			#define PF_HIDDEN_PROFILE_NAMED(NAME) ZoneScopedN(NAME)

			#define PF_PROFILE_FRAME() FrameMark
			#define PF_PROFILE_FUNC(...) PF_HIDDEN_PROFILE_PICK(_0 __VA_OPT__(,) __VA_ARGS__, PF_HIDDEN_PROFILE_NAMED, PF_HIDDEN_PROFILE_AUTO)(__VA_ARGS__)
			#define PF_PROFILE_FUNCN(NAME) ZoneScopedN(NAME)
			#define PF_PROFILE_TAG(NAME) 
			//#define PF_PROFILE_THREAD(NAME) tracy::SetThreadName(NAME)
			#define PF_PROFILE_THREAD(NAME) 
			#define PF_PROFILE_SCOPE_DYNAMIC(NAME) ZoneScoped; ZoneName(NAME, std::strlen(NAME))

	#else

		#include <optick.h>

		#define PF_PROFILE_FRAME(...) OPTICK_FRAME(__VA_ARGS__)
		#define PF_PROFILE_FUNC() OPTICK_EVENT()
		#define PF_PROFILE_FUNCN(NAME) OPTICK_EVENT(NAME)
		#define PF_PROFILE_TAG(NAME, ...) OPTICK_TAG(NAME, __VA_ARGS__)
		#define PF_PROFILE_SCOPE_DYNAMIC(NAME) OPTICK_EVENT_DYNAMIC(NAME)
		#define PF_PROFILE_THREAD(...) OPTICK_THREAD(__VA_ARGS__)

	#endif

#else

	#define PF_PROFILE_FRAME(...)
	#define PF_PROFILE_FUNC()
	#define PF_PROFILE_FUNCN(...)
	#define PF_PROFILE_TAG(...)
	#define PF_PROFILE_SCOPE_DYNAMIC(...)
	#define PF_PROFILE_THREAD(...)

#endif