#pragma once
#include <cstddef>
#include <cstdint>
#include <memory>

#pragma once

#if ABY_BUILD_DLL == 1
#	define ABY_API __declspec(dllexport)
#else
#	define ABY_API __declspec(dllimport)
#endif

#if ABY_ENG_ENABLE_ASSERT == 1
#	if defined(_MSC_VER)
#		define ABY_ENG_DEBUG_BREAK() __debugbreak()
#	elif defined(__clang__) || defined(__GNUC__)
#		if defined(_WIN32)
#			define ABY_ENG_DEBUG_BREAK() __builtin_debugtrap()
#		elif defined(__i386__) || defined(__x86_64__)
#			define ABY_ENG_DEBUG_BREAK() __asm__ __volatile__("int3")
#		elif defined(__aarch64__) || defined(__arm__)
#			define ABY_ENG_DEBUG_BREAK() __builtin_trap()
#		else
#			define ABY_ENG_DEBUG_BREAK() __builtin_trap()
#		endif
#	else
#		include <cstdlib>
#		define ABY_ENG_DEBUG_BREAK() std::exit(2)
#	endif
#else
#	define ABY_ENG_DEBUG_BREAK()
#endif

namespace aby::eng {

	using u8  = uint8_t;
	using u16 = uint16_t;
	using u32 = uint32_t;
	using u64 = uint64_t;
	using i8  = int8_t;
	using i16 = int16_t;
	using i32 = int32_t;
	using i64 = int64_t;

	using uchar = unsigned char;
	using usize = size_t;
	using uptr  = uintptr_t;
	using iptr  = intptr_t;
	using byte  = uchar;

	template <typename T>
	using ref = std::shared_ptr<T>;

	template <typename T>
	using unique = std::unique_ptr<T>;

	template <typename T>
	using weak = std::weak_ptr<T>;

} // namespace aby::eng
