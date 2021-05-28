#pragma once
#include <glm/glm.hpp>

namespace Proof {
    class Texture;
    class Shader;
    class VertexArray;
    class IndexBuffer;
    class VertexBuffer;
   struct Proof_API Vertex {
       glm::vec3 Postion;
       glm::vec2 Normal;
       glm::vec2 TexCoords;
    };

   class Proof_API Mesh {
   public:
       std::vector<Vertex> Vertices;
       std::vector<unsigned int> Indices;
       std::vector<Texture> Textures;
       Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _Indices, std::vector<Texture> Textures);
   private:
      void Draw(Shader& _Shader);
      void SetUpMesh();
   };
}