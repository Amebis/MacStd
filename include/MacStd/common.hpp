/*
	SPDX-License-Identifier: MIT
	Copyright © 2023 Amebis
*/

#pragma once

#include <stddef.h>

///
/// Size of the stack buffer in bytes used for initial system function call
///
/// Some system functions with variable length output data fail for insufficient buffer sizes. The function helpers use a fixed size stack buffer first. If the stack buffer really prooved sufficient, the helper allocates the exact length output on heap and copies the data without calling the system function again. Otherwise it allocates the memory on heap and retries.
///
/// \note Decrease this value in case of stack overflow.
///
#define MACSTD_STACK_BUFFER_BYTES 1024
