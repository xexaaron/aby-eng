#pragma once
#include "common.hpp"

#include <chrono>

namespace aby::eng {

	/// @brief Time class mainly used for deltatime with easy conversion functions
	/// @note  formatter {:ms} (milli) {:s} (sec) {:ns} (nano) {:us} (micro)
	// 		   calls function then uses suffix ms, s, ns, μs
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

namespace std {

	template <>
	struct formatter<aby::eng::Time, char> {
	private:
		enum class EUnit {
			milli,
			sec,
			nano,
			micro
		};

		EUnit m_Unit = EUnit::milli;
	public:
		template <class ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& ctx) {
			auto it  = ctx.begin();
			auto end = ctx.end();

			if (it == end || *it == '}')
				return it;

			if (*it == 'm') {
				++it;

				if (it != end && *it == 's') {
					m_Unit = EUnit::milli;
					++it;
				} else {
					throw format_error("invalid Time format");
				}
			} else if (*it == 's') {
				m_Unit = EUnit::sec;
				++it;
			} else if (*it == 'n') {
				++it;

				if (it != end && *it == 's') {
					m_Unit = EUnit::nano;
					++it;
				} else {
					throw format_error("invalid Time format");
				}
			} else if (*it == 'u') {
				++it;

				if (it != end && *it == 's') {
					m_Unit = EUnit::micro;
					++it;
				} else {
					throw format_error("invalid Time format");
				}
			} else {
				throw format_error("invalid Time format");
			}

			if (it != end && *it != '}')
				throw format_error("invalid Time format");

			return it;
		}

		template <class FmtContext>
		FmtContext::iterator format(const aby::eng::Time& t, FmtContext& ctx) const {
			switch (m_Unit) {
				case EUnit::milli:
					return format_to(ctx.out(), "{}ms", t.milli());
				case EUnit::sec:
					return format_to(ctx.out(), "{}s", t.sec());
				case EUnit::nano:
					return format_to(ctx.out(), "{}ns", t.nano());
				case EUnit::micro:
					return format_to(ctx.out(), "{}μs", t.micro());
			}
			return ctx.out();
		}
	};

} // namespace std
