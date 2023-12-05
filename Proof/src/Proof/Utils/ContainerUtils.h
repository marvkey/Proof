#pragma once
#include <vector>
#include <algorithm>
namespace Proof::Utils
{
	template<typename Vector, typename T>
	inline bool Contains(const Vector& v, T&& item)	
	{
		return  std::find(std::begin(v), std::end(v), std::forward<T>(item)) != std::end(v);
	}

	template<typename T, typename ConditionFunction>
	inline bool RemoveIf(std::vector<T>& vector, ConditionFunction condition)
	{
		auto it = std::remove_if(vector.begin(), vector.end(), condition);
		if (it != vector.end())
		{
			vector.erase(it, vector.end());
			return true;
		}
		return false;
	}

	template <typename Vector, typename ItemType>
	inline bool Remove(Vector& v, ItemType&& itemToRemove)
	{
		auto found = std::find(std::begin(v), std::end(v), itemToRemove);

		if (found == std::end(v))
			return false;

		v.erase(found);
		return true;
	}
}