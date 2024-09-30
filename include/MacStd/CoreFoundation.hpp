/*
	SPDX-License-Identifier: MIT
	Copyright © 2023-2024 Amebis
*/

#pragma once

#include "common.hpp"
#include <CoreFoundation/CoreFoundation.h>
#include <memory>

namespace macstd {
	///
	/// Deleter for unique_ptr using CFRelease
	///
	struct CFRelease_delete
	{
		///
		/// Default constructor
		///
		CFRelease_delete() noexcept {}

		///
		/// Delete a pointer
		///
		/// \sa [CFRelease function](https://developer.apple.com/documentation/corefoundation/1521153-cfrelease)
		///
		template <class _T>
		void operator()(_T *_Ptr) const
		{
			CFRelease(_Ptr);
		}
	};

	///
	/// CFType helper
	///
	/// \sa [CFType](https://developer.apple.com/documentation/corefoundation/cftype)
	template <class T>
	using CFType = std::unique_ptr<T, CFRelease_delete>;
}
