#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <math.h>
#include <algorithm>
namespace Proof
{
    struct InterpolationCurve 
    {

    public:
        glm::vec2 Min = { 0.0f, 0.0f };
        glm::vec2 Max = { 1.0f, 1.0f };
        std::vector<glm::vec2> Points = { {0.0f, 0.0f}, {1.0f, 1.0f} };

        void AddPoint(glm::vec2 p) {
            p.x = glm::clamp(p.x, Min.x, Max.x);
            p.y = glm::clamp(p.y, Min.y, Max.y);
            Points.push_back(p);
            std::sort(Points.begin(), Points.end(), [](auto& a, auto& b) { return a.x < b.x; });
        }

        void EditPoint(int index, glm::vec2 p) {
            p.x = std::clamp(p.x, Min.x, Max.x);
            p.y = std::clamp(p.y, Min.y, Max.y);
            Points[index] = p;
            std::sort(Points.begin(), Points.end(), [](auto& a, auto& b) { return a.x < b.x; });
        }

        // Linear interpolation for now
        float Evaluate(float t) const {
            if (Points.empty()) return 0.0f;
            if (t <= Points.front().x) return Points.front().y;
            if (t >= Points.back().x) return Points.back().y;

            for (size_t i = 0; i < Points.size() - 1; ++i) {
                const glm::vec2& p1 = Points[i];
                const glm::vec2& p2 = Points[i + 1];
                if (t >= p1.x && t <= p2.x) {
                    float factor = (t - p1.x) / (p2.x - p1.x);
                    return glm::mix(p1.y, p2.y, factor);
                }
            }
            return 0.0f;
        }
    };
}