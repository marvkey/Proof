#pragma once
#include <memory>
#include <iostream>
#include<chrono>
#include "UUID.h"
#define PF_BIND_FN(fn) std::bind(&fn, this, std::placeholders::_1)
#define PF_BIND_ACTION(fn)  std::bind(&fn, this)
#define PF_BIND_MOTION(fn) std::bind(&fn, this, std::placeholders::_1)

/*
#ifdef PF_PLATFORM_WINDOW64
    #ifdef LINK_DLL
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
*/
#define Proof_API
// Add this when locating a file, but not fore built in functions it is already automatically added
inline const std::string ProofCurrentDirectorySrc = "../Proof/src/";
inline const std::string ProofCurrentDirectory = "../Proof/";
#define BIT(x) (1 << x)
#ifdef _WIN64
#ifdef PF_ENABLE_ASSERT
#define PF_ASSERT(X,...){ if((!X)) {PF_ERROR("Assertion Failed %s",__VA_ARGS__); __debugbreak();  } }
#define PF_CORE_ASSERT(X,...){ if((!X)) {PF_ENGINE_ERROR("Assertion Failed %s",__VA_ARGS__); __debugbreak(); } } 
#else
#define PF_ASSERT(X,...)
#define PF_CORE_ASSERT(X,...)
#endif
#endif


#define GetVariableName(X)#X

namespace Proof {
    template<typename T>
    using Count = std::shared_ptr<T>;

    template<typename T, typename ... Args>
    inline constexpr Count<T> CreateCount(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Special = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    inline constexpr Special<T> CreateSpecial(Args&&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    struct Proof_API Timer {
        std::chrono::time_point<std::chrono::steady_clock>Start, End;
        void StartTimer() {
            Start = std::chrono::high_resolution_clock::now();
        }
        void TimerEnd() {
            End = std::chrono::high_resolution_clock::now();
            Duration = End - Start;
            Milliseconds = Duration.count() * 1000.0f;
        }
        float Milliseconds;
    private:
        std::chrono::duration <float>Duration;
    };
}