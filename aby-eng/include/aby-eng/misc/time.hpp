#pragma once
#include "common.hpp"

#include <chrono>

namespace aby::eng {

	class ABY_API Time {
	public:
		explicit Time(std::chrono::milliseconds ms);
		explicit Time(std::chrono::seconds s);
		explicit Time(std::chrono::nanoseconds s);
		explicit Time(std::chrono::microseconds s);

		auto milli() const -> float;
		auto sec() const -> float;
		auto nano() const -> float;
		auto micro() const -> float;
	private:
		std::chrono::nanoseconds m_Nanoseconds;
	};

} // namespace aby::eng
