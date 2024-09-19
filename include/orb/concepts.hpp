#pragma once
// STDLIB
#include <concepts>
#include <iostream>
#include <orb/type_traits.hpp>
#include <print>

namespace orb
{

template <typename T, typename... Tn>
concept AnyOf = is_any_of_v<T, Tn...>;

template <typename T, typename... Tn>
concept ConvertibleToAnyOf = is_convertible_to_any_of_v<T, Tn...>;

// Printing to the standard streams

template <typename T>
concept StdOstreamPrintable = requires(T object) {
    { std::cout << object } -> std::same_as<decltype(std::cout)&>;
    { std::cerr << object } -> std::same_as<decltype(std::cerr)&>;
};

template <typename T>
concept StdPrintable = requires(T object) {
    { std::print("{}", object) };
    { std::println("{}", object) };
};


} // namespace orb
