#include "Proofprch.h"
#include "BasicCollision.h"
#include "AABB.h"

namespace Proof {
    bool BasicCollision::PointInAABB(const AABB& collison, const glm::vec3& point)
    {
        /*
        To check if a point is inside an Axis-Aligned Bounding Box (AABB), you can use the following formula:

        Let's assume the AABB is defined by two points: `Min` (minimum point) and `Max` (maximum point), and the point you want to check is `P`.

        For a 3D AABB:
        - Check if `P.x` is between `Min.x` and `Max.x`.
        - Check if `P.y` is between `Min.y` and `Max.y`.
        - Check if `P.z` is between `Min.z` and `Max.z`.

        In mathematical terms:
        ```cpp
        bool IsPointInAABB(const glm::vec3& P, const glm::vec3& Min, const glm::vec3& Max) {
            return (P.x >= Min.x && P.x <= Max.x) &&
                   (P.y >= Min.y && P.y <= Max.y) &&
                   (P.z >= Min.z && P.z <= Max.z);
        }
        ```

        For a 2D AABB:
        - Check if `P.x` is between `Min.x` and `Max.x`.
        - Check if `P.y` is between `Min.y` and `Max.y`.

        In mathematical terms:
        ```cpp
        bool IsPointInAABB(const glm::vec2& P, const glm::vec2& Min, const glm::vec2& Max) {
            return (P.x >= Min.x && P.x <= Max.x) &&
                   (P.y >= Min.y && P.y <= Max.y);
        }
        ```

        These formulas perform the necessary checks for each dimension (X, Y, Z in 3D or X, Y in 2D) to determine if the point `P` is inside the AABB defined by `Min` and `Max`. 
        If all conditions are met, the point is inside the AABB, and the function returns `true`; otherwise, it returns `false`.

        */
        return (point.x >= collison.Min.x && point.x <= collison.Max.x) &&
            (point.y >= collison.Min.y && point.y <= collison.Max.y) &&
            (point.z >= collison.Min.z && point.z <= collison.Max.z);
    }
}
