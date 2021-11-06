#pragma once
#include "Proof/Resources/Math/Math.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Proof/Resources/Math/Vector.h"

namespace Proof
{
    struct Vertex {
        Vector Vertices{0,0,0};
        Vector Normal{0,0,0};
        glm::vec2 TexCoords{0,0};
        Vector Tangent{0,0,0};
        Vector Bitangent{0,0,0};
    };

    struct Vertex2D {
        Vector Position;
        glm::vec4 Color;
        glm::vec2 TexCoords;
        float TexSlot;
    };
}