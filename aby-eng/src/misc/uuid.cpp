#include "misc/uuid.hpp"

#include <random>

namespace aby::eng {

	namespace {

		thread_local std::random_device rd;
		thread_local std::mt19937_64 gen(rd());
		thread_local std::uniform_int_distribution<uint64_t> dist(1);

	} // namespace

	UUID::UUID() :
	    m_Value(dist(gen)) {
	}

	auto UUID::value() const -> u64 {
		return m_Value;
	}

	UUID::operator u64() const {
		return m_Value;
	}

	UUID::operator bool() const {
		return m_Value != 0;
	}

	auto UUID::operator==(const UUID& other) const -> bool {
		return m_Value == other.m_Value;
	}

	auto UUID::operator!=(const UUID& other) const -> bool {
		return m_Value != other.m_Value;
	}

	auto UUID::operator=(std::nullopt_t) -> UUID& {
		m_Value = 0;
		return *this;
	}

	auto UUID::operator=(std::nullptr_t) -> UUID& {
		m_Value = 0;
		return *this;
	}

} // namespace aby::eng
