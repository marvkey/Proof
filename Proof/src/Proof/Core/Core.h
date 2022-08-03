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
#include <optick.h>
#include "Proof/Resources/EnumReflection.h"
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

#define PROOF_EXPAND_MACRO(x) x
#define PROOF_STRINGIFY_MACRO(x) #x
#define Proof_API
// Add this when locating a file, but not fore built in functions it is already automatically added
inline const std::string ProofCurrentDirectorySrc = "../Proof/src/";
inline const std::string ProofCurrentDirectory = "../Proof/";
#define BIT(x) (1 << x)
#ifdef PF_ENABLE_ASSERT

    #define PF_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { PF##type##ERROR(msg, __VA_ARGS__);  __debugbreak(); } }
    #define PF_INTERNAL_ASSERT_WITH_MSG(type, check, ...) PF_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
    #define PF_INTERNAL_ASSERT_NO_MSG(type, check) PF_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", PROOF_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

    #define PF_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
    #define PF_INTERNAL_ASSERT_GET_MACRO(...) PROOF_EXPAND_MACRO( PF_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, PF_INTERNAL_ASSERT_WITH_MSG, PF_INTERNAL_ASSERT_NO_MSG) )
    
    #define PF_ASSERT(X,...){ if((!X)) {PF_ERROR("Assertion Failed {}",__VA_ARGS__); __debugbreak();  } }
    #define PF_CORE_ASSERT(X,...){ if((!X)) {PF_ENGINE_ERROR("Assertion Failed {}",__VA_ARGS__); __debugbreak(); } } 
    #define PF_CORE_ASSERT(X){ if((!X)) {PF_ENGINE_ERROR("Assertion Failed"); __debugbreak(); } } 
#else
    #define PF_ASSERT(X,...)
    #define PF_CORE_ASSERT(X,...)
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
    enum class TimerTypes {
        CPUTimer = 0,
        Renderer
    };
    struct FrameTimersControll {
    public:

        static void Add(const std::string& name, float time,float maxTime =0.0f, TimerTypes type = TimerTypes::CPUTimer) {
            for (FrameTimeManage& timerManage : s_FrameTimers) {
                if (timerManage.FunctionName == name) {
                    timerManage.TimerManage.Time += time;
                    timerManage.Times.emplace_back(time);
                    return;
                }   
            }
            s_FrameTimers.push_back({ name, TimeManage{ time,maxTime,type } });
        }
        struct TimeManage {
            float Time = 0;
            float MaxTime = 0;
            TimerTypes TimerType;
        };
        struct FrameTimeManage {
            std::string FunctionName;
            TimeManage TimerManage;
            std::vector<float> Times;
            FrameTimeManage(const std::string& functionName, TimeManage& timeManage) :
                FunctionName(functionName),TimerManage(timeManage)
            {
                Times.emplace_back(timeManage.Time);
            }
        };
    private:
       static void Reset() {
            for (FrameTimeManage& frameTime : s_FrameTimers) {
                frameTime.TimerManage.Time = 0;
                frameTime.Times.clear();
            }
        }
        // name and time
        static std::vector<FrameTimeManage> s_FrameTimers;
        friend class Application;
        friend class PerformancePanel;
    };

  
    struct RangeTimerMacro {
    public:
        RangeTimerMacro(const std::string& name, float maxTime = 0, TimerTypes types = TimerTypes::CPUTimer)
            : m_Name(name), m_MaxTime(maxTime),m_Type(types) {  
        }
        ~RangeTimerMacro();
    private:
        std::string m_Name;
        float m_MaxTime = 0.0f;
        Timer m_Timer;
        TimerTypes  m_Type;
    };
    #define PF_SCOPE_TIME(name) RangeTimerMacro ONLYUSEDONCEPERSCOPE(name); 
    #define PF_SCOPE_TIME_THRESHHOLD(name,X) RangeTimerMacro ONLYUSEDONCEPERSCOPETHRESHOLD(name,X);
    #define PF_SCOPE_TIME_THRESHHOLD_TYPE(name,X, Y) RangeTimerMacro ONLYUSEDONCEPERSCOPETHRESHOLDTYPE(name, X,Y)

}
