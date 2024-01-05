/*
	SPDX-License-Identifier: MIT
	Copyright © 2023 Amebis
*/

#pragma once

#include "common.hpp"
#include <mach-o/dyld.h>
#include <string>

///
/// Copies the path of the main executable into the string path.
///
/// \sa [_NSGetExecutablePath function](https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/dyld.3.html)
///
inline int _NSGetExecutablePath(std::string& path)
{
	char stack_buffer[MACSTD_STACK_BUFFER_BYTES];
	uint32_t capacity = MACSTD_STACK_BUFFER_BYTES;
	int result = _NSGetExecutablePath(stack_buffer, &capacity);
	if (result == 0) {
		path = stack_buffer;
		return 0;
	}
	if (result == -1) {
		path.resize(capacity - 1);
		result = _NSGetExecutablePath(&path[0], &capacity);
		if (result == 0) {
			path.resize(strnlen(path.data(), path.size()));
			return 0;
		}
	}
	return result;
}
