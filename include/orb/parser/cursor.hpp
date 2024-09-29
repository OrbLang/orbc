#pragma once

// STDLIB
#include <algorithm>
#include <concepts>
#include <iterator>
#include <optional>
#include <utility>
#include <vector>


/** @brief A simpler iterator that owns it's data, and without the use of operator overloading.
 *  A `Cursor` should only be used when c++ iterator algorithms will not be used,
 *  and when only simple input-forward iteration is required.
 *
 *  The `Cursor` class only provides a more explicit interface compared to c++ iterators,
 *  and provides more safety by returning `std::optional<T>` and doesn't throw any exceptions.
 */
template <typename T, typename AllocT = std::vector<const T>::allocator_type>
class Cursor
{
public:
    using reference = const T&;
    using value_type = const T;
    using allocator_type = AllocT;

private:
    using ReturnT = std::optional<reference>;

    /// Owned `T` array to iterate over
    std::vector<const T, AllocT> data;
    size_t current = 0;

public:
    /// Constructs an empty Cursor.
    /// Should only be used for placeholders.
    constexpr Cursor() noexcept = default;

    /// Copy constructor, copies data with and after the current value
    /// and constructs a new Cursor with copied data.
    constexpr Cursor(const Cursor& other) noexcept;

    /// Move constructor, overtakes ownership of data.
    constexpr Cursor(Cursor&& other) noexcept = default;

    /// Copies data with and after the current value
    /// and constructs a new Cursor with copied data.
    constexpr auto operator=(const Cursor& other) noexcept -> Cursor&;

    /// Moves and takes ownership of the data inside other
    /// This releases the data of this Cursor
    constexpr auto operator=(Cursor&& other) noexcept -> Cursor&;


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

    /*  @brief Construct a Cursor by copying from a vector
     *
     *  @param vec The vector to copy
     */
    explicit constexpr Cursor(const std::vector<T, AllocT>& vec) noexcept
        requires std::copy_constructible<T>
        : data(vec)
    {}

    /*  @breif Construct a Cursor by copying from any range
     *
     *  @param inputRange The range to copy
     */
    explicit constexpr Cursor(const std::ranges::input_range auto& inputRange) noexcept
        : data(std::ranges::distance(inputRange))
    {
        std::ranges::copy(inputRange, std::back_inserter(data));
    }


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


    /*  @brief Construct a Cursor by taking ownership of the data in a vector
     *
     *  @param vec The vector which's data will be moved
     */
    explicit constexpr Cursor(std::vector<T, AllocT>&& vec) noexcept
        : data(std::move(vec))
    {
        vec.clear();
    }

    /*  @brief Construct a Cursor by moving the data from a range
     *  taking ownership of the data inside
     *
     *  @param inputRange The range to move values from
     */
    explicit constexpr Cursor(std::ranges::input_range auto&& inputRange) noexcept
        : data(std::ranges::distance(inputRange))
    {
        std::ranges::move(std::move(inputRange), std::back_inserter(data));
    }


    // iterator functions, with a safety layer
    // by wrapping return values in `std::optional`


    /*  @brief Get the value currently pointed to inside the Cursor.
     *
     *  @return Returns a `const T&` if the current value is valid,
     *  otherwise an invalid `nullopt` is returned.
     */
    [[nodiscard]] auto Get() const noexcept -> ReturnT
    {
        if (current >= data.size())
            return std::nullopt;

        return data[current];
    }

    /*  @breif Get the value currently pointed to inside the Cursor.
     *  And then iterate to the next value.
     *
     *  @return Returns a `const T&` if the current value is valid,
     *  otherwise an invalid `nullopt` is returned.
     */
    [[nodiscard]] auto Eat() noexcept -> ReturnT
    {
        decltype(auto) oldVal = Get();
        Next();
        return oldVal;
    }

    /*  @brief Get the n'th value after the current value inside the Cursor
     *  without modifiying the current value.
     *
     *  @return Returns a `const T&` if the n'th value exists,
     *  otherwise an invalid `nullopt` is returned.
     */
    [[nodiscard]] auto Peek(size_t distance) const noexcept -> ReturnT
    {
        const size_t peekIndex = current + distance;
        if (peekIndex >= data.size())
            return std::nullopt;

        return data[peekIndex];
    }

    /*  @brief Moves the Cursor to the next value in the internal array.
     *
     *  @return Returns `*this`.
     */
    auto Next() noexcept -> Cursor&
    {
        current++;
        return *this;
    }

private:
    /// Get the remaining amount of objects to iterate over.
    auto RemainingLength() -> size_t { return data.size() - current; }
};
