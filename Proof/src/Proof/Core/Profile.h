#pragma once

#define PF_ENABLE_PROFILING 1
#if PF_ENABLE_PROFILING

	#if 0
		#define TRACY_ENABLE
			#include <tracy/Tracy.hpp>

		// Define a macro to check if x is empty
		#define ISEMPTY(x) (!x || x[0] == '\0')

		// Define PF_PROFILE_FUNC_IMPL macro
		#define PF_PROFILE_FUNC_IMPL(x) \
			ISEMPTY(x) ? ZoneScopedN(__FUNCTION__) : ZoneScopedN(x)
		#define PF_PROFILE_FRAME(x)             FrameMark()
		#define PF_PROFILE_FUNC(...)			PF_PROFILE_FUNC_IMPL(__VA_ARGS__)
		#define PF_PROFILE_TAG(y,x)             ZoneName(x, strlen(x))
		#define PF_PROFILE_THREAD(...)          ZoneScoped

	#else

		#include <optick.h>
		#define PF_PROFILE_FRAME(...)           OPTICK_FRAME(__VA_ARGS__)
		#define PF_PROFILE_FUNC(...)            OPTICK_EVENT(__VA_ARGS__)
		#define PF_PROFILE_TAG(NAME,...)        OPTICK_TAG(NAME,__VA_ARGS__)
		#define PF_PROFILE_SCOPE_DYNAMIC(NAME)  OPTICK_EVENT_DYNAMIC(NAME)
		#define PF_PROFILE_THREAD(...)          OPTICK_THREAD(__VA_ARGS__)
	#endif
#else
#define PF_PROFILE_FRAME(...)    
#define PF_PROFILE_FUNC(...) 
#define PF_PROFILE_TAG(NAME,...)
#define PF_PROFILE_SCOPE_DYNAMIC(NAME)
#define PF_PROFILE_THREAD(...) 
#endif