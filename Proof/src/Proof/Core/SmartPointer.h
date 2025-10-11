#pragma once
#include <unordered_set>
#include <atomic>
#include <compare>
#include <stdint.h>
#include "MemoryAllocator.h"

namespace Proof {


	class RefCounted {
	public:
		RefCounted() {}; // non-atomic initializations

		RefCounted(const RefCounted& other)
		{
		}
		//RefCounted& operator=(const RefCounted& other)
		//{
		//}
		virtual ~RefCounted() {};


		uint32_t GetStrongCount() {
			return m_StrongRefCount;
		}

	protected:
		virtual void Delete()const {
			delete this;
		}
	private:
		void IncreaseStrongRef() {
			m_StrongRefCount++;
		}
		void ReleaseStrongRef() {
			m_StrongRefCount--;
			if (m_StrongRefCount == 0 && m_IsConstructed == true)
			{
				Delete();
			}
		}
		mutable std::atomic<uint32_t>  m_StrongRefCount = 0;
		mutable std::atomic<bool>  m_IsConstructed = false;
		template <class T>
		friend class Count;
	};
	namespace RefUtils {

		class RefCountManager
		{
		public:
			static void AddToLiveReference(void* instance)
			{
				if (instance == nullptr)
					return;
				uintptr_t memoryAddress = uintptr_t(instance);
				s_LiveReferences.insert(memoryAddress);
			};
			static void RemoveFromLiveReference(void* instance)
			{
				if (instance == nullptr)
					return;
				uintptr_t memoryAddress = uintptr_t(instance);
				s_LiveReferences.erase(memoryAddress);
			}
			static bool IsLive(void* instance)
			{
				if (instance == nullptr)
					return false;

				uintptr_t memoryAddress = uintptr_t(instance);

				return s_LiveReferences.contains(memoryAddress);
			}
		private:
			static inline std::unordered_set<uintptr_t> s_LiveReferences;
		};

		static void AddToLiveReference(void* instance)
		{
			RefCountManager::AddToLiveReference(instance);
		}
		static void RemoveFromLiveReference(void* instance)
		{
			RefCountManager::RemoveFromLiveReference(instance);
		}
		static bool IsLive(void* instance)
		{
			return RefCountManager::IsLive(instance);
		}
	}
	template <class _Yty, class _Ty>
	struct Is_Compatible : std::is_convertible<_Yty*, _Ty*>::type {
		// N4659 [util.smartptr.shared]/5 "a pointer type Y* is said to be compatible
		// with a pointer type T* " "when either Y* is convertible to T* ..."
	};
	template<class T>
	class Count
	{
	public:
		using element_type = std::remove_extent_t<T>;
		constexpr Count() noexcept : m_Ptr(nullptr)
		{
		}

		constexpr Count(nullptr_t) noexcept : m_Ptr(nullptr)
		{
		}

		//template <class Type = T, std::enable_if_t<std::is_base_of<RefCounted, Type>::value && std::enable_if_t<Is_Compatible<Type*, T*>::value, int> = 0 >>
		template <class Type, typename std::enable_if<std::is_base_of<RefCounted, Type>::value&& std::is_convertible<Type*, T*>::value, int>::type = 0>
		Count(Type* data) {
			this->m_Ptr = data;
			AddStrongRef();
		}
		/*
		//template <class Type, std::enable_if_t<Is_Compatible<Type, T>::value, int> = 0>
		template <typename U = T, typename std::enable_if<std::is_same<U, T>::value>::type* = nullptr>
		Count(const Count<U>& right, element_type* px) noexcept {
			// construct shared_ptr object that aliases _Right
			this->AliasConstructor(right, px);
		}

		template <class Type, std::enable_if_t<Is_Compatible<Type, T>::value, int> = 0>
		Count(Count<Type>&& right, element_type* px) noexcept {
			// construct shared_ptr object that aliases _Right
			this->AliasMoveConstructor(std::move(right), px);
		}
		*/
		Count(const Count& other) noexcept { // construct shared_ptr object that owns same resource as _Other
			this->CopyConstructor(other);
		}
		template <class Type, std::enable_if_t<Is_Compatible<Type, T>::value, int> = 0>
		Count(const Count<Type>& other) noexcept { // construct shared_ptr object that owns same resource as _Other
			this->CopyConstructor(other);
		}

		Count(Count&& right) noexcept { // construct shared_ptr object that takes resource from _Right
			this->MoveConstructor(std::move(right));
		}

		template <class Type, std::enable_if_t<Is_Compatible<Type, T>::value, int> = 0>
		Count(Count<Type>&& right) noexcept { // construct shared_ptr object that takes resource from _Right
			this->MoveConstructor(std::move(right));
		}
		~Count() {
			this->DecrementStrongRef();
		}
		Count& operator=(const Count& other) noexcept {
			Count(other).Swap(*this);
			return *this;
		}
		template <class Type, std::enable_if_t<Is_Compatible<Type, T>::value, int> = 0>
		Count& operator=(const Count<Type>& other)noexcept {
			Count(other).Swap(*this);
			return *this;
		}

		Count& operator=(Count&& _Right) noexcept { // take resource from _Right
			Count(std::move(_Right)).Swap(*this);
			return *this;
		}
		template <class Type, std::enable_if_t<Is_Compatible<Type, T>::value, int> = 0>
		Count& operator=(Count<Type>&& other)noexcept {
			Count(std::move(other)).Swap(*this);
			return *this;
		}

		T* operator->() const {
			return this->Get();
		}
		T* Get() const noexcept {
			return m_Ptr;
		}
		template <class _Ty2 = T, std::enable_if_t<!std::disjunction_v<std::is_array<_Ty2>, std::is_void<_Ty2>>, int> = 0>
		_Ty2& operator*() const noexcept {
			return *this->Get();
		}
		explicit operator bool() const noexcept {
			return this->Get() != nullptr;
		}
		template <class... Args, std::enable_if_t<std::is_constructible<T, Args...>::value, int> = 0>
		static Count Create(Args&&... args) 
		{
#if 1
	#if PF_TRACK_MEMORY
				Count<T> sharedPtr(new(typeid(T).name()) T(std::forward<Args>(args)...));
				RefCounted* ref = static_cast<RefCounted*>(sharedPtr.m_Ptr);
				ref->m_IsConstructed = true;
				return sharedPtr;
	#else
				Count<T> sharedPtr(new T(std::forward<Args>(args)...));
				RefCounted* ref = static_cast<RefCounted*>(sharedPtr.m_Ptr);
				ref->m_IsConstructed = true;
				return sharedPtr;
	#endif
#else
			// a
			// 1️ Allocate uninitialized memory for T
			//------------------------------------------------------------------------------
			// Count<T>::Create
			//
			// This factory function safely constructs a RefCounted object inside a Count<T>
			// smart pointer using *placement new*.
			//
			// Why this exists:
			// Normally, writing `Count<T>(new T(...))` calls T's constructor before the
			// Count<T> wrapper exists. If T internally creates a Count<T>(this) during
			// construction, that temporary smart pointer can increment and immediately
			// decrement the reference count before any real owner exists — causing the
			// object to destroy itself inside its own constructor.
			//
			// How this works:
			//  1. Allocate raw memory large enough for T (no constructor call yet).
			//  2. Create an empty Count<T> wrapper (m_Ptr = nullptr).
			//  3. Use placement new to construct T directly in the allocated memory.
			//  4. Attach the constructed T to the Count<T> wrapper.
			//  5. Manually add the first strong reference (refcount = 1).
			//  6. Return the Count<T> that now owns the object.
			//
			// Result:
			//  - The Count<T> "owner" exists *before* T's constructor finishes.
			//  - Self-references (Count<T>(this)) inside T's constructor are now safe.
			//  - Matches the semantics of std::make_shared (safe construction + ownership).
			//
			// Notes:
			//  - If T's constructor throws, we manually delete the raw memory to prevent leaks.
			//  - RefCounted::ReleaseStrongRef() must still handle destruction and memory free.
			//
			//------------------------------------------------------------------------------
			
			  // 1) Raw allocate
		#if PF_TRACK_MEMORY
					void* mem = operator new(sizeof(T), typeid(T).name());
		#else
					void* mem = operator new(sizeof(T));
		#endif

					// Step 1: compute the correct address of the RefCounted subobject
		   // by taking a dummy T* and casting it to RefCounted*
					uintptr_t baseAddr = reinterpret_cast<uintptr_t>(static_cast<T*>(mem));
					uintptr_t refAddr = reinterpret_cast<uintptr_t>(
						static_cast<RefCounted*>(reinterpret_cast<T*>(mem))
						);
					size_t refOffset = refAddr - baseAddr;

					// Step 2: get the correct RefCounted* inside this unconstructed memory
					RefCounted* ref = reinterpret_cast<RefCounted*>(
						reinterpret_cast<uint8_t*>(mem) + refOffset
						);

					// Step 3: preinitialize the count safely
					ref->m_StrongRefCount.store(1, std::memory_order_relaxed);
			RefUtils::AddToLiveReference(ref);

			Count<T> wrapper;  // empty; will take ownership without incrementing

			// 3) Placement-construct T in that memory
			T* obj = new (mem) T(std::forward<Args>(args)...);

			// 4) Attach to wrapper (do NOT AddStrongRef — it's already 1)
			wrapper.m_Ptr = obj;

			// 5) Return owning Count<T> (refcount stays at 1)
			return wrapper;

#endif
		}
		template<class Type, std::enable_if_t<Is_Compatible<Type, T>::value, int> = 0>
		inline Type* AsRaw()const
		{
			//usign stawtic instae of dynamic casue the template will not allow us to use a type not allwoed
			// static cast little faster than dynamic
			return static_cast<Type*>(m_Ptr);
		}
		static Count CreateFrom(const Count<T>& other) {
			if (!other)return nullptr;
			const T& copy = *other.m_Ptr;
			T* data = new T(copy);
			Count<T> t(data);
			return t;
		}

		static Count<T> CreateFrom(Count<T>&& other) {
			if (!other)return nullptr;
			T* data = new T(*other.m_Ptr);
			Count<T> t(data);
			return t;
		}

		uint32_t GetStrongCount()
		{
			if (m_Ptr)
				return m_Ptr->GetStrongCount();
			return 0;
		}
		uint32_t GetWeakCount()
		{
			if (m_Ptr)
				return m_Ptr->GetWeakCount();
			return 0;
		}
		template<class U>
		inline constexpr Count<U> As()const;

		template<class U>
		inline constexpr bool Is() const;

		size_t GetMemoryAddress() const
		{
			return size_t(Get());
		}
		size_t GetMemoryAddress() 
		{
			return size_t(Get());
		}
	private:
		T* m_Ptr = nullptr;

		template <class _Ty2>
		void MoveConstructor(Count<_Ty2>&& right) noexcept {
			// implement shared_ptr's (converting) move ctor and weak_ptr's move ctor
			m_Ptr = right.m_Ptr;
			right.m_Ptr = nullptr;
		}

		template <class _Ty2>
		void CopyConstructor(const Count<_Ty2>& other) noexcept {
			// implement shared_ptr's (converting) copy ctor
			other.AddStrongRef();

			m_Ptr = other.m_Ptr;
		}

		//template <class _Ty2>
		//void AliasConstructor(const Count<_Ty2>& other, element_type* px) noexcept {
		//	// implement shared_ptr's aliasing ctor
		//	other.AddStrongRef();
		//	m_Ptr = px;
		//}
		//
		//template <class _Ty2>
		//void AliasMoveConstructor(const Count<_Ty2>& other, element_type* px) noexcept {
		//	// implement shared_ptr's aliasing move ctor
		//	m_Ptr = px;
		//
		//	other.m_Ptr = nullptr;
		//}
		//template <class T = T, std::enable_if_t<std::is_base_of<RefCounted, T>::value, int> = 0 >

		void AddStrongRef()const {
			if (m_Ptr)
			{
				void* cast = m_Ptr;
				RefCounted* refCast = static_cast<RefCounted*>(cast);
				refCast->IncreaseStrongRef();
				// first count to hold onto it
				if (refCast->GetStrongCount() == 1)
					RefUtils::AddToLiveReference((void*)refCast);
				//m_Ptr->IncreaseStrongRef();
			}
		}

		void DecrementStrongRef()const { // decrement reference count

			if (m_Ptr)
			{
				void* cast = m_Ptr;
				RefCounted* refCast = static_cast<RefCounted*>(cast);
				if (refCast->GetStrongCount() == 1)
				{
					RefUtils::RemoveFromLiveReference((void*)refCast);
				}
				refCast->ReleaseStrongRef();
				
			}
		}

		void Swap(Count& _Right) noexcept { // swap pointers
			std::swap(m_Ptr, _Right.m_Ptr);
		}

		friend class Count;
	};
	template <class _Ty1, class _Ty2>
	_NODISCARD bool operator==(const Count<_Ty1>& _Left, const Count<_Ty2>& _Right) noexcept {
		return _Left.Get() == _Right.Get();
	}
	template <class _Ty>
	_NODISCARD bool operator==(const Count<_Ty>& _Left, nullptr_t) noexcept {
		return _Left.Get() == nullptr;
	}
	template <class _Ty1, class _Ty2>
	_NODISCARD Count<_Ty1> Dynamic_Count_cast(const Count<_Ty2>& _Other) noexcept {
		// dynamic_cast for shared_ptr that properly respects the reference count control block
		const auto _Ptr = dynamic_cast<typename Count<_Ty1>::element_type*>(_Other.Get());

		if (_Ptr)
		{
			//return Count<_Ty1>(_Other, _Ptr);
			return Count<_Ty1>(_Ptr);
		}

		return {};
	}

	template <class _Ty1, class _Ty2>
	_NODISCARD Count<_Ty1> Dynamic_Count_cast(Count<_Ty2>&& _Other) noexcept {
		// dynamic_cast for shared_ptr that properly respects the reference count control block
		const auto _Ptr = dynamic_cast<typename Count<_Ty1>::element_type*>(_Other.Get());

		if (_Ptr)
		{
			return Count<_Ty1>(std::move(_Other), _Ptr);
		}

		return {};
	}
	template<typename T>
	template <class U>
	inline constexpr Count<U> Count<T>::As()const
	{
		return Dynamic_Count_cast<U>(*this);
	}

	template<typename T>
	template<class U>
	inline constexpr bool Count<T>::Is() const
	{
		return As<U> != nullptr;
	}

	template<class T>
	class WeakCount
	{
	public:
		WeakCount() :m_Instance(nullptr) {};

		constexpr WeakCount(nullptr_t) noexcept : m_Instance(nullptr) {}

		template <class T = T, std::enable_if_t<std::is_base_of<RefCounted, T>::value, int> = 0 >
		WeakCount(T* instance)
			: m_Instance(instance)
		{
		}

		WeakCount(const WeakCount& _Other) noexcept {
			this->CopyConstructor(_Other); // same type, no conversion
		}

		template <class _Ty2, std::enable_if_t<std::is_base_of<RefCounted, T>::value, int> = 0>
		WeakCount(const Count<_Ty2>& _Other) noexcept {
			this->CopyConstructor(_Other.Get()); // shared_ptr keeps resource alive during conversion
		}

		template <class _Ty2, std::enable_if_t<std::is_base_of<RefCounted, T>::value, int> = 0>
		WeakCount(const WeakCount<_Ty2>& _Other) noexcept {
			this->CopyConstructor(_Other); // same type, no conversion
		}

		WeakCount(WeakCount&& _Other) noexcept {
			this->CopyConstructor(_STD move(_Other));
		}

		template <class _Ty2, std::enable_if_t<std::is_base_of<RefCounted, T>::value, int> = 0>
		WeakCount(WeakCount<_Ty2>&& _Other) noexcept {
			this->CopyConstructor(_STD move(_Other));
		}

		WeakCount& operator=(const WeakCount& _Right) noexcept {
			WeakCount(_Right).Swap(*this);
			return *this;
		}

		template <class Type, std::enable_if_t<Is_Compatible<Type, T>::value, int> = 0>
		WeakCount& operator=(const WeakCount<Type>& _Right) noexcept {
			WeakCount(_Right).Swap(*this);
			return *this;
		}

		WeakCount& operator=(WeakCount&& _Right) noexcept {
			WeakCount(_STD move(_Right)).Swap(*this);
			return *this;
		}

		template <class Type, std::enable_if_t<Is_Compatible<Type, T>::value, int> = 0>
		WeakCount& operator=(WeakCount<Type>&& _Right) noexcept {
			WeakCount(_STD move(_Right)).Swap(*this);
			return *this;
		}

		bool IsValid()const { return m_Instance != nullptr ? RefUtils::IsLive((void*)m_Instance) : false; }
		operator bool()const { return IsValid(); };

		Count<T> Lock() const { // convert to shared_ptr
			if (!IsValid())
				return nullptr;
			return Count<T>(m_Instance);
		}
	private:
		template <class _Ty2>
		void CopyConstructor(const WeakCount<_Ty2>& other) noexcept {
			m_Instance = other.m_Instance;
		}
		void Swap(WeakCount& _Right) noexcept { // swap pointers
			std::swap(m_Instance, _Right.m_Instance);
		}
		friend class WeakCount;

		T* m_Instance = nullptr;
	};



}

namespace std {
	template<typename T>
	struct hash<Proof::Count<T>> 
	{
		std::size_t operator()(const Proof::Count<T>& ptr) const noexcept
		{
			if(ptr == nullptr)
				return std::hash<size_t>()(size_t(0));
			else
				return std::hash<size_t>()(ptr.GetMemoryAddress());
		}
	};

	template<typename T>
	struct hash<Proof::WeakCount<T>> 
	{
		std::size_t operator()(const Proof::WeakCount<T>& ptr) const noexcept 
		{
			if (ptr.IsValid())
				return std::hash<size_t>()(ptr.Lock().GetMemoryAddress());
			else
				return std::hash<size_t>()(size_t(0));
		}
	};
}
