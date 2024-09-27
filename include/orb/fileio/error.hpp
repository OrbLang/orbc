#pragma once

// Project Headers
#include <orb/concepts.hpp>

namespace orb
{

template <class T>
    requires orb::StdPrintable<T> || orb::StdOstreamPrintable<T>
class Error
{
    T description;

public:
    consteval Error() noexcept : description("Unknown error in IO") {}
    constexpr Error(Error&& other) noexcept = default;
    constexpr Error(const Error& other) noexcept = default;

    constexpr Error(T&& description) noexcept : description(std::forward<T>(description)) {}

    auto operator=(Error&&) -> Error& = delete;
    auto operator=(const Error&) -> Error& = delete;

    auto GetDescription() const -> const T& { return description; }
    friend auto operator<<(std::ostream& stream, const Error& err) -> std::ostream&
    {
        stream << err.GetDescription();
        return stream;
    }
};

} // namespace orb
