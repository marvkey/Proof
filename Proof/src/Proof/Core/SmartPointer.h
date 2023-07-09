#pragma once
#include <atomic>
#include <compare>
namespace Proof{
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

		void DecreaseStrongRef() noexcept { // decrement reference count
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
			this->DecreaseStrongRef();
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

}
