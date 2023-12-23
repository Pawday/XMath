#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace xm
{

template <typename T>
    requires std::is_unsigned_v<T>
constexpr std::array<uint8_t, std::numeric_limits<T>::digits / 8> htole(T val)
{
    constexpr size_t bytes = std::numeric_limits<T>::digits / 8;

    std::array<uint8_t, bytes> output;

    for (size_t byte_idx = 0; byte_idx < bytes; byte_idx++)
    {
        T mask = 0xff;
        mask <<= (byte_idx * 8);
        T val_at_byte = val & mask;
        val_at_byte >>= (byte_idx * 8);
        output[byte_idx] = val_at_byte;
    }

    return output;
}

template <typename T>
    requires std::is_unsigned_v<T>
constexpr std::array<uint8_t, std::numeric_limits<T>::digits / 8> htobe(T val)
{
    constexpr size_t bytes = std::numeric_limits<T>::digits / 8;

    std::array<uint8_t, bytes> output;

    for (size_t byte_idx = 0; byte_idx < bytes; byte_idx++)
    {
        T mask = 0xff;
        mask <<= ((bytes - 1 - byte_idx) * 8);
        T val_at_byte = val & mask;
        val_at_byte >>= (byte_idx * 8);
        output[byte_idx] = val_at_byte;
    }
    return output;
}

template <typename T>
    requires std::is_unsigned_v<T>
constexpr T letoh(const std::array<uint8_t, std::numeric_limits<T>::digits / 8> &data)
{
    constexpr size_t bytes = std::numeric_limits<T>::digits / 8;

    T output = 0;

    for (size_t byte_idx = 0; byte_idx < bytes; byte_idx++)
    {
        T val = data[byte_idx];
        val <<= (byte_idx * 8);
        output |= val;
    }

    return output;
}

template <typename T>
    requires std::is_unsigned_v<T>
constexpr T betoh(const std::array<uint8_t, std::numeric_limits<T>::digits / 8> &data)
{
    constexpr size_t bytes = std::numeric_limits<T>::digits / 8;

    T output = 0;

    for (size_t byte_idx = 0; byte_idx < bytes; byte_idx++)
    {
        T val = data[byte_idx];
        val <<= ((bytes - 1 - byte_idx) * 8);
        output |= val;
    }

    return output;
}

} // namespace xm
