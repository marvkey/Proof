#pragma once
#include <filesystem>"
#include "Log.h"
#ifdef PF_ENABLE_ASSERT
#define PF_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { PF##type##ERROR(msg, __VA_ARGS__);  __debugbreak(); } }
#define PF_INTERNAL_ASSERT_WITH_MSG(type, check, ...) PF_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define PF_INTERNAL_ASSERT_NO_MSG(type, check) PF_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", PROOF_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define PF_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define PF_INTERNAL_ASSERT_GET_MACRO(...) PROOF_EXPAND_MACRO( PF_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, PF_INTERNAL_ASSERT_WITH_MSG, PF_INTERNAL_ASSERT_NO_MSG) )

#define PF_ASSERT(X,...){ if((!X)) {PF_ERROR("Assertion Failed {}",__VA_ARGS__); __debugbreak();  } }
#define PF_CORE_ASSERT(...) PROOF_EXPAND_MACRO( PF_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_ENGINE_, __VA_ARGS__) )
#else
#define PF_ASSERT(X,...)
#define PF_CORE_ASSERT(X,...)
#endif