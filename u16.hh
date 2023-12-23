#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>

#include "u8.hh"
#include "xm_endian.hh"

namespace xm
{
struct u16
{

    constexpr u16(uint16_t val) : m_val(htole(val))
    {
    }

    constexpr u16(std::array<uint8_t, 2> little_endian_val) : m_val(little_endian_val)
    {
    }

    constexpr bool overflow_with(u16 r) const
    {
        bool b0_overflow = u8(this->m_val[0]).overflow_with(r.m_val[0]);
        bool b1_overflow = u8(this->m_val[1]).overflow_with(r.m_val[1]);

        if (!b0_overflow && !b1_overflow)
        {
            return false;
        }

        if (b0_overflow && b1_overflow)
        {
            return true;
        }

        if (b1_overflow)
        {
            return true;
        }

        return u8(this->m_val[1]).overflow_with(1);
    }

    [[nodiscard]] constexpr u16 add_overflow(u16 increment) const
    {
        bool byte_0_overflow = u8(this->m_val[0]).overflow_with(increment.m_val[0]);
        u8 byte_0 = u8(this->m_val[0]).add_overflow(increment.m_val[0]);

        u16 output({byte_0.data(), this->m_val[1]});

        if (!this->overflow_with(increment))
        {
            if (byte_0_overflow)
            {
                output.m_val[1]++;
            }
            return output;
        }

        u8 new_high(this->m_val[1]);
        if (byte_0_overflow)
        {
            new_high = new_high.add_overflow(1);
        }
        new_high = new_high.add_overflow(increment.m_val[1]);

        output.m_val[1] = new_high.data();
        return output;
    }

    constexpr std::optional<u16> operator+(const std::optional<u16> &other)
    {
        if (!other.has_value())
        {
            return std::nullopt;
        }

        if (this->overflow_with(*other))
        {
            return std::nullopt;
        }

        return u16(this->m_val).add_overflow(other->m_val);
    }

    friend constexpr bool operator==(const u16 &lhs, const u16 &rhs);

    static constexpr u16 sum_of(u8 lhs, u8 rhs)
    {
        uint8_t high_byte = 0;
        if (lhs.overflow_with(rhs))
        {
            high_byte = 1;
        }
        return u16({lhs.add_overflow(rhs).data(), high_byte});
    }

    constexpr std::array<uint8_t, 2> data() const
    {
        return this->m_val;
    }

  private:
    // Little endian
    std::array<uint8_t, 2> m_val;
};

constexpr bool operator==(const u16 &lhs, const u16 &rhs)
{
    for (size_t byte_idx = 0; byte_idx < lhs.m_val.size(); byte_idx++)
    {
        if (lhs.m_val[byte_idx] != rhs.m_val[byte_idx])
        {
            return false;
        }
    }

    return true;
}

} // namespace xm
