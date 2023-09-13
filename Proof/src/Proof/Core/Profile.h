#pragma once
#include <optick.h>

#define PF_ENABLE_PROFILING 1
#if PF_ENABLE_PROFILING
#define PF_PROFILE_FRAME(...)           OPTICK_FRAME(__VA_ARGS__)
#define PF_PROFILE_FUNC(...)            OPTICK_EVENT(__VA_ARGS__)
#define PF_PROFILE_TAG(NAME,...)        OPTICK_TAG(NAME,__VA_ARGS__)
#define PF_PROFILE_SCOPE_DYNAMIC(NAME)  OPTICK_EVENT_DYNAMIC(NAME)
#define PF_PROFILE_THREAD(...)          OPTICK_THREAD(__VA_ARGS__)
#else
#define PF_PROFILE_FRAME(...)    
#define PF_PROFILE_FUNC(...) 
#define PF_PROFILE_TAG(NAME,...)
#define PF_PROFILE_SCOPE_DYNAMIC(NAME)
#define PF_PROFILE_THREAD(...) 
#endif