#pragma once
#include "Proof/Core/Assert.h"
#include <map>
namespace Proof
{
	struct RenderCustomTypeSetHelper
	{
		static uint32_t GetCurrentFrameInFlight();
		static uint32_t RT_GetCurrentFrameInFlight();
		static uint32_t GetMaxFrameFlight();
	};
	template<typename TCustomType>
	class RendererCustomTypeSet
	{
	public:
		template <class... Args, std::enable_if_t<std::is_constructible<TCustomType, Args...>::value, int> = 0>
		RendererCustomTypeSet(Args&&... args)
		{
			for (uint32_t index = 0; index < RenderCustomTypeSetHelper::GetMaxFrameFlight(); index++)
			{
				m_Values[index] = TCustomType(std::forward<Args>(args)...);
			}
		}
		TCustomType& GetByIndex(uint32_t index)
		{
			PF_CORE_ASSERT(m_Values.contains(index), "RendererCustomTypeSe Set Does not contain index");
			return m_Values.at(index);
		}

		TCustomType& Get()
		{
			return GetByIndex(RenderCustomTypeSetHelper::GetCurrentFrameInFlight());
		}
		TCustomType& RT_Get()
		{
			return GetByIndex(RenderCustomTypeSetHelper::RT_GetCurrentFrameInFlight());
		}

		void ForEach(const std::function<void(TCustomType&)>& func)
		{
			for (auto& pair : m_Values)
			{
				func(pair.second);
			}
		}
		uint32_t GetCount()
		{
			return m_Values.size();
		}
	private:
		std::map<uint32_t, TCustomType> m_Values;
		
	};
	
	

}