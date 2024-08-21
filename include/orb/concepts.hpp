#pragma once
// STDLIB
#include <concepts>


template <typename T, typename... Ts>
concept IsOneOf = (std::same_as<T, Ts> || ...);
