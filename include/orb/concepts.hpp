#pragma once
// STDLIB
#include <concepts>


template <typename T, typename... Ts>
concept IsAnyOf = (std::same_as<T, Ts> || ...);
