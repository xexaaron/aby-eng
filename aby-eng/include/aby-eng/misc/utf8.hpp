#pragma once

#include "common.hpp"

#include <compare>
#include <format>
#include <limits>
#include <ranges>
#include <span>
#include <string>

namespace aby::eng::utf8 {

	/// @brief A 32 bit utf-8 codepoint class to 'ensure' operations from the utf8 namespace are used
	///		   on codepoints instead of the standard library functions
	enum class codepoint : char32_t {
		// sentinels
		min         = 0,          // lowest possible value
		null        = 0,          // null terminator
		max         = 0x10FFFF,   // highest possible value
		invalid     = 0xFFFFFFFF, // utf8 codepoints cannot go beyond max
		// special codepoints
		undefined   = 0xFFFF, // non-character
		replacement = 0xFFFD, // corrupted, unrecgonized, or invalid text data
		bom         = 0xFEFF, // byte order mark (zero-width no-break space)
	};

	class CodepointIterator {
	public:
		using difference_type   = std::ptrdiff_t;
		using value_type        = codepoint;
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

		auto operator[](size_t idx) -> codepoint;
		auto operator[](size_t idx) const -> codepoint;
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

namespace aby::eng::utf8 {

	constexpr auto operator<=>(utf8::codepoint cp, char32_t c) -> std::strong_ordering {
		return std::to_underlying(cp) <=> c;
	}

	constexpr auto operator<=>(char32_t c, utf8::codepoint cp) -> std::strong_ordering {
		return c <=> std::to_underlying(cp);
	}

	constexpr auto operator==(utf8::codepoint cp, char32_t c) -> bool {
		return std::to_underlying(cp) == c;
	}

	constexpr auto operator==(char32_t c, utf8::codepoint cp) -> bool {
		return cp == c;
	}

} // namespace aby::eng::utf8

namespace std {

	template <>
	struct formatter<aby::eng::utf8::codepoint, char> {
		template <class ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& ctx) {
			auto it = ctx.begin();
			if (it != ctx.end() && *it != '}')
				throw format_error("invalid format");
			return it;
		}

		template <class FmtContext>
		FmtContext::iterator format(const aby::eng::utf8::codepoint& cp, FmtContext& ctx) const {
			switch (cp) {
				case aby::eng::utf8::codepoint::max:
					return format_to(ctx.out(), "<U+{}>", "max");
				case aby::eng::utf8::codepoint::null:
					return format_to(ctx.out(), "<U+{}>", "null-terminator");
				case aby::eng::utf8::codepoint::invalid:
					return format_to(ctx.out(), "<U+{}>", "invalid");
				case aby::eng::utf8::codepoint::undefined:
					return format_to(ctx.out(), "<U+{}>", "non-character");
				case aby::eng::utf8::codepoint::replacement:
					return format_to(ctx.out(), "<U+{}>", "replacement");
				case aby::eng::utf8::codepoint::bom:
					return format_to(ctx.out(), "<U+{}>", "BOM");
				default:
					return format_to(ctx.out(), "U+{:04X}", std::to_underlying(cp));
			}
			return format_to(ctx.out(), "U+{:04X}", std::to_underlying(cp));
		}
	};

} // namespace std
