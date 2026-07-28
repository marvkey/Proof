#pragma once
#include <glm/glm.hpp>
#include "Proof/Math/MathResource.h"
#include <glm/gtx/quaternion.hpp>
#include "Proof/Math/Math.h"
namespace Proof
{
	struct Transform
	{
		glm::vec3 Location = glm::vec3(1);
		glm::vec3 Rotation = glm::vec3(0); // deggrees
		glm::vec3 Scale = glm::vec3(1);

        Transform() {};
        Transform(const glm::mat4& transform) { SetTransform(transform); };
		void SetTransform(const glm::mat4& transform)
		{
			glm::quat rotationQuat;
			MathResource::DecomposeTransform(transform, Location, rotationQuat, Scale);
			Rotation = glm::eulerAngles(rotationQuat);
		}
        glm::mat4 GetTransform() const {
            return glm::translate(glm::mat4(1.0f), Location)
                * glm::toMat4(glm::quat(Rotation))
                * glm::scale(glm::mat4(1.0f), Scale);
        }
        std::string ToString();         

	};



    // for c# cause glm some stupid reason starts with w
    struct QuaternionProper
    {
        float X, Y, Z, W; 

    };
    namespace Utils
    {
        // crates matrix form location, rotation, scale
        inline glm::mat4 MatrixTRS(glm::vec3 location, glm::quat rotation, glm::vec3 scale)
        {
            return glm::translate(glm::mat4(1.0f), location)
                * glm::toMat4(rotation)
                * glm::scale(glm::mat4(1.0f), scale);
        }
        inline glm::quat QuaternionProperToGlm(QuaternionProper proper)
        {
            glm::quat quat;
            quat.x = proper.X;
            quat.y = proper.Y;
            quat.z = proper.Z;
            quat.w = proper.W;

            return quat;
        }

        inline QuaternionProper GlmToQuaternionProper(glm::quat quat)
        {
            QuaternionProper proper;
            proper.X = quat.x ;
            proper.Y = quat.y ;
            proper.Z = quat.z ;
            proper.W = quat.w ;

            return proper;
        }


        inline std::vector<glm::vec3> SamplePlanePoints(float planeSize)
        {
            std::vector<glm::vec3>Positions;

            float offset = 0.8f; // so points dont be on the corner cause on a gerstner wave the corners bend

            float halfSize = (planeSize * offset) / 2.f;

            float verticalStep = (planeSize * offset) / 4.0f; // divide plane with 4 intervals
            float horizontalStep = (planeSize * offset) / 4.0f; // divded pplane with 4 interaval

            for (int col = 0; col < 5; col++)
            {
                float x = -halfSize + col * horizontalStep;

                //loop each row in a column

                for (int row = 0; row < 5; row++)
                {
                    float z = halfSize - row * verticalStep;

                    Positions.push_back(glm::vec3(x, 0.0f, z));
                }
            }

            return Positions;
        }
        //InverseTransformPoint
        inline glm::vec3 WorldToLocal(const glm::vec3& worldPosition, const glm::mat4& modelMatrix)
        {
            // First, inverse the model matrix to convert world position back to local space
            glm::mat4 inverseModelMatrix = glm::inverse(modelMatrix);

            // Transform the world position back to local space
            glm::vec3 localPosition = glm::vec3(inverseModelMatrix * glm::vec4(worldPosition, 1.0f));

            return localPosition;
        }

        //TransformPoint
        inline glm::vec3 LocalToWorld(const glm::vec3& localPosition, const glm::mat4& modelMatrix)
        {
            // Transform the local position to world space using the model matrix
            glm::vec3 worldPosition = glm::vec3(modelMatrix * glm::vec4(localPosition, 1.0f));

            return worldPosition;
        }

        inline glm::vec3 InverseTransformPoint(const glm::mat4& transform, const glm::vec3& worldPosition)
        {
            return glm::vec3(glm::inverse(transform) * glm::vec4(worldPosition, 1.0f));
        }

        inline glm::vec3 TransformPoint(const glm::mat4& transform, const glm::vec3& localPoint)
        {
            return glm::vec3(transform * glm::vec4(localPoint, 1.0f));
        }

        inline glm::quat FromRotation(glm::vec3 fromDirection, glm::vec3 toDirection)
        {
            glm::vec3 v0 = glm::normalize(fromDirection);
            glm::vec3 v1 = glm::normalize(toDirection);
            float d = glm::dot(v0, v1);

            glm::quat value;
            if (d > -1 + 1e-6)
            {
                float s = Math::SquareRoot((1 + d) * 2);
                float invs = 1 / s;
                glm::vec3 c = glm::cross(v0, v1) * invs;
                value = glm::quat(s * 0.5f,c.x, c.y, c.z);
            }
            else if (d > 1 - 1e-6)
            {
                value = glm::quat(1, 0, 0, 0);
            }
            else
            {
                glm::vec3 axis = glm::cross(Math::GetRightVector(), v0);
                if (glm::length2(axis) < 1e-6)
                {
                    axis = glm::cross(Math::GetRightVector(), v0);
                }
                value = glm::quat(0,axis.x, axis.y, axis.z);
            }

            return value;
        }
    };
    // still neeeds some work to be ready
    // able to edit the min and  max at runtime could be a positibily
    template<typename T, T MinVal = std::numeric_limits<T>::min(), T MaxVal = std::numeric_limits<T>::max()>
    struct VariableWithRange
    {
        T value;   // Only the value takes up space

        // Constructor that clamps the initial value
        VariableWithRange(T val) : value(std::clamp(val, MinVal, MaxVal)) {}

        // Overload assignment operator to clamp the value on assignment
        VariableWithRange& operator=(T val) {
            value = std::clamp(val, MinVal, MaxVal);
            return *this;
        }

        // Implicit conversion operator to retrieve the value
        operator T() const {
            return value;
        }
    };

    enum class VisibilityState
    {
        VisibleToAll = 1,
        VisiblePlayerOnly, // only player can see it 
        HiddenFromPlayer,// everyne else can see it apart from player
        VisibleSelecedPlayers,// only selected players can see it
        HiddenToAll, // no one can see it
    };
    DEFINE_ENUM_CLASS_FLAGS(VisibilityState);

    struct Visibility
    {
        
        VisibilityState State;
        // Overload the bool operator
        operator bool() const 
        {
            return State == VisibilityState::VisibleToAll;
        }

        // Set the visibility using a boolean
        void operator=(bool isVisible) 
        {
            State = isVisible ? VisibilityState::VisibleToAll : VisibilityState::HiddenToAll;
        }

        // Overload the cast to VisibilityState
        operator VisibilityState() const 
        {
            return State;
        }

    };
}