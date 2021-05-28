#pragma once
#include "Proof3D/Math/Vector3.h"
namespace Proof {
    struct Material {
        Vector3 Ambient;
        Vector3 diffuse;
        Vector3 specular;
        float shininess;
    };
}