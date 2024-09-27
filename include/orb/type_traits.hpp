#pragma once
#include <concepts>
#include <type_traits>
namespace orb
{
template <typename T, typename... Tn>
struct is_any_of : std::bool_constant<(std::same_as<T, Tn> || ...)>
{};

template <typename T, typename... Tn>
inline constexpr bool is_any_of_v = is_any_of<T, Tn...>::value;


template <typename T, typename... Tn>
struct is_convertible_to_any_of : std::bool_constant<(std::convertible_to<T, Tn> || ...)>
{};

template <typename T, typename... Tn>
inline constexpr bool is_convertible_to_any_of_v = is_convertible_to_any_of<T, Tn...>::value;
} // namespace orb
