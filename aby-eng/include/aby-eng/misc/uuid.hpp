#pragma once
#include "common.hpp"

#include <format>
#include <optional>

namespace aby::eng {

	class ABY_API UUID {
	public:
		/// @brief Generate a valid uuid
		UUID();
		/// @brief Generate an invalid uuid
		constexpr UUID(std::nullopt_t) : m_Value(0) {
		}
		/// @brief Generate an invalid uuid
		constexpr UUID(std::nullptr_t) : m_Value(0) {
		}
		~UUID() = default;

		auto value() const -> u64;

		explicit operator u64() const;
		explicit operator bool() const;

		auto operator==(const UUID& other) const -> bool;
		auto operator!=(const UUID& other) const -> bool;

		auto operator=(std::nullopt_t) -> UUID&;
		auto operator=(std::nullptr_t) -> UUID&;
	public:
		static constexpr auto null() -> UUID {
			return UUID(std::nullopt);
		}
	private:
		u64 m_Value;
	};

} // namespace aby::eng

namespace std {

	template <>
	struct formatter<aby::eng::UUID, char> {
		template <class ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& ctx) {
			auto it = ctx.begin();
			if (it != ctx.end() && *it != '}')
				throw format_error("invalid format");
			return it;
		}

		template <class FmtContext>
		FmtContext::iterator format(const aby::eng::UUID& uuid, FmtContext& ctx) const {
			if (uuid) {
				return format_to(ctx.out(), "<{}>", uuid.value());
			} else {
				return format_to(ctx.out(), "<{}>", "null");
			}
		}
	};

} // namespace std
