#include "misc/time.hpp"

namespace aby::eng {

	Time::Time(std::chrono::milliseconds ms) :
	    m_Nanoseconds(std::chrono::duration_cast<std::chrono::nanoseconds>(ms)) {
	}

	Time::Time(std::chrono::seconds s) :
	    m_Nanoseconds(std::chrono::duration_cast<std::chrono::nanoseconds>(s)) {
	}

	Time::Time(std::chrono::nanoseconds ns) :
	    m_Nanoseconds(ns) {
	}

	Time::Time(std::chrono::microseconds us) :
	    m_Nanoseconds(std::chrono::duration_cast<std::chrono::nanoseconds>(us)) {
	}

	auto Time::milli() const -> float {
		return std::chrono::duration<float, std::milli>(m_Nanoseconds).count();
	}

	auto Time::sec() const -> float {
		return std::chrono::duration<float>(m_Nanoseconds).count();
	}

	auto Time::nano() const -> float {
		return static_cast<float>(m_Nanoseconds.count());
	}

	auto Time::micro() const -> float {
		return std::chrono::duration<float, std::micro>(m_Nanoseconds).count();
	}

} // namespace aby::eng
