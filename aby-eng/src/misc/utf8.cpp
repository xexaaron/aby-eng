#include "misc/utf8.hpp"

#include "log.hpp"

#include <iterator>
#include <vector>

namespace aby::eng::utf8 {

	CodepointIterator::CodepointIterator(const char* ptr, const char* end) : m_Ptr(ptr), m_End(end) {
	}

	auto CodepointIterator::operator*() const -> codepoint {
		const auto* p = reinterpret_cast<const unsigned char*>(m_Ptr);

		if (p[0] < 0x80) {
			return p[0];
		}

		if ((p[0] & 0xE0) == 0xC0) {
			return (static_cast<codepoint>(p[0] & 0x1F) << 6) |
			       static_cast<codepoint>(p[1] & 0x3F);
		}

		if ((p[0] & 0xF0) == 0xE0) {
			return (static_cast<codepoint>(p[0] & 0x0F) << 12) |
			       (static_cast<codepoint>(p[1] & 0x3F) << 6) |
			       static_cast<codepoint>(p[2] & 0x3F);
		}

		if ((p[0] & 0xF8) == 0xF0) {
			return (static_cast<codepoint>(p[0] & 0x07) << 18) |
			       (static_cast<codepoint>(p[1] & 0x3F) << 12) |
			       (static_cast<codepoint>(p[2] & 0x3F) << 6) |
			       static_cast<codepoint>(p[3] & 0x3F);
		}

		// Invalid UTF-8.
		return 0xFFFD;
	}

	auto CodepointIterator::operator++() -> CodepointIterator& {
		const auto* p = reinterpret_cast<const unsigned char*>(m_Ptr);

		size_t length = 1;

		if ((p[0] & 0x80) == 0) {
			length = 1;
		} else if ((p[0] & 0xE0) == 0xC0) {
			length = 2;
		} else if ((p[0] & 0xF0) == 0xE0) {
			length = 3;
		} else if ((p[0] & 0xF8) == 0xF0) {
			length = 4;
		}

		m_Ptr += std::min(length, static_cast<size_t>(m_End - m_Ptr));
		return *this;
	}

	auto CodepointIterator::operator--() -> CodepointIterator& {
		auto* p = reinterpret_cast<const unsigned char*>(m_Ptr);

		do {
			--p;
		} while ((*p & 0xC0) == 0x80);

		m_Ptr = reinterpret_cast<const char*>(p);
		return *this;
	}

	auto CodepointIterator::operator!=(const CodepointIterator& other) const -> bool {
		return m_Ptr != other.m_Ptr;
	}

	CodepointIterator::operator const char*() {
		return m_Ptr;
	}

} // namespace aby::eng::utf8

namespace aby::eng::utf8 {

	Codepoints::Codepoints(std::string_view string) : m_String(string) {
	}

	auto Codepoints::size() const -> size_t {
		return std::distance(begin(), end());
	}

	auto Codepoints::begin() const -> CodepointIterator {
		return CodepointIterator(m_String.data(), m_String.data() + m_String.size());
	}

	auto Codepoints::end() const -> CodepointIterator {
		return CodepointIterator(m_String.data() + m_String.size(), m_String.data() + m_String.size());
	}

	auto Codepoints::byte_offset(size_t idx) const -> size_t {
		expect(idx <= size(), "codepoint index out of bounds");

		auto it = begin();
		std::advance(it, idx);

		return static_cast<size_t>(it - m_String.data());
	}

	auto Codepoints::operator[](size_t idx) -> char32_t {
		auto it = begin();
		std::advance(it, idx);
		return *it;
	}

	auto Codepoints::operator[](size_t idx) const -> char32_t {
		auto it = begin();
		std::advance(it, idx);
		return *it;
	}

} // namespace aby::eng::utf8

namespace aby::eng::utf8 {

	auto codepoints(std::string_view string) -> Codepoints {
		return Codepoints(string);
	}

	auto encode(codepoint cp) -> std::string {
		std::string str;
		encode(str, cp);
		return str;
	}

	auto encode(std::span<const codepoint> cps) -> std::string {
		std::string str;
		encode(str, cps);
		return str;
	}

	auto encode(std::string& str, codepoint cp) -> void {
		encode(str, std::span<const codepoint>(&cp, 1));
	}

	auto encode(std::string& str, std::span<const codepoint> cps) -> void {
		for (auto cp : cps) {
			if (cp <= 0x7F) {
				str.push_back(static_cast<char>(cp));
			} else if (cp <= 0x7FF) {
				str.push_back(static_cast<char>(0xC0 | (cp >> 6)));
				str.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
			} else if (cp <= 0xFFFF) {
				str.push_back(static_cast<char>(0xE0 | (cp >> 12)));
				str.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
				str.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
			} else {
				str.push_back(static_cast<char>(0xF0 | (cp >> 18)));
				str.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
				str.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
				str.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
			}
		}
	}

	auto to_lower(utf8::codepoint cp) -> utf8::codepoint {
		if (cp >= U'A' && cp <= U'Z')
			return cp + (U'a' - U'A');
		return cp;
	}

	auto to_upper(utf8::codepoint cp) -> utf8::codepoint {
		if (cp >= U'a' && cp <= U'z')
			return cp - (U'a' - U'A');
		return cp;
	}

} // namespace aby::eng::utf8
