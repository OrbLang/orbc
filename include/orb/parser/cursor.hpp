#pragma once

// STDLIB
#include <algorithm>
#include <concepts>
#include <iterator>
#include <memory>
#include <optional>
#include <ranges>
#include <type_traits>
#include <utility>
#include <vector>

namespace orb
{

/** @brief A simpler iterator that owns it's data, and without the use of operator overloading.
 *  A `Cursor` should only be used when c++ iterator algorithms will not be used,
 *  and when only simple input-forward iteration is required.
 *
 *  The `Cursor` class only provides a more explicit interface compared to c++ iterators,
 *  and provides more safety by returning `std::optional<T>` and doesn't throw any exceptions.
 */
template <typename T, typename AllocT = typename std::vector<std::remove_cv_t<T>>::allocator_type>
class Cursor
{
public:
    using pointer        = const T*;
    using value_type     = T;
    using allocator_type = AllocT;

private:
    using ReturnT = std::optional<pointer>;

    // Maybe std::unique_ptr<value_type[]> is better
    // as there is less overhead?

    /// Owned `T` array to iterate over
    std::vector<value_type, AllocT> data;
    size_t current = 0;

public:
    /// Constructs an empty Cursor.
    /// Should only be used for placeholders.
    constexpr Cursor() noexcept = default;

    /// Copy constructor, copies data with and after the current value
    /// and constructs a new Cursor with copied data.
    explicit constexpr Cursor(const Cursor& other) noexcept = default;

    /// Move constructor, overtakes ownership of data.
    explicit constexpr Cursor(Cursor&& other) noexcept = default;

    /// Copies data with and after the current value
    /// and constructs a new Cursor with copied data.
    constexpr auto operator=(const Cursor& other) noexcept -> Cursor&
        requires std::copy_constructible<value_type>
    {
        data.assign(other.data.begin() + other.current, other.data.end());
    }

    /// Moves and takes ownership of the data inside other
    /// This releases the data of this Cursor
    constexpr auto operator=(Cursor&& other) noexcept -> Cursor& = default;


    // Contructors copying data


    /*  @brief Construct a Cursor by copying from a c array
     *
     *  @param array The array to copy
     *  @param size The size of the array
     */
    explicit constexpr Cursor(T array[], size_t size) noexcept
        : data(array, array + size)
    {}

    /*  @brief Construct a Cursor by copying from a c array
     *
     *  @param array The array to copy
     *  @tparam size The size of the array
     */
    template <size_t size>
    explicit constexpr Cursor(T array[size]) noexcept
        : Cursor(array, size)
    {}

    /*  @breif Construct a Cursor by copying from any range
     *
     *  @param inputRange The range to copy
     */
    explicit constexpr Cursor(const std::ranges::input_range auto& range) noexcept
        : data(std::from_range, range)
    {}


    // Contructors moving data, and taking ownership


    /*  @brief Construct a Cursor by taking ownership of a c array
     *
     *  @param array The array to move
     *  @param size The size of the array
     */
    explicit constexpr Cursor(T*&& array, size_t size) noexcept
        : data(std::move_iterator(std::move(array)), std::move_sentinel(std::move(array + size)))
    {
        array = nullptr;
    }

    /*  @brief Construct a Cursor by moving the data from a range
     *  taking ownership of the data inside
     *
     *  @param inputRange The range to move values from
     */
    explicit constexpr Cursor(std::ranges::input_range auto&& range) noexcept
        : data(
              std::make_move_iterator(std::ranges::begin(range)),
              std::make_move_iterator(std::ranges::end(range))
          )

    // Or with ranges: (Not sure wich is best...)

    /* : data(                                                  */
    /*   std::from_range,                                       */
    /*       std::ranges::subrange(                             */
    /*           std::move_iterator(std::ranges::begin(range)), */
    /*           std::move_sentinel(std::ranges::end(range))    */
    /*       )                                                  */
    /*   )                                                      */
    {}


    // iterator functions, with a safety layer
    // by wrapping return values in `std::optional`


    /*  @brief Get the value currently pointed to inside the Cursor.
     *
     *  @return Returns a `const T&` if the current value is valid,
     *  otherwise an invalid `nullopt` is returned.
     */
    [[nodiscard]]
    constexpr auto Get() const noexcept -> ReturnT
    {
        if (current >= data.size())
            return std::nullopt;

        return std::make_optional(GetPointerAt(current));
    }

    /*  @breif Get the value currently pointed to inside the Cursor.
     *  And then iterate to the next value.
     *
     *  @return Returns a `const T&` if the current value is valid,
     *  otherwise an invalid `nullopt` is returned.
     */
    [[nodiscard("Use `Next()` method to iterate, instead of `Eat()`")]]
    constexpr auto Eat() noexcept -> ReturnT
    {
        ReturnT oldVal = Get();
        Next();

        return oldVal;
    }

    /*  @brief Get the n'th value after the current value inside the Cursor
     *  without modifiying the current value.
     *
     *  @return Returns a `const T&` if the n'th value exists,
     *  otherwise an invalid `nullopt` is returned.
     */
    [[nodiscard]]
    constexpr auto Peek(size_t distance) const noexcept -> ReturnT
    {
        const size_t peekIndex = current + distance;
        if (peekIndex >= data.size())
            return std::nullopt;

        return std::make_optional(GetPointerAt(peekIndex));
    }

    /*  @brief Moves the Cursor to the next value in the internal array.
     *
     *  @return Returns `*this`.
     */
    constexpr auto Next() noexcept -> Cursor&
    {
        current++;
        return *this;
    }

    /*  @brief Checks whether the `Cursor` has anymore values.
        `IsValid()` is a helper function, and is a simpler alias for `has_value()`
        on the `std::optional`.
     *
     *  ```cpp
     *  cursor.IsValid()
     *  ```
     *  same as:
     *
     *  ```cpp
     *  cursor.Get().has_value();
     *  ```
     *
     *  @return Returns true if there are values left, false if there are not
     */
    constexpr auto IsValid() noexcept -> bool { return Get().has_value(); }

private:
    auto GetPointerAt(size_t idx) const noexcept -> pointer { return std::addressof(data[idx]); }
};

} // namespace orb
