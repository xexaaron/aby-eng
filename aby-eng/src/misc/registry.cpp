#include "misc/registry.hpp"

#include <unordered_map>

namespace aby::eng {

	std::unordered_map<std::string_view, int64_t> s_RegistryValues;

	auto Registry::get(std::string_view str) -> int64_t {
		return s_RegistryValues[str];
	}

	auto Registry::set(std::string_view str, int64_t value) -> void {
		s_RegistryValues[str] = value;
	}

} // namespace aby::eng
