#pragma once
#include "Proof/Core/Core.h"
namespace Proof
{

	class CommandBuffer : public RefCounted {
	public:
		static Count<CommandBuffer>Create();
		virtual ~CommandBuffer() = default;
		template<class T>
		T* As() {
			return  dynamic_cast<T*>(this);
		}
		
	};

	class RenderCommandBuffer : public RefCounted{
	public:
		static Count<RenderCommandBuffer >Create(CommandBuffer* buffer =nullptr);
		virtual ~RenderCommandBuffer() = default;
	};
}