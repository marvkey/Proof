#include "Proofprch.h"
#include "Mesh.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/Shader.h"
#include "Platform/OpenGL/Texture.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include<vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace Proof {
    Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _Indices, std::vector<Texture> _Textures):
        Vertices(_vertices), Indices(_Indices), Textures(_Textures)
    {
        SetUpMesh();
    }
    
    void Mesh::Draw(Shader& _Shader) {
        unsigned int DiffuseNR = 1;
        unsigned int SpecularNR = 1;
        for (unsigned int i = 0; i < Textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + 1);
            std::string Number;
            std::string Name = Textures[i].GetTextureType();
            if (Name == "texture_diffuse")
                Number = std::to_string(DiffuseNR++);
            else if (Name == "texture_specular")
                Number == std::to_string(SpecularNR++);

            _Shader.SetFloat(("Material." + Name + Number), i);
            Textures[i].BindTexture();
        }
        //`VertexArray1.BindVertexArray();
        glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
    }
    
    void Mesh::SetUpMesh(){
        OpenGLVertexArray VertexArray1;

        OpenGLVertexBuffer Buffer1;
        OpenGLIndexBuffer IndexBuffer1;
        VertexArray1.BindVertexArray();
        Buffer1.AddVertexBufferData(&Vertices[0], Vertices.size() * sizeof(Vertex));
        IndexBuffer1.AddIndexBufferData(&Indices[0], Indices.size() * sizeof(unsigned int));

        VertexArray1.AddAtributePointerInt(0, 3, sizeof(Vertex), 0);
        VertexArray1.AddAtributePointervoid(1, 3, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        VertexArray1.AddAtributePointervoid(2, 2, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        VertexArray1.UnBind();
    }
}