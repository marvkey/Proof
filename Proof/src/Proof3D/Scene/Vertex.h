#pragma once
struct Vertex {
    Vector Vertices;
    Vector Normal;
    glm::vec2 TexCoords;
    Vector Tangent;
    Vector Bitangent;
};

struct Vertex2D {
    Vector Position;
    glm::vec4 Color;
    glm::vec2 TexCoords;
    float TexSlot;
};
