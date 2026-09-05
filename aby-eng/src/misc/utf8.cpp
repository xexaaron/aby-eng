#include "misc/utf8.hpp"

#include <iterator>

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

	auto CodepointIterator::operator!=(const CodepointIterator& other) const -> bool {
		return m_Ptr != other.m_Ptr;
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

} // namespace aby::eng::utf8

namespace aby::eng::utf8 {

	auto codepoints(std::string_view string) -> Codepoints {
		return Codepoints(string);
	}

} // namespace aby::eng::utf8
