#pragma once

#include "common.hpp"

#include <ranges>

namespace aby::eng::utf8 {

	using codepoint = char32_t;

	class CodepointIterator {
	public:
		using difference_type   = std::ptrdiff_t;
		using value_type        = codepoint; // The underlying type being pointed to
		using pointer           = codepoint*;
		using reference         = codepoint&;
		using iterator_category = std::forward_iterator_tag;
		using iterator_concept  = std::forward_iterator_tag;
	public:
		CodepointIterator(const char* ptr, const char* end);

		auto operator*() const -> codepoint;
		auto operator++() -> CodepointIterator&;
		auto operator!=(const CodepointIterator& other) const -> bool;
	private:
		const char* m_Ptr;
		const char* m_End;
	};

	class Codepoints {
	public:
		explicit Codepoints(std::string_view string);
		auto size() const -> size_t;
		auto begin() const -> CodepointIterator;
		auto end() const -> CodepointIterator;
	private:
		std::string_view m_String;
	};

	/**
    * @brief Convienence function to iterate codepoints
    * @param string The string to get the codepoints from
    * @return Codepoints(string)
    */
	auto codepoints(std::string_view string) -> Codepoints;

} // namespace aby::eng::utf8
