#pragma once
#include <algorithm>
#include <limits>
#include <glm/glm.hpp>
namespace Proof
{
    //template <typename T, T MinVal = std::numeric_limits<T>::lowest(), T MaxVal = std::numeric_limits<T>::max()>
    template <typename T, T MinVal = 0, T MaxVal = std::numeric_limits<T>::max()>
    struct ClampedValue 
    {
        static_assert(MinVal <= MaxVal, "MinVal must be less than or equal to MaxVal");


        ClampedValue(T val) {
            m_Value = glm::clamp(val, MinVal, MaxVal);
        }

        ClampedValue& operator=(T newVal) {
            m_Value = glm::clamp(newVal, MinVal, MaxVal);
            return *this;
        }

        operator T() const { return m_Value; }

        void Set(T newVal) {
            m_Value = glm::clamp(newVal, MinVal, MaxVal);
        }

        ClampedValue GetValue()
        {
            return m_Value;
        }

        static constexpr T Min() { return MinVal; }
        static constexpr T Max() { return MaxVal; }
    private:
        T m_Value;

    };

    template <typename T>
    struct RuntimeClampedValue {
        T value;
        T minValue;
        T maxValue;

        RuntimeClampedValue(T val, T minVal, T maxVal)
            : minValue(minVal), maxValue(maxVal)
        {
            value = std::clamp(val, minValue, maxValue);
        }

        RuntimeClampedValue& operator=(T newVal) {
            value = std::clamp(newVal, minValue, maxValue);
            return *this;
        }

        operator T() const { return value; }

        void set(T newVal) {
            value = std::clamp(newVal, minValue, maxValue);
        }

        void setMinMax(T minVal, T maxVal) {
            minValue = minVal;
            maxValue = maxVal;
            value = std::clamp(value, minValue, maxValue);
        }

        T min() const { return minValue; }
        T max() const { return maxValue; }
    };
}