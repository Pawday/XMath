#pragma once

#include <concepts>
#include <limits>
#if 0
#include <cstdint>
#include <optional>

namespace xm
{
struct u8
{
    constexpr u8(uint8_t val) : m_val(val)
    {
    }

    constexpr bool overflow_with(u8 r) const
    {
        return this->overflow_with(r.m_val);
    }

    constexpr bool overflow_with(uint8_t r) const
    {
        return this->m_val > (0xff - r);
    }

    // 0xff + 0x01 = 0x00
    [[nodiscard]] constexpr u8 add_overflow(u8 increment) const
    {
        if (!this->overflow_with(increment))
        {
            return u8(this->m_val + increment.m_val);
        }

        return [](u8 l, u8 r)
        {
            if ((0xff - l.m_val) > r.m_val)
            {
                return u8(0xff - ((0xff - l.m_val) - r.m_val));
            }
            return u8(0xff - ((0xff - r.m_val) - l.m_val));
        }(*this, increment);
    }

    constexpr std::optional<u8> operator+(const std::optional<u8> &other)
    {
        if (!other.has_value())
        {
            return std::nullopt;
        }
        if (this->overflow_with(*other))
        {
            return std::nullopt;
        }

        return u8(this->m_val).add_overflow(other->m_val);
    }

    friend constexpr bool operator==(const u8 &lhs, const u8 &rhs);

    constexpr bool operator==(const u8 &other) const
    {
        return other.m_val == this->m_val;
    }

    constexpr uint8_t data() const
    {
        return m_val;
    }

  private:
    uint8_t m_val;
};

constexpr bool operator==(const u8 &lhs, const u8 &rhs)
{
    return lhs.m_val == rhs.m_val;
}


} // namespace xm
#else

#include "unsigned.hh"

namespace xm
{
struct u8
{
    constexpr u8(uint8_t val) : m_data(val)
    {
    }

    constexpr bool operator==(u8 other)
    {
        return m_data == other.m_data;
    }

    constexpr bool operator==(uint8_t other)
    {
        return m_data == u8(other).m_data;
    }

    template <std::unsigned_integral T> constexpr bool operator==(uint16_t other)
    {
        if (other > std::numeric_limits<uint8_t>::max())
        {
            return false;
        }
        return m_data == u8(other).m_data;
    }

    constexpr bool overflow_with(u8 other) const
    {
        return this->m_data.overflow_with(other.m_data);
    }

    [[nodiscard]] constexpr u8 add_overflow(u8 other) const
    {
        return this->m_data.add_overflow(other.m_data).data_le()[0];
    }

    constexpr std::optional<u8> operator+(const std::optional<u8> &other) const
    {
        if (!other.has_value())
        {
            return std::nullopt;
        }
        if (this->overflow_with(*other))
        {
            return std::nullopt;
        }

        return this->m_data.add_overflow(other->m_data).data_le()[0];
    }

  private:
    u<8> m_data;
};

} // namespace xm

#endif
