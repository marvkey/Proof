#pragma once
#include "Proof/Renderer/Buffer.h"
namespace Proof {
   class Proof_API OpenGLVertexBuffer: public VertexBuffer {
   public:
	  OpenGLVertexBuffer(uint16_t Size = 1);
	  ~OpenGLVertexBuffer();
	  void BindVertexBuffer();
	  void AddVertexBufferData(void* Data,unsigned int Size);
	  void UnBind();
   private:
	  unsigned int VertexBufferObject;
	  uint16_t VertexBufferSize;
   };

   class Proof_API OpenGLIndexBuffer: public IndexBuffer {
   public:
	  OpenGLIndexBuffer(uint16_t Size = 1);
	  ~OpenGLIndexBuffer();
	  void AddIndexBufferData(void* Data,unsigned int Size);
	  void BindIndexBuffer();
	  void UnBind();
   private:
	  unsigned int IndexBufferObject;
	  uint16_t IndexBufferSize;
   };
}