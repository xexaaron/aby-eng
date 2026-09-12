#include "core/text.hpp"

#include "log.hpp"
#include "misc/utf8.hpp"

namespace aby::eng {

	Text::Text(const std::string& data) :
	    m_Data(data) {
	}

	Text::Text(std::span<utf8::codepoint> codepoints) :
	    m_Data(utf8::encode(codepoints)) {
	}

	auto Text::data() const -> const std::string& {
		return m_Data;
	}

	auto Text::data() -> std::string& {
		return m_Data;
	}

	auto Text::append(std::string_view str) -> void {
		m_Data.append(str);
	}

	auto Text::append(const Text& other) -> void {
		m_Data.append(other.m_Data);
	}

	auto Text::append(char c, size_t amt) -> void {
		m_Data.append(c, amt);
	}

	auto Text::append(utf8::codepoint cp, size_t amt) -> void {
		const auto encoded = utf8::encode(cp);
		m_Data.reserve(m_Data.size() + encoded.size() * amt);
		while (amt-- > 0)
			m_Data.append(encoded);
	}

	auto Text::prepend(std::string_view str) -> void {
		m_Data.insert(0, str);
	}

	auto Text::prepend(const Text& text) -> void {
		m_Data = text.m_Data + m_Data;
	}

	auto Text::prepend(char c, size_t amt) -> void {
		m_Data = std::string(c, amt) + m_Data;
	}

	auto Text::prepend(utf8::codepoint cp, size_t amt) -> void {
		std::string s;
		while (amt-- > 0) {
			utf8::encode(s, cp);
		}
		m_Data = s + m_Data;
	}

	auto Text::insert(size_t pos, std::string_view str) -> void {
		auto cps = utf8::codepoints(m_Data);
		expect(pos <= cps.size(), "insert position out of bounds: {} <= {}", pos, cps.size());
		const auto offset = cps.byte_offset(pos);
		m_Data.insert(offset, str);
	}

	auto Text::insert(size_t pos, const Text& text) -> void {
		insert(pos, text.view());
	}

	auto Text::insert(size_t pos, utf8::codepoint cp) -> void {
		insert(pos, utf8::encode(cp));
	}

	auto Text::trim_back(size_t amt) -> void {
		const auto len = length();
		expect(amt <= len, "cannot trim more codepoints than the string contains: {} <= {}", amt, len);
		erase(len - amt, amt);
	}

	auto Text::trim_front(size_t amt) -> void {
		erase(0, amt);
	}
	auto Text::to_lower() -> Text& {
		return transform([](utf8::codepoint cp) -> utf8::codepoint {
			return utf8::to_lower(cp);
		});
	}

	auto Text::to_upper() -> Text& {
		return transform([](utf8::codepoint cp) -> utf8::codepoint {
			return utf8::to_upper(cp);
		});
	}

	auto Text::transform(std::function<utf8::codepoint(utf8::codepoint)>&& proc) -> Text& {
		const auto count = length();

		for (size_t idx = 0; idx < count; ++idx) {
			const auto cp = (*this)[idx];
			set(idx, proc(cp));
		}

		return *this;
	}

	auto Text::erase(size_t pos, size_t amt) -> void {
		auto cps = utf8::codepoints(m_Data);

		expect(pos <= cps.size(), "erase position out of bounds: {} <= {}", pos, cps.size());
		expect(amt <= cps.size() - pos, "erase amount out of bounds: {} <= {}", amt, cps.size() - pos);

		const auto first = cps.byte_offset(pos);
		const auto last  = cps.byte_offset(pos + amt);

		m_Data.erase(first, last - first);
	}

	auto Text::clear() -> void {
		m_Data.clear();
	}

	auto Text::contains(utf8::codepoint cp) const -> bool {
		for (const auto codepoint : utf8::codepoints(m_Data)) {
			if (codepoint == cp)
				return true;
		}
		return false;
	}

	auto Text::contains(std::string_view str) const -> bool {
		return m_Data.find(str) != std::string::npos;
	}

	auto Text::contains(const Text& text) const -> bool {
		return contains(text.data());
	}

	auto Text::starts_with(std::string_view str) const -> bool {
		return m_Data.starts_with(str);
	}

	auto Text::starts_with(utf8::codepoint cp) const -> bool {
		if (empty())
			return false;

		return front() == cp;
	}

	auto Text::starts_with(const Text& text) const -> bool {
		return starts_with(text.data());
	}

	auto Text::ends_with(std::string_view str) const -> bool {
		return m_Data.ends_with(str);
	}

	auto Text::ends_with(utf8::codepoint cp) const -> bool {
		if (empty())
			return false;

		return back() == cp;
	}

	auto Text::ends_with(const Text& text) const -> bool {
		return ends_with(text.data());
	}

	auto Text::bytes() const -> size_t {
		return m_Data.size();
	}

	auto Text::length() const -> size_t {
		return utf8::codepoints(m_Data).size();
	}

	auto Text::empty() const -> bool {
		return m_Data.empty();
	}

	auto Text::view() const -> std::string_view {
		return std::string_view(m_Data);
	}

	auto Text::begin() -> utf8::CodepointIterator {
		return utf8::Codepoints(m_Data).begin();
	}

	auto Text::end() -> utf8::CodepointIterator {
		return utf8::Codepoints(m_Data).end();
	}

	auto Text::begin() const -> utf8::CodepointIterator {
		return utf8::Codepoints(m_Data).begin();
	}

	auto Text::end() const -> utf8::CodepointIterator {
		return utf8::Codepoints(m_Data).end();
	}

	auto Text::cbegin() const -> utf8::CodepointIterator {
		return utf8::Codepoints(m_Data).begin();
	}

	auto Text::cend() const -> utf8::CodepointIterator {
		return utf8::Codepoints(m_Data).end();
	}

	auto Text::back() const -> utf8::codepoint {
		expect(!m_Data.empty(), "cannot get back of empty codepoint range");
		auto it = end();
		--it;
		return *it;
	}

	auto Text::front() const -> utf8::codepoint {
		expect(!empty(), "cannot get front of empty codepoint range");
		return *begin();
	}

	auto Text::set(size_t idx, utf8::codepoint cp) -> void {
		auto cps = utf8::codepoints(m_Data);
		expect(idx < cps.size(), "codepoint index out of bounds: {} < {}", idx, cps.size());
		const auto first = cps.byte_offset(idx);
		const auto last  = cps.byte_offset(idx + 1);
		m_Data.replace(first, last - first, utf8::encode(cp));
	}

	auto Text::operator==(const Text& other) const -> bool {
		return m_Data == other.m_Data;
	}

	auto Text::operator==(std::string_view other) const -> bool {
		return m_Data == other;
	}

	auto Text::operator<=>(const Text& other) const -> std::strong_ordering {
		return m_Data <=> other.m_Data;
	}

	auto Text::operator<=>(std::string_view other) const -> std::strong_ordering {
		return std::string_view(m_Data) <=> other;
	}

	auto Text::operator[](size_t idx) const -> utf8::codepoint {
		return utf8::codepoints(m_Data)[idx];
	}

	auto Text::operator[](size_t idx) -> utf8::codepoint {
		auto codepoints = utf8::codepoints(m_Data);
		auto it         = codepoints.begin();
		std::advance(it, idx);
		return *it;
	}

} // namespace aby::eng
