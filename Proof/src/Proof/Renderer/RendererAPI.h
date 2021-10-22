#pragma once
#include <glm/glm.hpp>
#include"VertexArray.h"
namespace Proof {
	struct ProofClear{
		enum Clear:uint32_t {
			DepthBuffer = GL_DEPTH_BUFFER_BIT,
			StencilBuffer = GL_STENCIL_BUFFER_BIT,
			ColourBuffer = GL_COLOR_BUFFER_BIT
		};
	};

	struct ProofRenderTest{
		enum RenderTest:uint32_t {
			Blend = GL_BLEND,
			DepthTest =GL_DEPTH_TEST,
			CullFace = GL_CULL_FACE
		};
	};

	enum class DepthType:uint32_t{
		Equal = GL_LEQUAL,
		Less = GL_LESS
	};
	enum class DrawType: uint32_t {
		Triangles = GL_TRIANGLES,
		Points = GL_POINTS,
		Lines= GL_LINES,
		Quads = GL_QUADS
	};
	class Proof_API RendererAPI {
	public:
		enum class API {
			None =0, OpenGL =1, Vulkan=2
		};
		
		virtual void SetClearColor(const glm::vec4&Color) = 0;
		virtual void SetClearColor(float R, float G, float B, float A)= 0;
		virtual void DrawArrays(uint32_t Count,DrawType drawType = DrawType::Triangles)=0;
		virtual void DrawIndexed(const Count<VertexArray>& ArrayObject,DrawType drawType =DrawType::Triangles) = 0;
		virtual void DrawIndexed(const Count<VertexArray>& ArrayObject,uint32_t Count,DrawType drawType = DrawType::Triangles) = 0;
		virtual void DrawElementIndexed(const Count<VertexArray>& ArrayObject,uint32_t Count,uint32_t AmountElement,DrawType drawType = DrawType::Triangles) =0;
		virtual void DrawElementIndexed(const Count<VertexArray>& ArrayObject,uint32_t AmountElement,DrawType drawType = DrawType::Triangles) = 0;
		
		inline static API GetAPI() { return ActiveAPI; }

		/**
		* @param bitfield use the enum form the struct Proof Clear
		*/ 
		virtual void Clear(uint32_t bitField) = 0;
		/**
		* @param bitfield use the enum form the struct ProofRenderTest
		*/
		virtual void Enable(uint32_t bitField) =0;
		/**
		* @param bitfield use the enum form the struct ProofRenderTest
		*/
		virtual void Disable(uint32_t bitField) =0;

		virtual void SetViewPort(uint32_t width,uint32_t height) = 0;
		virtual void DepthFunc(DepthType type) =0;
	private:
		static API ActiveAPI;
	};
}