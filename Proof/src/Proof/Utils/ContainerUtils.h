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
	template<typename Vector, typename T>
	inline bool ContainsAny(const Vector& v, const std::vector<T>& items)
	{
		for (const T& item : items) 
		{
			if (Contains(v, item))
				return true;
		}
		return false;

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

	template <typename Vector>
	inline bool RemoveAtIndex(Vector& v, uint32_t index)
	{
		if (v.size() <= index)
			return false;

		v.erase(v.begin()+index);
		return true;
	}

	// New Swap function
	template <typename Vector>
	inline bool SwapIndexes(Vector& v, uint32_t index1, uint32_t index2)
	{
		if (index1 >= v.size() || index2 >= v.size())
			return false;

		std::swap(v[index1], v[index2]);
		return true;
	}


	// Find the index of an element in the vector
	template <typename Vector, typename T>
	inline int FindIndex(const Vector& v, const T& item)
	{
		auto it = std::find(v.begin(), v.end(), item);
		if (it != v.end())
		{
			return static_cast<int>(std::distance(v.begin(), it));
		}
		return -1; // Return -1 if the element is not found
	}

	// Move an element from one index to another
	template <typename Vector>
	inline bool MoveElement(Vector& v, uint32_t fromIndex, uint32_t toIndex)
	{
		if (fromIndex >= v.size() || toIndex >= v.size())
			return false;

		auto element = std::move(v[fromIndex]);
		v.erase(v.begin() + fromIndex);
		v.insert(v.begin() + toIndex, std::move(element));
		return true;
	}

	// Get a subvector from a given range
	template <typename Vector>
	inline Vector SubVector(const Vector& v, size_t start, size_t end)
	{
		if (start > end || end > v.size())
			return {};

		return Vector(v.begin() + start, v.begin() + end);
	}

	// Apply a function to all elements in the vector
	template <typename Vector, typename Function>
	inline void ForEach(const Vector& v, Function func)
	{
		std::for_each(v.begin(), v.end(), func);
	}

	// Apply a function to all elements in the vector (backwards)
	template <typename Vector, typename Function>
	inline void ForEachBackward(const Vector& v, Function func)
	{
		std::for_each(v.rbegin(), v.rend(), func);
	}

	// Sort the vector based on a comparator function
	template <typename Vector, typename Compare>
	inline void Sort(Vector& v, Compare comp)
	{
		std::sort(v.begin(), v.end(), comp);
	}

	// Reverse the elements in the vector
	template <typename Vector>
	inline void Reverse(Vector& v)
	{
		std::reverse(v.begin(), v.end());
	}

	/*
	* hass to be ordered
	  std::vector<int> numbers = {1, 3, 4};
    std::vector<int> missing = findMissingNumbers<int>(numbers);  // Return missing numbers

    // Print the missing numbers
    for (int num : missing) {
        std::cout << "Missing number: " << num << std::endl;
    }

	*/
	template <typename T, typename Container>
	inline std::vector<T> FindMissingNumbers(const Container& numbers) 
	{
		std::vector<T> missingNumbers;

		for (int i = 0; i < numbers.size() - 1; ++i) {
			// Check if the difference between consecutive numbers is greater than 1
			if (numbers[i + 1] - numbers[i] > 1) {
				// Missing numbers are between numbers[i] and numbers[i + 1]
				for (T j = numbers[i] + 1; j < numbers[i + 1]; ++j) {
					missingNumbers.push_back(j);
				}
			}
		}
		return missingNumbers;
	}

}