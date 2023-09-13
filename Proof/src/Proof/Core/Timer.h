#pragma once
#include<chrono>
#include <unordered_map>
namespace Proof
{
    struct Timer 
    {
        Timer() {
            Reset();
        }
        // returns the time passed in nanoseconds
        float Elapsed() {
            // using nano seconds to get a more accurate answer when minusing to get seconds
            return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
        }
        float ElapsedMillis() {
            return Elapsed() * 1000.0f;
        }

        float ElapsedSeconds() {
            return ElapsedMillis() * 1000.0f;
        }
        void Reset() {
            m_Start = std::chrono::high_resolution_clock::now();
        }
    private:
        std::chrono::time_point<std::chrono::steady_clock>m_Start;
    };
    struct ScopeTimer {
    public:
        ScopeTimer(const std::string& name)
            : m_Name(name) {
        }
        ~ScopeTimer();
    private:
        std::string m_Name;
        Timer m_Timer;
    };

    struct ScopePerformanceTimer {
    public:
        ScopePerformanceTimer(const std::string& name, float threshold = 0)
            : m_Name(name), m_Threshold(threshold)
        {
            m_Timer = Timer();
        }
        ~ScopePerformanceTimer();

        static void ClearTimers();
        static const std::unordered_map<std::string, std::pair<float, uint32_t>>& GetTimers() { return s_Timers; }
    private:
        inline static std::mutex m_PerFrameDataMutex;

        std::string m_Name;
        float m_Threshold = 0.0f;
        Timer m_Timer;
        // timer name, timer time, times funnction called in frame
        static std::unordered_map<std::string, std::pair<float,uint32_t>> s_Timers;
    };


    #define PF_SCOPE_PERF(...) PF_SCOPE_PERF_INTERNAL(__VA_ARGS__, __FUNCTION__)
    #define PF_SCOPE_PERF_INTERNAL(name, fallback) ScopePerformanceTimer timer_##__LINE__(name ? name : fallback)


    #define PF_SCOPE_PERF_TIME(...) PF_SCOPE_PERF_INTERNAL(__VA_ARGS__)
    #define PF_SCOPE_PERF_INTERNAL(name, threshold) ScopePerformanceTimer timer_##__LINE__(name ? name : __FUNCTION__, threshold)


}