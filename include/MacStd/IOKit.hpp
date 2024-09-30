/*
	SPDX-License-Identifier: MIT
	Copyright © 2024 Amebis
*/

#pragma once

#include "common.hpp"
#import <IOKit/IOKitLib.h>

namespace macstd {
	///
	/// IOKit handle wrapper class
	///
	template <typename T>
	class io_object : public handle<T, 0>
	{
		MACSTD_HANDLE_IMPL(io_object, T, 0)

	public:
		///
		/// Releases an object handle
		///
		/// \sa [IOObjectRelease function](https://developer.apple.com/documentation/iokit/1514627-ioobjectrelease)
		///
		virtual ~io_object()
		{
			if (this->m_h != 0)
				free_internal();
		}

	protected:
		///
		/// Releases an object handle
		///
		/// \sa [IOObjectRelease function](https://developer.apple.com/documentation/iokit/1514627-ioobjectrelease)
		///
		void free_internal() noexcept override
		{
			IOObjectRelease(this->m_h);
		}
	};

	using io_connect = io_object<io_connect_t>;
	using io_enumerator = io_object<io_enumerator_t>;
	using io_ident = io_object<io_ident_t>;
	using io_iterator = io_object<io_iterator_t>;
	using io_registry_entry = io_object<io_registry_entry_t>;
	using io_service = io_object<io_service_t>;
	using uext_object = io_object<uext_object_t>;
}

