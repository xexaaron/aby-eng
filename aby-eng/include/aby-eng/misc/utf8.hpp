#pragma once

#include "common.hpp"

#include <ranges>
#include <span>
#include <string>

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
		auto operator--() -> CodepointIterator&;
		auto operator!=(const CodepointIterator& other) const -> bool;

		operator const char*();
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
		auto byte_offset(size_t idx) const -> size_t;

		auto operator[](size_t idx) -> char32_t;
		auto operator[](size_t idx) const -> char32_t;
	private:
		std::string_view m_String;
	};

	/**
    * @brief Convienence function to iterate codepoints
    * @param string The string to get the codepoints from
    * @return Codepoints(string)
    */
	auto codepoints(std::string_view string) -> Codepoints;
	/**
	* @brief encode a codepoint into a string
	* @param cp utf8 codepoint
	* @return encoded std::string
	*/
	auto encode(codepoint cp) -> std::string;
	/**
	* @brief encode a list of codepoints into a string
	* @param cps utf8 codepoints
	* @return encoded std::string
	*/
	auto encode(std::span<const codepoint> cps) -> std::string;
	/**
	* @brief encode a codepoint into a string
	* @param str string to append to
	* @param cp utf8 codepoint
	*/
	auto encode(std::string& str, codepoint cp) -> void;
	/**
	* @brief encode a list of codepoints into a string
	* @param str string to append to
	* @param cps utf8 codepoints
	*/
	auto encode(std::string& str, std::span<const codepoint> cps) -> void;

	// TODO: Make the api implement to_lower and to_upper for non-ascii characters

	/**
	* @brief Get the lowercase version of an ascii character that is utf8 encoded
	* @return lowercase utf8::codepoint
	*/
	auto to_lower(utf8::codepoint cp) -> utf8::codepoint;
	/**
	* @brief Get the uppercase version of an ascii character that is utf8 encoded
	* @return uppercase utf8::codepoint
	*/
	auto to_upper(utf8::codepoint cp) -> utf8::codepoint;

} // namespace aby::eng::utf8
