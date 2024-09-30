/*
	SPDX-License-Identifier: MIT
	Copyright © 2023-2024 Amebis
*/

#pragma once

#include <stddef.h>

///
/// Declares a class as non-copyable
///
#define MACSTD_NONCOPYABLE(C) \
private: \
	   C        (_In_ const C &h) noexcept; \
	C& operator=(_In_ const C &h) noexcept;

///
/// Declares a class as non-movable
///
#define MACSTD_NONMOVABLE(C) \
private: \
	   C        (_Inout_ C &&h) noexcept; \
	C& operator=(_Inout_ C &&h) noexcept;

///
/// Size of the stack buffer in bytes used for initial system function call
///
/// Some system functions with variable length output data fail for insufficient buffer sizes. The function helpers use a fixed size stack buffer first. If the stack buffer really prooved sufficient, the helper allocates the exact length output on heap and copies the data without calling the system function again. Otherwise it allocates the memory on heap and retries.
///
/// \note Decrease this value in case of stack overflow.
///
#define MACSTD_STACK_BUFFER_BYTES 1024

///
/// Implements default constructors and operators to prevent their auto-generation by compiler.
///
#define MACSTD_HANDLE_IMPL(C, T, INVAL) \
public: \
	   C        (              ) noexcept                                  {} \
	   C        (_In_opt_ T   h) noexcept : handle<T, INVAL>(          h ) {} \
	   C        (_Inout_  C &&h) noexcept : handle<T, INVAL>(std::move(h)) {} \
	C& operator=(_In_opt_ T   h) noexcept                                  { handle<T, INVAL>::operator=(          h ); return *this; } \
	C& operator=(_Inout_  C &&h) noexcept                                  { handle<T, INVAL>::operator=(std::move(h)); return *this; } \
MACSTD_NONCOPYABLE(C)

///
/// Implements default constructors and operators to prevent their auto-generation by compiler.
///
#define MACSTD_DPLHANDLE_IMPL(C, T, INVAL) \
public: \
	   C        (                    ) noexcept                                                  {} \
	   C        (_In_opt_       T   h) noexcept : dplhandle<T, INVAL>(                   h     ) {} \
	   C        (_In_     const C  &h) noexcept : dplhandle<T, INVAL>(duplicate_internal(h.m_h)) {} \
	   C        (_Inout_        C &&h) noexcept : dplhandle<T, INVAL>(std::move         (h    )) {} \
	C& operator=(_In_opt_       T   h) noexcept                                                  { dplhandle<T, INVAL>::operator=(          h ); return *this; } \
	C& operator=(_In_     const C  &h) noexcept                                                  { dplhandle<T, INVAL>::operator=(          h ); return *this; } \
	C& operator=(_Inout_        C &&h) noexcept                                                  { dplhandle<T, INVAL>::operator=(std::move(h)); return *this; } \
private:

namespace macstd
{
	///
	/// Base abstract template class to support generic object handle keeping
	///
	/// It provides basic operators and methods common to all descendands of this class establishing a base to ease the replacement of native object handle type with classes in object-oriented approach.
	///
	template <class T, const T INVAL>
	class handle
	{
	public:
		///
		/// Initializes a new class instance with the object handle set to INVAL.
		///
		handle() noexcept : m_h(INVAL)
		{}

		///
		/// Initializes a new class instance with an already available object handle.
		///
		/// \param[in] h  Initial object handle value
		///
		handle(_In_opt_ T h) noexcept : m_h(h)
		{}

		///
		/// Move constructor
		///
		/// \param[in,out] h  A rvalue reference of another object
		///
		handle(_Inout_ handle<T, INVAL> &&h) noexcept
		{
			// Transfer handle.
			m_h   = h.m_h;
			h.m_h = INVAL;
		}

	private:
		// This class is noncopyable.
		handle(_In_ const handle<T, INVAL> &h) noexcept {};
		handle<T, INVAL>& operator=(_In_ const handle<T, INVAL> &h) noexcept {};

	public:
		///
		/// Attaches already available object handle.
		///
		/// \param[in] h  Object handle value
		///
		handle<T, INVAL>& operator=(_In_opt_ T h) noexcept
		{
			attach(h);
			return *this;
		}

		///
		/// Move assignment
		///
		/// \param[inout] h  A rvalue reference of another object
		///
		#pragma warning(suppress: 26432) // Move constructor is also present, but not detected by code analysis somehow.
		handle<T, INVAL>& operator=(_Inout_ handle<T, INVAL> &&h) noexcept
		{
			if (this != std::addressof(h)) {
				// Transfer handle.
				if (m_h != INVAL)
					free_internal();
				m_h   = h.m_h;
				h.m_h = INVAL;
			}
			return *this;
		}

		///
		/// Auto-typecasting operator
		///
		/// \return Object handle
		///
		operator T() const
		{
			return m_h;
		}

		///
		/// Returns the object handle value when the object handle is a pointer to a value (class, struct, etc.).
		///
		/// \return Object handle value
		///
		T*& operator*() const
		{
			assert(m_h != INVAL);
			return *m_h;
		}

		///
		/// Returns the object handle reference.
		/// \return Object handle reference
		///
		T* operator&()
		{
			assert(m_h == INVAL);
			return &m_h;
		}

		///
		/// Provides object handle member access when the object handle is a pointer to a class or struct.
		///
		/// \return Object handle
		///
		T operator->() const
		{
			assert(m_h != INVAL);
			return m_h;
		}

		///
		/// Tests if the object handle is invalid.
		///
		/// \return
		/// - Non zero when object handle is invalid;
		/// - Zero otherwise.
		///
		/// \note Implementing `operator bool() const` would be correct C++ approach here. Unfortunately, it would produce ambiguities where `bool` and `T` are polymorphed.
		/// Use `!!` to test if the object handle is valid.
		///
		bool operator!() const
		{
			return m_h == INVAL;
		}

		///
		/// Is handle less than?
		///
		/// \param[in] h  Object handle to compare against
		/// \return
		/// - Non zero when object handle is less than h;
		/// - Zero otherwise.
		///
		bool operator<(_In_opt_ T h) const
		{
			return m_h < h;
		}

		///
		/// Is handle less than or equal to?
		///
		/// \param[in] h  Object handle to compare against
		/// \return
		/// - Non zero when object handle is less than or equal to h;
		/// - Zero otherwise.
		///
		bool operator<=(_In_opt_ T h) const
		{
			return !operator>(h);
		}

		///
		/// Is handle greater than or equal to?
		///
		/// \param[in] h  Object handle to compare against
		/// \return
		/// - Non zero when object handle is greater than or equal to h;
		/// - Zero otherwise.
		///
		bool operator>=(_In_opt_ T h) const
		{
			return !operator<(h);
		}

		///
		/// Is handle greater than?
		///
		/// \param[in] h  Object handle to compare against
		/// \return
		/// - Non zero when object handle is greater than h;
		/// - Zero otherwise.
		///
		bool operator>(_In_opt_ T h) const
		{
			return h < m_h;
		}

		///
		/// Is handle not equal to?
		///
		/// \param[in] h  Object handle to compare against
		/// \return
		/// - Non zero when object handle is not equal to h;
		/// - Zero otherwise.
		///
		bool operator!=(_In_opt_ T h) const
		{
			return !operator==(h);
		}

		///
		/// Is handle equal to?
		///
		/// \param[in] h  Object handle to compare against
		/// \return
		/// - Non zero when object handle is equal to h;
		/// - Zero otherwise.
		///
		bool operator==(_In_opt_ T h) const
		{
			return m_h == h;
		}

		///
		/// Sets a new object handle for the class
		///
		/// When the current object handle of the class is non-INVAL, the object is destroyed first.
		///
		/// \param[in] h  New object handle
		///
		void attach(_In_opt_ T h) noexcept
		{
			if (m_h != INVAL)
				free_internal();
			m_h = h;
		}

		///
		/// Dismisses the object handle from this class
		///
		/// \return Object handle
		///
		T detach()
		{
			T h = m_h;
			m_h = INVAL;
			return h;
		}

		///
		/// Destroys the object
		///
		void free()
		{
			if (m_h != INVAL) {
				free_internal();
				m_h = INVAL;
			}
		}

	protected:
		///
		/// Abstract member function that must be implemented by child classes to do the actual object destruction.
		///
		virtual void free_internal() noexcept = 0;

	protected:
		T m_h; ///< Object handle
	};

	///
	/// Base abstract template class to support object handle keeping for objects that support trivial handle duplication
	///
	template <class T, T INVAL>
	class dplhandle : public handle<T, INVAL>
	{
	public:
		///
		/// Initializes a new class instance with the object handle set to INVAL.
		///
		dplhandle() noexcept
		{}

		///
		/// Initializes a new class instance with an already available object handle.
		///
		/// \param[in] h  Initial object handle value
		///
		dplhandle(_In_opt_ T h) noexcept : handle<T, INVAL>(h)
		{}

		///
		/// Copy constructor
		///
		/// \param[in,out] h  A reference of another object
		///
		dplhandle<T, INVAL>(_In_ const dplhandle<T, INVAL> &h) : handle<T, INVAL>(duplicate_internal(h.m_h))
		{}

		///
		/// Move constructor
		///
		/// \param[in,out] h  A rvalue reference of another object
		///
		dplhandle<T, INVAL>(_Inout_ dplhandle<T, INVAL> &&h) noexcept : handle<T, INVAL>(std::move(h))
		{}

		///
		/// Attaches already available object handle.
		///
		/// \param[in] h  Object handle value
		///
		dplhandle<T, INVAL>& operator=(_In_opt_ T h) noexcept
		{
			handle<T, INVAL>::operator=(h);
			return *this;
		}

		///
		/// Duplicates the object.
		///
		/// \param[in] h  Object
		///
		dplhandle<T, INVAL>& operator=(_In_ const dplhandle<T, INVAL> &h) noexcept
		{
			if (this != std::addressof(h)) {
				if (h.m_h != INVAL) {
					T h_new = duplicate_internal(h.m_h);

					if (this->m_h != INVAL)
						this->free_internal();

					this->m_h = h_new;
				} else {
					if (this->m_h != INVAL)
						this->free_internal();

					this->m_h = INVAL;
				}
			}
			return *this;
		}

		///
		/// Moves the object.
		///
		/// \param[inout] h  A rvalue reference of another object
		///
		#pragma warning(disable: 26432) // Move constructor is also present, but not detected by code analysis somehow.
		dplhandle<T, INVAL>& operator=(_Inout_ dplhandle<T, INVAL> &&h) noexcept
		{
			handle<T, INVAL>::operator=(std::move(h));
			return *this;
		}

		///
		/// Duplicates and returns a new object handle.
		///
		/// \return Duplicated object handle
		///
		T duplicate() const
		{
			return this->m_h != INVAL ? duplicate_internal(this->m_h) : INVAL;
		}

		///
		/// Duplicates an object handle and sets a new object handle.
		///
		/// \param[in] h  Object handle of existing object
		///
		void attach_duplicated(_In_opt_ T h)
		{
			if (this->m_h != INVAL)
				this->free_internal();

			this->m_h = h != INVAL ? duplicate_internal(h) : INVAL;
		}

	protected:
		///
		/// Abstract member function that must be implemented by child classes to do the actual object handle duplication.
		/// On failure, it should throw appropriate exception describing the cause, rather than return an invalid handle.
		///
		/// \param[in] h  Object handle of existing object
		///
		/// \return Duplicated object handle
		///
		virtual T duplicate_internal(_In_ T h) const = 0;
	};
}
