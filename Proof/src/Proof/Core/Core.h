#pragma once
#include <memory>
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
