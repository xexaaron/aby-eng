#pragma once

#include "common.hpp"
#include "misc/font.hpp"
#include "misc/utf8.hpp"
#include "misc/uuid.hpp"

#include <string>

namespace aby::eng {

	/// @brief UTF-8 aware text class
	class ABY_API Text {
	public:
		Text(const std::string& data, FontPtr font);
		Text(std::span<utf8::codepoint> codepoints, FontPtr font);

		auto append(std::string_view str) -> void;
		auto append(const Text& other) -> void;
		auto append(char c, size_t amt = 1) -> void;
		auto append(utf8::codepoint cp, size_t amt = 1) -> void;
		auto prepend(std::string_view str) -> void;
		auto prepend(const Text& text) -> void;
		auto prepend(char c, size_t amt = 1) -> void;
		auto prepend(utf8::codepoint cp, size_t amt = 1) -> void;
		auto insert(size_t pos, std::string_view str) -> void;
		auto insert(size_t pos, const Text& text) -> void;
		auto insert(size_t pos, utf8::codepoint cp) -> void;
		auto trim_back(size_t amt = 1) -> void;
		auto trim_front(size_t amt = 1) -> void;
		auto to_lower() -> Text&;
		auto to_upper() -> Text&;
		auto transform(std::function<utf8::codepoint(utf8::codepoint)>&& proc) -> Text&;
		auto erase(size_t pos, size_t amt = 1) -> void;
		auto clear() -> void;

		auto contains(utf8::codepoint cp) const -> bool;
		auto contains(std::string_view str) const -> bool;
		auto contains(const Text& text) const -> bool;
		auto starts_with(std::string_view str) const -> bool;
		auto starts_with(utf8::codepoint cp) const -> bool;
		auto starts_with(const Text& text) const -> bool;
		auto ends_with(std::string_view str) const -> bool;
		auto ends_with(utf8::codepoint cp) const -> bool;
		auto ends_with(const Text& text) const -> bool;

		auto font() const -> FontPtr;
		auto data() const -> const std::string&;
		auto data() -> std::string&;
		auto bytes() const -> size_t;
		auto length() const -> size_t;
		auto size() const -> glm::fvec2;
		auto height() const -> float;
		auto width() const -> float;
		auto empty() const -> bool;
		auto view() const -> std::string_view;

		auto begin() -> utf8::CodepointIterator;
		auto end() -> utf8::CodepointIterator;
		auto begin() const -> utf8::CodepointIterator;
		auto end() const -> utf8::CodepointIterator;
		auto cbegin() const -> utf8::CodepointIterator;
		auto cend() const -> utf8::CodepointIterator;
		auto back() const -> utf8::codepoint;
		auto front() const -> utf8::codepoint;

		auto set(size_t idx, utf8::codepoint cp) -> void;
		auto set_font(FontPtr font) -> void;

		auto operator==(const Text& other) const -> bool;
		auto operator==(std::string_view other) const -> bool;
		auto operator<=>(const Text& other) const -> std::strong_ordering;
		auto operator<=>(std::string_view other) const -> std::strong_ordering;
		auto operator[](size_t idx) const -> utf8::codepoint;
		auto operator[](size_t idx) -> utf8::codepoint;
	private:
		FontPtr m_Font;
		std::string m_Data;
	};

} // namespace aby::eng

namespace std {

	template <>
	struct formatter<aby::eng::Text, char> {
		template <class ParseContext>
		constexpr auto parse(ParseContext& ctx) -> ParseContext::iterator {
			auto it = ctx.begin();

			if (it != ctx.end() && *it != '}')
				throw format_error("invalid format");

			return it;
		}

		template <class FormatContext>
		auto format(const aby::eng::Text& text, FormatContext& ctx) const -> FormatContext::iterator {
			return format_to(ctx.out(), "{}", text.view());
		}
	};

} // namespace std
