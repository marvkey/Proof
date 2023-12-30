#include "Proofprch.h"
#include "BasicCollision.h"
#include "AABB.h"
#include "Ray.h"

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
    bool BasicCollision::AABBInAABB(const AABB& aabb0, const AABB& aabb1)
    {
        /*
        *
        * To check if one Axis-Aligned Bounding Box (AABB) is entirely inside another AABB, you can use the following formula:

        Let's assume you have two AABBs defined by their minimum and maximum points: `AABB1` with `Min1` and `Max1`, and `AABB2` with `Min2` and `Max2`.

        In 3D space, you can use the following conditions:
        - Check if `Min2.x` is greater than or equal to `Min1.x` and if `Max2.x` is less than or equal to `Max1.x`.
        - Check if `Min2.y` is greater than or equal to `Min1.y` and if `Max2.y` is less than or equal to `Max1.y`.
        - Check if `Min2.z` is greater than or equal to `Min1.z` and if `Max2.z` is less than or equal to `Max1.z`.

        In mathematical terms:
        ```cpp
        bool IsAABBInsideAABB(const glm::vec3& Min1, const glm::vec3& Max1, const glm::vec3& Min2, const glm::vec3& Max2) {
            return (Min2.x >= Min1.x && Max2.x <= Max1.x) &&
                   (Min2.y >= Min1.y && Max2.y <= Max1.y) &&
                   (Min2.z >= Min1.z && Max2.z <= Max1.z);
        }
        ```

        In 2D space, the conditions are similar, but you only need to consider the X and Y dimensions:
        - Check if `Min2.x` is greater than or equal to `Min1.x` and if `Max2.x` is less than or equal to `Max1.x`.
        - Check if `Min2.y` is greater than or equal to `Min1.y` and if `Max2.y` is less than or equal to `Max1.y`.

        In mathematical terms:
        ```cpp
        bool IsAABBInsideAABB(const glm::vec2& Min1, const glm::vec2& Max1, const glm::vec2& Min2, const glm::vec2& Max2) {
            return (Min2.x >= Min1.x && Max2.x <= Max1.x) &&
                   (Min2.y >= Min1.y && Max2.y <= Max1.y);
        }
        ```

        These formulas check if `AABB2` is entirely contained within `AABB1`. If all conditions are met, the function returns `true`, indicating that `AABB2` is inside `AABB1`. Otherwise, it returns `false`.
        */

         // Check the X, Y, and Z dimensions
        bool xOverlap = (aabb0.Min.x <= aabb1.Max.x) && (aabb0.Max.x >= aabb1.Min.x);
        bool yOverlap = (aabb0.Min.y <= aabb1.Max.y) && (aabb0.Max.y >= aabb1.Min.y);
        bool zOverlap = (aabb0.Min.z <= aabb1.Max.z) && (aabb0.Max.z >= aabb1.Min.z);

        // If all X, Y, and Z dimensions overlap, return true; otherwise, return false.
        return xOverlap && yOverlap && zOverlap;
    }
    bool BasicCollision::RayInAABB(const Ray& ray, const AABB& aabb, float& tmin, float& tmax)
    {
        /**
        *
        * Certainly! Let's break down why the AABB (Axis-Aligned Bounding Box) intersection algorithm works step by step:
        1. **Calculating t-values along each axis (X, Y, Z):**
           - The ray is defined by an origin point (`ray.Origin`) and a direction vector (`ray.Direction`).
           - To find the intersection with the AABB, we calculate `t` values for each axis (X, Y, and Z) using the parametric equation of a ray: `ray.Origin + t * ray.Direction`.

        2. **Sorting and comparing t-values:**
           - We sort the `t` values along each axis to ensure that `tmin` is the smaller value and `tmax` is the larger value.
           - If `tmin` is greater than `tmax` along any axis, it means the ray is pointing away from the AABB along that axis, and there can be no intersection. This check efficiently eliminates non-intersecting cases.

        3. **Determining the intersection intervals:**
           - We calculate `tymin` and `tymax` for the Y-axis and `tzmin` and `tzmax` for the Z-axis. These values represent the intervals along each axis where the ray intersects the AABB.
           - By comparing these intervals with the intervals along the X-axis (`tmin` and `tmax`), we check if they overlap. If any of the intervals do not overlap, it means there is no intersection in that dimension.
           - For example, if the interval along the X-axis does not overlap with the interval along the Y-axis, it means the ray is either above or below the AABB, and there is no intersection.

        4. **Returning the result:**
           - If all three intervals along the X, Y, and Z axes overlap, it means the ray intersects the AABB in all dimensions.
           - If any of the intervals do not overlap, it means there is no intersection.
           - Therefore, we return `true` if all intervals overlap and `false` if any of them do not.

        In summary, this algorithm leverages the properties of AABBs (specifically, that they are aligned with the coordinate axes) to efficiently check for intersection. 
        By calculating and comparing the intervals along each axis, we determine whether the ray intersects the AABB in 3D space. 
        If the intervals overlap along all axes, we consider it an intersection; otherwise, it's not. 
        This approach minimizes unnecessary calculations and checks, making it a fast and effective way to handle AABB-ray intersection tests.
        */

        /**
        *In the context of ray-AABB (Axis-Aligned Bounding Box) intersection testing, `tmin` and `tmax` represent the minimum and maximum values of the parameter `t` along the ray where the intersection with the AABB occurs.
        Here's a breakdown of what `tmin` and `tmax` mean:
        1. `t` is a parameter that represents a point along the ray. It starts at some minimum value and increases as you move along the ray.
        2. `tmin` represents the minimum `t` value at which the ray enters the AABB. In other words, it's the point along the ray where the ray first intersects the AABB from outside (if it does intersect). If there's no intersection, `tmin` will be greater than `tmax`.
        3. `tmax` represents the maximum `t` value at which the ray exits the AABB. It's the point along the ray where the ray exits the AABB if it entered it. If there's no intersection, `tmin` will still be greater than `tmax`, indicating no intersection.
        When you perform an AABB-ray intersection test, you're essentially trying to find the values of `t` at which the ray enters and exits the AABB, if at all. If `tmin` is greater than `tmax`, it means there's no intersection along the ray's path. If `tmin` is less than or equal to `tmax`, there is an intersection, and the range between `tmin` and `tmax` represents the portion of the ray that lies inside the AABB.
        These `t` values are crucial for various purposes, such as determining the intersection point in 3D space, calculating the hit position, and performing ray tracing or collision detection with objects represented by AABBs.
        */

        tmin = (aabb.Min.x - ray.Origin.x) / ray.Direction.x;
        tmax = (aabb.Max.x - ray.Origin.x) / ray.Direction.x;

        if (tmin > tmax)
        {
            std::swap(tmin, tmax);
        }

        float tymin = (aabb.Min.y - ray.Origin.y) / ray.Direction.y;
        float tymax = (aabb.Max.y - ray.Origin.y) / ray.Direction.y;

        if (tymin > tymax)
        {
            std::swap(tymin, tymax);
        }

        if ((tmin > tymax) || (tymin > tmax))
        {
            return false;
        }

        if (tymin > tmin)
        {
            tmin = tymin;
        }

        if (tymax < tmax)
        {
            tmax = tymax;
        }

        float tzmin = (aabb.Min.z - ray.Origin.z) / ray.Direction.z;
        float tzmax = (aabb.Max.z - ray.Origin.z) / ray.Direction.z;

        if (tzmin > tzmax)
        {
            std::swap(tzmin, tzmax);
        }

        if ((tmin > tzmax) || (tzmin > tmax))
        {
            return false;
        }

        if (tzmin > tmin)
        {
            tmin = tzmin;
        }

        if (tzmax < tmax)
        {
            tmax = tzmax;
        }

        return true;
    }
    bool BasicCollision::RayInAABB(const Ray& ray, const AABB& aabb, float& t)
    {
        glm::vec3 dirfrac;
             // r.dir is unit direction vector of ray
        dirfrac.x = 1.0f / ray.Direction.x;
        dirfrac.y = 1.0f / ray.Direction.y;
        dirfrac.z = 1.0f / ray.Direction.z;
        // lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
        // r.org is origin of ray
        const glm::vec3& lb = aabb.Min;
        const glm::vec3& rt = aabb.Max;
        float t1 = (lb.x - ray.Origin.x) * dirfrac.x;
        float t2 = (rt.x - ray.Origin.x) * dirfrac.x;
        float t3 = (lb.y - ray.Origin.y) * dirfrac.y;
        float t4 = (rt.y - ray.Origin.y) * dirfrac.y;
        float t5 = (lb.z - ray.Origin.z) * dirfrac.z;
        float t6 = (rt.z - ray.Origin.z) * dirfrac.z;

        float tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
        float tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));

        // if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
        if (tmax < 0)
        {
            t = tmax;
            return false;
        }

        // if tmin > tmax, ray doesn't intersect AABB
        if (tmin > tmax)
        {
            t = tmax;
            return false;
        }

        t = tmin;
        return true;
    }
    bool BasicCollision::RayInAABB(const Ray& ray, const AABB& aabb, float tmin, float tmax, float& t)
    {
        auto t0 = (aabb.Min - ray.Origin) * (1.f/ray.Direction);
        auto t1 = (aabb.Max - ray.Origin) * (1.f / ray.Direction);

        auto tsmall = glm::min(t0, t1);
        auto tbig = glm::max(t0, t1);

        auto tminf = glm::max(tmin, glm::max(tsmall.x, glm::max(tsmall.y, tsmall.z)));
        auto tmaxf = glm::min(tmax, glm::min(tbig.x, glm::min(tbig.y, tbig.z)));

        t = tminf;

        return (tminf <= tmaxf);
    }
}
