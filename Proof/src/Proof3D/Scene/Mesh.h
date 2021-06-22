#pragma once

#include"Proof3D/Math/Vector.h"
#include <vector>
#include "Proof/Renderer/Texture.h"
#include "Proof/Renderer/Shader.h"
#include "Proof/Renderer/RendererCommand.h"
#include "Proof3D/Scene/Component.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include <unordered_map>
#include "Component.h"
#include "Vertex.h"

class Mesh {
public:
    TransformComponent MeshTransform;
    TagComponent MeshTags;
    Mesh(std::vector<Vertex>& Vertices,std::vector<uint32_t>& Indices,std::vector<Proof::Count<Proof::Texture2D>>& Textures);
    Mesh(std::vector<Vertex>& Vertices,std::vector<uint32_t>& Indices);
    void Draw(const Proof::Count<Proof::Shader>& shader);
    std::vector<Vertex> m_Vertices;
    std::vector<uint32_t> m_Indices;
private:
    Proof::Count<Proof::VertexArray> VertexArrayObject;
    Proof::Count<Proof::VertexBuffer> VertexBufferObject;
    Proof::Count<Proof::IndexBuffer> IndexBufferObject;
    void SetupMesh();
    glm::mat4 ModelMatrix= glm::mat4(1.0f);
    void InitMatrix();
    std::vector<Proof::Count<Proof::Texture2D>>m_Textures;
    std::unordered_map<std::string,uint8_t>m_MapTextures;
    void InitilizeMapTextures();
    friend class Renderer3D;
};
