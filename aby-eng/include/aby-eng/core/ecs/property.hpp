#pragma once
#include "misc/meta.hpp"

#include <concepts>
#include <format>
#include <memory>
#include <type_traits>

#if defined(__clang__) || defined(__GNUC__)
#	include <cstdlib>
#	include <cxxabi.h>
#endif

/**
* Defines the meta structure for a component containing properties
* @param ... The member variable names. ie. 
* @note
*	
* 	 Usage: ``` struct foo { property<...> x = ...; ABY_ENG_PROPERTIES(x); }; ```
*/
#define ABY_ENG_PROPERTIES(...)                                                      \
public:                                                                              \
	friend struct meta;                                                              \
private:                                                                             \
	using internal_property_type_list =                                              \
	    decltype(::aby::eng::meta::type_list{ __VA_ARGS__ });                        \
public:                                                                              \
	struct meta {                                                                    \
		using property_type_list =                                                   \
		    typename internal_property_type_list::transform<std::remove_cvref_t>;    \
		using type_list =                                                            \
		    typename property_type_list::transform<::aby::eng::meta::internal_type>; \
	};                                                                               \
                                                                                     \
	template <typename Property>                                                     \
	auto get() -> Property& {                                                        \
		auto properties = std::tie(__VA_ARGS__);                                     \
                                                                                     \
		Property* result = nullptr;                                                  \
                                                                                     \
		std::apply(                                                                  \
		    [&]<typename... Ts>(Ts&... properties) {                                 \
			(                                                                        \
			    [&] {                                                                \
				using T = std::remove_cvref_t<Ts>;                                   \
				if constexpr (std::same_as<T, Property>) {                           \
					result = &properties;                                            \
				}                                                                    \
			}(), ...);                                                               \
		},                                                                           \
		    properties);                                                             \
                                                                                     \
		return *result;                                                              \
	}

namespace aby::eng::ecs {

	enum class EProperty {
		visible,
		hidden,
	};

	namespace detail {

		template <typename T, bool = std::is_class_v<T>>
		struct component_property_base;

		template <typename T>
		struct component_property_base<T, true> : public T {
			using type = T;
			using T::T;
			using T::operator=;

			constexpr component_property_base() = default;

			constexpr component_property_base(const T& value) :
			    T(value) {
			}

			constexpr component_property_base(T&& value) :
			    T(std::move(value)) {
			}

			constexpr operator T&() noexcept {
				return *this;
			}

			constexpr operator const T&() const noexcept {
				return *this;
			}
		};

		template <typename T>
		struct component_property_base<T, false> {
			using type = T;

			T value{};

			constexpr component_property_base() = default;

			template <typename U>
			requires std::constructible_from<T, U>
			constexpr component_property_base(U&& value) :
			    value(std::forward<U>(value)) {
			}

			template <typename U>
			requires std::assignable_from<T&, U>
			constexpr auto operator=(U&& value) -> component_property_base& {
				this->value = std::forward<U>(value);
				return *this;
			}

			constexpr operator T&() noexcept {
				return value;
			}

			constexpr operator const T&() const noexcept {
				return value;
			}
		};

		template <typename T>
		struct component_property : component_property_base<T, std::is_class_v<T>> {
			using type = T;
			using base = component_property_base<T, std::is_class_v<T>>;

			using base::base;
			using base::operator=;
		};

	} // namespace detail

	/**
	* A component property structure that wraps a type
	* @tparam DisplayName a string literal name used to display the property in the editor
	* @tparam T the type of object to wrap
	* @tparam Access determined if the property can be changed in the editor
	*/
	template <meta::fixed_string DisplayName, typename T, EProperty Access>
	struct property : detail::component_property<std::remove_cvref_t<decltype(std::declval<T>())>> {
		using type = std::remove_cvref_t<decltype(std::declval<T>())>;
		using base = ::aby::eng::ecs::detail::component_property<type>;
		using base::base;
		using base::operator=;

		template <typename... Args>
		requires(meta::CConstructible<T, Args...>)
		constexpr property(Args&&... args) : base(args...) {
		}

		/**
		* Get the property value
		*/
		auto value() const -> const base& {
			return *this;
		}

		/**
		* Get the property value
		*/
		auto value() -> base& {
			return *this;
		}

		/**
		* The property's meta information
		*/
		struct meta {
			/**
			* Get the display name for the property
			*/
			static constexpr auto name() -> std::string_view {
				return DisplayName.value;
			}
			/**
			* Get the access qualifier for the property
			*/
			static constexpr auto access() -> EProperty {
				return Access;
			}
#if defined(__clang__) || defined(__GNUC__)
			/**
			* Get the demangled type name of the property's wrapped type
			*/
			static auto type_name() -> std::string {
				static auto demangle = [](const char* name) -> std::string {
					int status = 0;

					std::unique_ptr<char, decltype(&std::free)> result{
						abi::__cxa_demangle(name, nullptr, nullptr, &status),
						&std::free
					};

					return status == 0 ? result.get() : name;
				};

				return demangle(typeid(T).name());
			}
#else
			/**
			* Get the type name of the property's wrapped type
			*/
			static constexpr auto type_name() -> std::string_view {
				return typeid(T).name();
			}
#endif
			/**
			* Get the type info of the property's wrapped type
			*/
			static constexpr auto type() -> const std::type_info& {
				return typeid(T);
			}
		};
	};

} // namespace aby::eng::ecs

namespace std {

	template <>
	struct formatter<aby::eng::ecs::EProperty, char> {
		template <class ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& ctx) {
			auto it = ctx.begin();
			if (it != ctx.end() && *it != '}')
				throw format_error("invalid format");
			return it;
		}

		template <class FmtContext>
		FmtContext::iterator format(const aby::eng::ecs::EProperty& a, FmtContext& ctx) const {
			std::string_view str = "<unknown>";
			switch (a) {
				case aby::eng::ecs::EProperty::hidden: {
					str = "private";
					break;
				}
				case aby::eng::ecs::EProperty::visible: {
					str = "public";
					break;
				}
			}
			return std::ranges::copy(str, ctx.out()).out;
		}
	};

	template <aby::eng::meta::fixed_string DisplayName, typename T, aby::eng::ecs::EProperty Access>
	struct formatter<aby::eng::ecs::property<DisplayName, T, Access>, char> {
		using object_type = aby::eng::ecs::property<DisplayName, T, Access>;
		using meta        = object_type::meta;

		template <class ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& ctx) {
			auto it = ctx.begin();
			if (it != ctx.end() && *it != '}')
				throw format_error("invalid format");
			return it;
		}

		template <class FmtContext>
		FmtContext::iterator format(const object_type& p, FmtContext& ctx) const {
			return std::format_to(ctx.out(), "[ name: {}, access: {}, type: {}, value: {} ]", meta::name(), meta::access(), meta::type_name(), p.value());
		}
	};

} // namespace std
