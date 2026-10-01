#pragma once

#include <algorithm>
#include <cstddef>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

namespace aby::eng::meta {

	/** 
	* Checks if type can be initialized via aggregrate initialization
	* @tparam T the type
	* @tparam Args the arguments to initialize @c T with 
	*/
	template <typename T, typename... Args>
	concept CAggregateInitializable = requires {
		T{ std::declval<Args>()... };
	};

	/**
	* Checks if a type can be constructed or aggregrate initialized
	* @tparam T the type
	* @tparam Args the arguments to construct @c T with
	*/
	template <typename T, typename... Args>
	concept CConstructible = std::is_constructible_v<T, Args...> || CAggregateInitializable<T, Args...>;

	/**
	* Gets the type, type defintion from a class  
	*/
	template <typename T>
	using internal_type = typename T::type;

	/**
	* A fixed string for string literals as template paremeters
	* @tparam N the size of the string 
	*/
	template <std::size_t N>
	struct fixed_string {
		char value[N];

		consteval fixed_string(const char (&str)[N]) {
			std::copy_n(str, N, value);
		}

		constexpr operator std::string_view() {
			return value;
		}

		constexpr auto operator==(const fixed_string&) const -> bool = default;
	};

} // namespace aby::eng::meta

namespace aby::eng::meta {

	namespace detail {

		struct type_list_base {};

		template <typename T>
		concept CTypeList = std::is_base_of_v<type_list_base, T>;

		template <typename T, typename Fn>
		constexpr void for_each_t_list(Fn&& fn);

		template <typename T, typename Fn>
		constexpr void for_each_t_one(Fn&& fn) {
			if constexpr (CTypeList<T>) {
				for_each_t_list<T>(std::forward<Fn>(fn));
			} else {
				std::forward<Fn>(fn).template operator()<T>();
			}
		}

		template <typename List, typename Fn, std::size_t... Is>
		constexpr void for_each_t_impl(
		    Fn&& fn,
		    std::index_sequence<Is...>) {
			(for_each_t_one<std::tuple_element_t<Is, typename List::type>>(std::forward<Fn>(fn)), ...);
		}

		template <typename T, typename Fn>
		constexpr void for_each_t_list(Fn&& fn) {
			static_assert(CTypeList<T>);
			for_each_t_impl<T>(std::forward<Fn>(fn), std::make_index_sequence<T::SIZE>{});
		}

	} // namespace detail

	/**
	* A list that "holds" types and can then perform transformation operations
	* on them or be iterated over. 
	* @tparam Ts the types to store. 
	*/
	template <typename... Ts>
	struct type_list : detail::type_list_base {
		type_list(Ts...) {
		}
		type_list(std::tuple<Ts...>) {
		}

		/// The amount of types held by this list
		static constexpr std::size_t SIZE = sizeof...(Ts);
		/// The tuple of types
		using type                        = std::tuple<Ts...>;

		/**
		* Transform a list of types into another @c type_list by applying a template
		* @tparam Transformation the transformation to apply such as @c std::remove_cvref_t  
		*/
		template <template <typename> typename Transformation>
		using transform = type_list<Transformation<Ts>...>;
	};

	/**
	* @brief Iterate a list of types
	* @tparam Ts the types to iterate
	* @param fn The function to execute on each type in the format @code []<typename T>(){...} @endcode
	* @note
	*
	* 	 Passing a @c type_list<...> to this function will iterate over the 
	* 	 types contained by the list. 
	*
	* 	 You can interchange type lists and types freely, even using multiple type lists.
	*/
	template <typename... Ts>
	constexpr void for_each_t(auto&& fn) {
		static_assert(sizeof...(Ts) > 0, "type list cannot be empty");
		(detail::for_each_t_one<Ts>(std::forward<decltype(fn)>(fn)), ...);
	}

} // namespace aby::eng::meta

