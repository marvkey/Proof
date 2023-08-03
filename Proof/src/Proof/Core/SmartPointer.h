#pragma once
#include <unordered_set>
#include <atomic>
#include <compare>
namespace Proof{


	class RefCounted {
	public:
		RefCounted() {}; // non-atomic initializations

		RefCounted(const RefCounted& other)
		{
		}
		RefCounted& operator=(const RefCounted& other)
		{
		}
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
			if (m_StrongRefCount == 0)
			{
				Delete();
			}
		}
		mutable std::atomic<uint32_t>  m_StrongRefCount = 0;
		template <class T>
		friend class Count;
	};
	namespace RefUtils {

		class RefCountManager
		{
		public:
			static void AddToLiveReference(void* instance)
			{
				uintptr_t memoryAddress = uintptr_t(instance);
				s_LiveReferences.insert(memoryAddress);
			};
			static void RemoveFromLiveReference(void* instance)
			{
				uintptr_t memoryAddress = uintptr_t(instance);
				s_LiveReferences.erase(memoryAddress);
			}
			static bool IsLive(void* instance)
			{
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
		constexpr Count() noexcept : m_Ptr (nullptr)
		{
		}

		constexpr Count(nullptr_t) noexcept : m_Ptr(nullptr)
		{ 
		} 

		template <class T = T, std::enable_if_t<std::is_base_of<RefCounted, T>::value, int> = 0 >
		Count(T* data) {
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
		static Count Create(Args&&... args) {
			T* data = new T(args...);
			Count<T> t(data);
			return t;
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
		template<class U, std::enable_if_t<Is_Compatible<U, T>::value, int> = 0>
		inline constexpr Count<U> As()const;

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
				if(refCast->GetStrongCount() ==1)
					RefUtils::AddToLiveReference((void*)refCast);
				//m_Ptr->IncreaseStrongRef();
			}
		}

		void DecrementStrongRef()const { // decrement reference count
			if (m_Ptr)
			{
				void* cast = m_Ptr;
				RefCounted* refCast = static_cast<RefCounted*>(cast);
				bool remoeMemory = false;
				// means its goig to get released from memoery
				if (refCast->GetStrongCount() == 1)
				{
					RefUtils::RemoveFromLiveReference((void*)refCast);
				}

				refCast->ReleaseStrongRef();
				//m_Ptr->ReleaseStrongRef();
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
	template <class U, std::enable_if_t<Is_Compatible<U, T>::value, int>>
	inline constexpr Count<U> Count<T>::As()const
	{
		return Dynamic_Count_cast<U>(*this);
	}

	template<class T>
	class WeakCount
	{
	public:
		WeakCount() :m_Instance(nullptr) {	};

		constexpr WeakCount(nullptr_t) noexcept : m_Instance(nullptr) {	}

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

		bool IsValid()const { return m_Instance ? RefUtils::IsLive((void*)m_Instance) : false; }
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
	#if 0 
	/**
 * a lot of this code was taken from the visual studio representiaon of shared poitner
 * it has been manuactured for a better api adn multithreading
 */
	// Template function to check if a class has implemented all virtual functions
	template <typename T, typename = void>
	struct HasImplementedAllVirtuals : std::false_type {};

	template <typename T>
	struct HasImplementedAllVirtuals<T, std::void_t<decltype(std::declval<T>().foo()), decltype(std::declval<T>().bar())>> : std::true_type {};


	class RefCounterBase {
	public:
		RefCounterBase(const RefCounterBase&) = delete;
		RefCounterBase& operator=(const RefCounterBase&) = delete;
		virtual ~RefCounterBase() {}

		void AddStrongRef() {
			m_StrongRefCounter++;
		}
		void RemoveStrongRef() {
			m_StrongRefCounter--;
			if (m_StrongRefCounter == 0)
			{
				DestroyPointer();
				DestroySelf();
			}
		}
		uint32_t GetStrongCount() {
			return m_StrongRefCounter;
		}
	protected:
		constexpr RefCounterBase() noexcept = default; // non-atomic initializations
	private:
		virtual void DestroyPointer() = 0; // destroy managed resource
		virtual void DestroySelf() = 0; // destroy self
		std::atomic<uint32_t>  m_StrongRefCounter = 1;
	};
	template <class T>
	class RefCount : public RefCounterBase { // handle reference counting for pointer without deleter
	public:
		explicit RefCount(T* _Px) : RefCounterBase(), m_Ptr(_Px) {}

	private:
		void DestroySelf() { // destroy managed resource
			delete this;
		}

		void DestroyPointer() { // destroy self
			delete m_Ptr;
		}

		T* m_Ptr;
	};
	template <class _Ty>
	class Count;

	template <class _Ty>
	class PointerBase { // base class for shared_ptr and weak_ptr
	public:
		using element_type = std::remove_extent_t<_Ty>;

		_NODISCARD long GetStrongCount() const noexcept {
			return m_RefCount ? m_RefCount->GetStrongCount() : 0;
		}

		template <class _Ty2>
		_NODISCARD bool GetOwnerBefore(const PointerBase<_Ty2>& _Right) const noexcept { // compare addresses of manager objects
			return m_RefCount < _Right.m_RefCount;
		}

		PointerBase(const PointerBase&) = delete;
		PointerBase& operator=(const PointerBase&) = delete;

		_NODISCARD element_type* Get() const noexcept {
			return m_Ptr;
		}
	protected:

		constexpr PointerBase() noexcept = default;

		~PointerBase() = default;

		template <class _Ty2>
		void MoveConstructor(PointerBase<_Ty2>&& right) noexcept {
			// implement shared_ptr's (converting) move ctor and weak_ptr's move ctor
			m_Ptr = right.m_Ptr;
			m_RefCount = right.m_RefCount;

			right.m_Ptr = nullptr;
			right.m_RefCount = nullptr;
		}

		template <class _Ty2>
		void CopyConstructor(const PointerBase<_Ty2>& other) noexcept {
			// implement shared_ptr's (converting) copy ctor
			other.AddStrongRef();

			m_Ptr = other.m_Ptr;
			m_RefCount = other.m_RefCount;
		}

		template <class _Ty2>
		void AliasConstructor(const PointerBase<_Ty2>& other, element_type* px) noexcept {
			// implement shared_ptr's aliasing ctor
			other.AddStrongRef();
			m_Ptr = px;
			m_RefCount = other.m_RefCount;
		}

		template <class _Ty2>
		void AliasMoveConstructor(const PointerBase<_Ty2>& other, element_type* px) noexcept {
			// implement shared_ptr's aliasing move ctor
			m_Ptr = px;
			m_RefCount = other.m_RefCount;

			other.m_Ptr = nullptr;
			other.m_RefCount = nullptr;
		}
		/*

		template <class _Ty0>
		friend class weak_ptr; // specifically, weak_ptr::lock()
		template <class _Ty2>
		bool _Construct_from_weak(const weak_ptr<_Ty2>& _Other) noexcept {
			// implement shared_ptr's ctor from weak_ptr, and weak_ptr::lock()
			if (_Other._Rep && _Other._Rep->_Incref_nz())
			{
				_Ptr = _Other._Ptr;
				_Rep = _Other._Rep;
				return true;
			}

			return false;
		}
		*/
		void AddStrongRef() const noexcept {
			if (m_RefCount)
			{
				m_RefCount->AddStrongRef();
			}
		}

		void DecrementStrongRef() noexcept { // decrement reference count
			if (m_RefCount)
			{
				m_RefCount->RemoveStrongRef();
			}
		}

		void Swap(PointerBase& _Right) noexcept { // swap pointers
			std::swap(m_Ptr, _Right.m_Ptr);
			std::swap(m_RefCount, _Right.m_RefCount);
		}

		/*
		template <class _Ty2>
		void _Weakly_construct_from(const _Ptr_base<_Ty2>& _Other) noexcept { // implement weak_ptr's ctors
			if (_Other._Rep)
			{
				_Ptr = _Other._Ptr;
				_Rep = _Other._Rep;
				_Rep->_Incwref();
			}
			else
			{
				_STL_INTERNAL_CHECK(!_Ptr && !_Rep);
			}
		}

		template <class _Ty2>
		void _Weakly_convert_lvalue_avoiding_expired_conversions(const _Ptr_base<_Ty2>& _Other) noexcept {
			// implement weak_ptr's copy converting ctor
			if (_Other._Rep)
			{
				_Rep = _Other._Rep; // always share ownership
				_Rep->_Incwref();

				if (_Rep->_Incref_nz())
				{
					_Ptr = _Other._Ptr; // keep resource alive during conversion, handling virtual inheritance
					_Rep->_Decref();
				}
				else
				{
					_STL_INTERNAL_CHECK(!_Ptr);
				}
			}
			else
			{
				_STL_INTERNAL_CHECK(!_Ptr && !_Rep);
			}
		}

		template <class _Ty2>
		void _Weakly_convert_rvalue_avoiding_expired_conversions(_Ptr_base<_Ty2>&& _Other) noexcept {
			// implement weak_ptr's move converting ctor
			_Rep = _Other._Rep; // always transfer ownership
			_Other._Rep = nullptr;

			if (_Rep && _Rep->_Incref_nz())
			{
				_Ptr = _Other._Ptr; // keep resource alive during conversion, handling virtual inheritance
				_Rep->_Decref();
			}
			else
			{
				_STL_INTERNAL_CHECK(!_Ptr);
			}

			_Other._Ptr = nullptr;
		}
		*/
		/*
		void _Incwref() const noexcept {
			if (_Rep)
			{
				_Rep->_Incwref();
			}
		}

		void _Decwref() noexcept { // decrement weak reference count
			if (_Rep)
			{
				_Rep->_Decwref();
			}
		}
		*/
	private:
		friend class Count<_Ty>;


		template<class T>
		friend class PointerBase;
		element_type* m_Ptr{ nullptr };
		RefCounterBase* m_RefCount{ nullptr };
	};
	template <class _Yty, class _Ty>
	struct Is_Compatible : std::is_convertible<_Yty*, _Ty*>::type {
		// N4659 [util.smartptr.shared]/5 "a pointer type Y* is said to be compatible
		// with a pointer type T* " "when either Y* is convertible to T* ..."
	};
	template <typename T>
	class Count : public PointerBase<T>
	{
	private:
		using Mybase = PointerBase<T>;

	public:
		using typename Mybase::element_type;

		constexpr Count() noexcept = default;

		constexpr Count(nullptr_t) noexcept { } // construct empty shared_ptr

		explicit Count(T* data) {
			this->m_Ptr = data;
			this->m_RefCount = new RefCount<T>(data);
		}

		explicit Count(T* data, RefCounterBase* base) {
			this->m_Ptr = data;
			this->m_RefCount = base;
		}

		//template <class Type, std::enable_if_t<Is_Compatible<Type, T>::value, int> = 0>
		template<typename T>
		Count(const Count<T>& right, element_type* px) noexcept {
			// construct shared_ptr object that aliases _Right
			this->AliasConstructor(right, px);
		}

		template <class Type, std::enable_if_t<Is_Compatible<Type, T>::value, int> = 0>
		Count(Count<Type>&& right, element_type* px) noexcept {
			// construct shared_ptr object that aliases _Right
			this->AliasMoveConstructor(std::move(right), px);
		}

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

		template <class _Ty2 = T, std::enable_if_t<!std::disjunction_v<std::is_array<_Ty2>, std::is_void<_Ty2>>, int> = 0>
		_Ty2& operator*() const noexcept {
			return *this->Get();
		}
		template<typename  U>
		inline constexpr Count<U> As()const;
		explicit operator bool() const noexcept {
			return this->Get() != nullptr;
		}
		
		// an error can occure if teh class you are trying to create 
		// has a virutal function not implemented 
		// or hte arguments do not mathc the contrucor
		template <class... Args, std::enable_if_t<std::is_constructible<T, Args...>::value, int> = 0>
		static Count Create(Args&&... args) {
			T* data = new T(args...);
			Count<T> t(data);
			return t;
		} 
		
		static Count CreateFrom(const Count& other) {
			if (!other)return nullptr;
			const auto& copy = *other;
			T* data = new T(copy);
			Count<T> t(data);
			return t;
		}

		static Count CreateFrom(Count&& other) {
			if (!other)return nullptr;
			T* data = new T(*other);
			Count<T> t(data);
			return t;
		}
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
			return Count<_Ty1>(_Other, _Ptr);
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
	template<typename U>
	inline constexpr Count<U> Count<T>::As()const
	{
		return Dynamic_Count_cast<U>(*this);
	}
	#endif
}
