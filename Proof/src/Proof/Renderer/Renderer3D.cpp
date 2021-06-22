#include "Proofprch.h"
#include "Renderer3D.h"
#include "Shader.h"
//#include "Platform/Window/CurrentWindow.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Proof3D/Scene/Mesh.h"
#include "Proof3D/Scene/Component.h"
namespace Proof {
	struct Renderer3DStorage {
		Count<VertexArray> m_VertexArray;
		Count<Shader> m_Shader;
        Count<Texture2D> m_NullTexture;
	};
    static BatchRenderer3D* s_BatchRender3D;
	static Renderer3DStorage* s_Storage3DData;
	void Renderer3D::Init() {
        /*
         GLuint indices[] = {
            0,1,5,5,1,6,
            1,2,6,6,2,7,
            2,3,7,7,3,8,
            3,4,8,8,4,9,
            10,11,0,0,11,1,
            5,6,12,12,6,13
        };

        float vertices[] = {
            -1,-1,-1,0,0,
            1,-1,-1,1,0,
            1,1,-1,2,0,
            -1,1,-1,3,0,
            -1,-1,-1,4,0,

            -1,-1,1,0,1,
            1,-1,1,1,1,
            1,1,1,2,1,
            -1,1,1,3,1,
            -1,-1,1,4,1,

            -1,1,-1,0,-1,
            1,1,-1,1,-1,

            -1,1,1,0,2,
            1,1,1,1,2
        };

        
		s_Storage3DData = new Renderer3DStorage();
		s_Storage3DData->m_Shader = Shader::Create("MeshShader",ProofCurrentDirectorySrc+"Proof/Renderer/Asset/Shader/3D/MeshShader.shader");
        s_Storage3DData->m_NullTexture = Texture2D::Create(1,1);
        uint32_t  EmptyTextureData = 0xfffffffff;
        s_Storage3DData->m_NullTexture->SetData(&EmptyTextureData,sizeof(uint32_t));
        s_Storage3DData->m_VertexArray = VertexArray::Create();
        Count<VertexBuffer> m_VertexBuffer = VertexBuffer::Create(vertices,sizeof(vertices));
        Count<IndexBuffer> m_IndexBuffer = IndexBuffer::Create(indices,sizeof(indices));
        m_VertexBuffer->Bind();
        m_IndexBuffer->Bind();

        s_Storage3DData->m_VertexArray->AttachIndexBuffer(m_IndexBuffer);
        s_Storage3DData->m_VertexArray->AddData(0,3, 5* sizeof(float),(void*)0);
        s_Storage3DData->m_VertexArray->AddData(1,2,5 * sizeof(float),(void*)(3 * sizeof(float)));
        s_Storage3DData->m_VertexArray->UnBind();
        s_Storage3DData->m_Shader->UseShader();
        s_Storage3DData->m_Shader->SetInt("u_Texture",0);

        s_BatchRender3D = new BatchRenderer3D();
        */
    }
	void Renderer3D::ShutDown() {
		delete s_Storage3DData;
	}
	void Renderer3D::BeginContext(const glm::mat4& Projection,const Camera& camera) {
		s_Storage3DData->m_Shader->UseShader();
		s_Storage3DData->m_Shader->SetMat4("u_Projection",Projection);
		s_Storage3DData->m_Shader->SetMat4("u_View",camera.GetCameraView());
	}
	void Renderer3D::DrawCube(const Vector& Location,const Vector& Scale) {
		glm::mat4 Transform = glm::mat4(1.0f);
		Transform = glm::translate(Transform,{Location});
		Transform = glm::scale(Transform,{Scale});
        DrawCube({Transform});
	}
    void Renderer3D::DrawCube(const Vector& Location,const Vector& Scale,const glm::vec4& Color) {
        glm::mat4 Transform = glm::mat4(1.0f);
        Transform = glm::translate(Transform,{Location});
        Transform = glm::scale(Transform,{Scale});
        DrawCube({Transform},Color);
    }
    void Renderer3D::DrawCube(const Vector& Location,const Vector& Scale,const Count<Texture2D>& Texture) {
        glm::mat4 Transform = glm::mat4(1.0f);
        Transform = glm::translate(Transform,{Location});
        Transform = glm::scale(Transform,{Scale});
        DrawCube({Transform},Texture);
    }
    void Renderer3D::DrawCube(const Vector& Location,const Vector& Scale,const Count<Texture2D>& Texture,const glm::vec4& TintColor) {
        glm::mat4 Transform = glm::mat4(1.0f);
        Transform = glm::translate(Transform,{Location});
        Transform = glm::scale(Transform,{Scale});
        DrawCube({Transform},Texture,TintColor);
    }

    void Renderer3D::DrawCube(const glm::mat4& Transform,const glm::vec4& Color) {
        s_Storage3DData->m_Shader->SetVec4("u_Color",Color);
        s_Storage3DData->m_Shader->SetMat4("u_Model",Transform);
        s_Storage3DData->m_NullTexture->BindTexture();
        RendererCommand::DrawIndexed(s_Storage3DData->m_VertexArray);
    }
    void Renderer3D::DrawCube(const glm::mat4& Transform,Count<Texture2D>Tex,const glm::vec4& TintColor) {
        s_Storage3DData->m_Shader->SetVec4("u_Color",TintColor);
        s_Storage3DData->m_Shader->SetMat4("u_Model",Transform);
        Tex->BindTexture();
        RendererCommand::DrawIndexed(s_Storage3DData->m_VertexArray);
        Tex->unBind();
    }

    void Renderer3D::DrawMesh(StaticMeshComponent& MeshComponent) {
        PF_ENGINE_INFO("ADDING MESH");
        if (MeshComponent.GetModel() == nullptr) {
            return;
        }
        for (int i = 0; i < MeshComponent.GetModel()->GetMesh().size(); i++) {
                s_BatchRender3D->AttachVertex(MeshComponent.GetModel()->GetMesh()[i].m_Vertices);
        }
        PF_ENGINE_INFO("DONE ADDING MESH");
    }
	void Renderer3D::EndContext() {
        /*
        s_BatchRender3D->m_IndexBufferObject = IndexBuffer::Create(&s_BatchRender3D->m_Vertices[0],s_BatchRender3D->m_Vertices.size());
        s_BatchRender3D->m_VertexBufferObject->Bind();
        s_BatchRender3D->m_IndexBufferObject->Bind();
        s_BatchRender3D->m_VertexArrayObject->AttachIndexBuffer(s_BatchRender3D->m_IndexBufferObject);
        RendererCommand::DrawIndexed(s_BatchRender3D->m_VertexArrayObject);
        */
    }

    BatchRenderer3D::BatchRenderer3D() {
        m_VertexArrayObject = VertexArray::Create();
        m_VertexBufferObject = VertexBuffer::Create(MaxVertices * sizeof(Vertex));
        m_VertexArrayObject->AddData(0,3,sizeof(Vertex),(void*)offsetof(Vertex,Vertices));
        m_VertexArrayObject->AddData(1,3,sizeof(Vertex),(void*)offsetof(Vertex,Normal));
        m_VertexArrayObject->AddData(2,2,sizeof(Vertex),(void*)offsetof(Vertex,TexCoords));
        m_VertexArrayObject->AddData(3,3,sizeof(Vertex),(void*)offsetof(Vertex,Tangent));
        m_VertexArrayObject->AddData(4,3,sizeof(Vertex),(void*)offsetof(Vertex,Bitangent));
    }
    void BatchRenderer3D::AttachVertex(std::vector<Vertex>& Vertices) {
        PF_ENGINE_INFO("%i",Vertices.size());
        /*
        if (m_Vertices.size() == 0) {
            m_VertexBufferObject->AddData(&Vertices[0],NumofVertices);
        }
        */
        NumofVertices += Vertices.size();

        //m_Vertices.insert(m_Vertices.end(),Vertices.begin(),Vertices.end());
    }

    void BatchRenderer3D::AttachVertex(const Vector& Vertices,const Vector& Normal,const glm::vec2& TexCoords,const Vector& Tangent,const Vector& Bitangent) {
        Vertex TempVertex;
        TempVertex.Vertices = Vertices;
        TempVertex.Normal = Normal;
        TempVertex.TexCoords = TexCoords;
        TempVertex.Tangent = Tangent;
        TempVertex.Bitangent = Bitangent;
        if (m_Vertices.size() == 0) {
            m_VertexBufferObject->AddData(&TempVertex,0);
        }
        else
            m_VertexBufferObject->AddData(&TempVertex,m_Vertices.size());
        m_Vertices.emplace_back(TempVertex);
    }

    void BatchRenderer3D::AttachVertex(const Vertex& vertex) {
        //m_VertexBufferObject->AddData(&vertex,NumofVertices);
        /*
        Vertex TempVertex = vertex;
        if (m_Vertices.size() == 0) {
            m_VertexBufferObject->AddData(&TempVertex,0);
        }
        else
            m_VertexBufferObject->AddData(&TempVertex,m_Vertices.size());
        m_Vertices.emplace_back(TempVertex);
        */
    }
    void BatchRenderer3D::AttachIndex(std::vector<uint32_t>& Indices) {
        for (uint32_t Element : Indices) {
            m_Indices.emplace_back(Element + m_Vertices.size());
        }
    }
}