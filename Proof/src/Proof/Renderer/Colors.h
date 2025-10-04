#pragma once
#include <glm/glm.hpp>
namespace Proof
{
	class Colors
	{
	public:

        inline static const glm::vec4 Red = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        inline static const glm::vec4 Green = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        inline static const glm::vec4 Blue = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        inline static const glm::vec4 Yellow = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
        inline static const glm::vec4 Cyan = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
        inline static const glm::vec4 Magenta = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
        inline static const glm::vec4 Orange = glm::vec4(1.0f, 0.65f, 0.0f, 1.0f);
        inline static const glm::vec4 Purple = glm::vec4(0.5f, 0.0f, 0.5f, 1.0f);
        inline static const glm::vec4 Pink = glm::vec4(1.0f, 0.75f, 0.8f, 1.0f);
        inline static const glm::vec4 Brown = glm::vec4(0.65f, 0.16f, 0.16f, 1.0f);
        inline static const glm::vec4 Lime = glm::vec4(0.75f, 1.0f, 0.0f, 1.0f);
        inline static const glm::vec4 Olive = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
        inline static const glm::vec4 Navy = glm::vec4(0.0f, 0.0f, 0.5f, 1.0f);
        inline static const glm::vec4 Teal = glm::vec4(0.0f, 0.5f, 0.5f, 1.0f);
        inline static const glm::vec4 Gold = glm::vec4(1.0f, 0.84f, 0.0f, 1.0f);
        inline static const glm::vec4 Silver = glm::vec4(0.75f, 0.75f, 0.75f, 1.0f);
        inline static const glm::vec4 Gray = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        inline static const glm::vec4 LightGray = glm::vec4(0.75f, 0.75f, 0.75f, 1.0f);
        inline static const glm::vec4 DarkGray = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f);
        inline static const glm::vec4 White = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        inline static const glm::vec4 Black = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        inline static const glm::vec4 Transparent = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	};

    enum class ColorGradientMode
    {
        Blend,
        Fixed
    };
    class ColorGradient
    {
    public:
        ColorGradientMode Mode = ColorGradientMode::Blend;
        struct ColorKey
        {
            glm::vec4 Color = { 1.0f,1.0f,1.0f,1.0f }; // Supports RGB + Alpha
            float Time;      // 0.0 to 1.0
        };

        void Updatekey(int index, ColorKey key)
        {
            if (m_ColorKeys.size() > index)
            {
                m_ColorKeys[index] = key;
                std::sort(m_ColorKeys.begin(), m_ColorKeys.end(),
                    [](const ColorKey& a, const ColorKey& b) { return a.Time < b.Time; });
            }
        }

        ColorKey GetKey(int index)
        {
            if(m_ColorKeys.size() > index)
				return m_ColorKeys[index];
			return ColorKey{};
        }
        void AddColorKey(const glm::vec3& color, float time)
        {
            AddColorKey(glm::vec4(color, 1.0f), time);
        }

        void AddColorKey(const glm::vec4& color, float time)
        {
            time = glm::clamp(time, 0.0f, 1.0f);
            m_ColorKeys.push_back({ color, time });
            std::sort(m_ColorKeys.begin(), m_ColorKeys.end(),
                [](const ColorKey& a, const ColorKey& b) { return a.Time < b.Time; });
        }

        glm::vec4 Evaluate(float t) const
        {
            if (m_ColorKeys.empty()) return glm::vec4(1.0f); // white default

            t = glm::clamp(t, 0.0f, 1.0f);

            // If t is before the first key or after the last key
            if (t <= m_ColorKeys.front().Time) return m_ColorKeys.front().Color;
            if (t >= m_ColorKeys.back().Time)  return m_ColorKeys.back().Color;

            switch (Mode)
            {
                case Proof::ColorGradientMode::Blend:
                {
                    // Find surrounding keys
                    for (size_t i = 0; i + 1 < m_ColorKeys.size(); ++i)
                    {
                        const ColorKey& a = m_ColorKeys[i];
                        const ColorKey& b = m_ColorKeys[i + 1];

                        if (t >= a.Time && t <= b.Time)
                        {
                            float localT = (t - a.Time) / (b.Time - a.Time);
                            return glm::mix(a.Color, b.Color, localT);
                        }
                    }
                }
                    break;
                case Proof::ColorGradientMode::Fixed:
                {
                    glm::vec4 result = m_ColorKeys[0].Color; // default to first key
                    for (size_t i = 0; i < m_ColorKeys.size(); ++i)
                    {
                        // loop till we find a key time that is not greater 
                        if (t >= m_ColorKeys[i].Time)
                            result = m_ColorKeys[i].Color;
                        else
                            break;// keys are sorted, so we can stop early
                    }

                    return result;
                }
                    break;
            }
            

            return m_ColorKeys.back().Color; // fallback
        }

        const std::vector<ColorKey>& GetKeys()const { return m_ColorKeys; };
        const std::vector<ColorKey>& GetKeys() { return m_ColorKeys; };

    private:
        std::vector<ColorKey> m_ColorKeys;
    };


    struct UBGradientKeyHold
    {
        glm::vec4 Color;  // rgba
        float Time;       // 0..1
        float _pad[3];    // pad to 16
    };

    #define MAX_GRADIENT_KEYS 16
    struct alignas(16) UBGradientKey
    {
        UBGradientKeyHold Keys[MAX_GRADIENT_KEYS];
        int KeyCount;
        glm::ivec3 _pad; // pad to 16
    };
}