/*
	SPDX-License-Identifier: MIT
	Copyright © 2023 Amebis
*/

#pragma once

#include "common.hpp"
#include <unistd.h>
#include <string>

///
/// Copies the absolute pathname of the current working directory
///
/// \sa [getcwd function](https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/getcwd.3.html)
///
inline const char* getcwd(std::string& path)
{
	char stack_buffer[MACSTD_STACK_BUFFER_BYTES];
	auto result = getcwd(stack_buffer, MACSTD_STACK_BUFFER_BYTES);
	if (result) {
		path = result;
		return path.data();
	}
	if (errno == ERANGE) {
		result = getcwd(NULL, 0);
		if (result) {
			path = result;
			free(result);
			return path.data();
		}
	}
	return NULL;
}
