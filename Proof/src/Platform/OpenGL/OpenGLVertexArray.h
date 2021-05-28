#pragma once
#include<GL/glew.h>
#include "Proof/Renderer/VertexArray.h"
namespace Proof {
   class Proof_API OpenGLVertexArray:public VertexArray {
   public:
	  OpenGLVertexArray(uint16_t Size = 1);
	  ~OpenGLVertexArray();
	  void BindVertexArray();
	  void AddAtributePointerInt(int Position,int Count,int SizeOfOneVertex,int Offset); /// this is when we know the offset from using static array
	  void AddAtributePointervoid(int Position,int Count,int SizeOfOneVertex,const void* Offset);// this is when we watn tot write a number
	  void UnBind();
   private:
	  unsigned int VertexArrayObject;
	  uint16_t VertexArraySize;
   };
}

