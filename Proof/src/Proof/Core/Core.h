#pragma once
#include <memory>
#include <iostream>
#ifdef PF_PLATFORM_WINDOW64
    #ifdef PF_CHANGEBUILD_STATIC
        #ifdef  PF_BUILD_DLL
            #define Proof_API __declspec(dllexport)
        #else
            #define Proof_API __declspec(dllimport)
        #endif
        #else
    #define Proof_API
#endif
#else We Only support winodws 64 Bit
#endif

// Add this when locating a file, but not fore built in functions it is already automatically added
constexpr auto FolderOfProofCurrentDirectory = "../Proof/";;
template<typename T>
using Ref = std::shared_ptr<T>;
#ifdef PF_PLATFORM_WINDOW64
    #ifdef PF_ENABLE_ASSERT
        #define PF_ASSERT(X,...){ if((X)) {PF_ERROR("Assertion Failed {} ",__VA_ARGS__); __debugbreak();  } }
        #define PF_CORE_ASSERT(X,...){ if((X)) {PF_ENGINE_ERROR("Assertion Failed {}",__VA_ARGS__); __debugbreak(); } } 
    #else
    #define PF_ASSERT(X,...)
    #define PF_CORE_ASSERT(X,...)
    #endif
#endif

#define GetVariableName(x)(#x)

