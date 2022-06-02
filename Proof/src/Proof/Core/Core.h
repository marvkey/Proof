#pragma once
#include <memory>
#include <iostream>
#include<chrono>
#include "UUID.h"
#include <vector>
#define PF_BIND_FN(fn) std::bind(&fn, this, std::placeholders::_1)
#define PF_BIND_ACTION(fn)  std::bind(&fn, this)
#define PF_BIND_MOTION(fn) std::bind(&fn, this, std::placeholders::_1)
#include <unordered_map>
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
#define PF_ASSERT(X,...){ if((!X)) {PF_ERROR("Assertion Failed {}",__VA_ARGS__); __debugbreak();  } }
#define PF_CORE_ASSERT(X,...){ if((!X)) {PF_ENGINE_ERROR("Assertion Failed {}",__VA_ARGS__); __debugbreak(); } } 
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
        Timer() {
            Reset();
        }
        float TimePassed() {
            // using nano seconds to get a more accurate answer when minusing to get seconds
            return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
        }
        float TimePassedMillis() {
            return TimePassed() * 1000.0f;
        }
        void Reset() {
            m_Start = std::chrono::high_resolution_clock::now();
        }
    private:
        std::chrono::time_point<std::chrono::steady_clock>m_Start, End;
    };

    struct Proof_API RangeTimer {
    public:
        RangeTimer(const std::string& name)
            : m_Name(name) {
        }
        ~RangeTimer();
    private:
        std::string m_Name;
        Timer m_Timer;

    };
    struct FrameTimersControll {
    public:

        static void Add(const std::string& name, float time,float maxTime =0.0f) {
            auto val = s_FrameTimers.find(name);
            if (val != s_FrameTimers.end()) {
                val->second.Time += time;
                return;
            }
            s_FrameTimers.insert({ name, { time,maxTime } });
        }
        struct TimeManage {
            float Time = 0;
            float MaxTime = 0;
        };
    private:
      
        // name and time
        static std::unordered_map<std::string, TimeManage> s_FrameTimers;
        friend class Application;
        friend class PerformancePanel;
    };

    struct RangeTimerMacro {
    public:
        RangeTimerMacro(const std::string& name, float MaxTime = 0)
            : m_Name(name) {
        }
        ~RangeTimerMacro() {
            float time = m_Timer.TimePassedMillis();
            FrameTimersControll::Add(m_Name, time, m_MaxTime);
        }
    private:
        std::string m_Name;
        float m_MaxTime = 0.0f;
        Timer m_Timer;
    };
}
