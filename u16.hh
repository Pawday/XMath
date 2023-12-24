#pragma once


#if 0

#include <algorithm>
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

    constexpr bool overflow_with(u16 rhs) const
    {
        u8 Lhs_0(this->ls_part());
        u8 Lhs_1(this->ms_part());
        u8 Rhs_0(rhs.ls_part());
        u8 Rhs_1(rhs.ms_part());

        bool b0_overflow = Lhs_0.overflow_with(Rhs_0);
        bool b1_overflow = Lhs_1.overflow_with(Rhs_1);

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

        return Lhs_1.overflow_with(1);
    }

    [[nodiscard]] constexpr u16 add_overflow(u16 rhs) const
    {
        u8 Lhs_0(this->ls_part());
        u8 Lhs_1(this->ms_part());
        u8 Rhs_0(rhs.ls_part());
        u8 Rhs_1(rhs.ms_part());

        bool part_0_overflow = Lhs_0.overflow_with(Rhs_0);
        u8 part_0 = Lhs_0.add_overflow(Rhs_0);
        auto part_0_data = part_0.data();

        u16 output = 0;
        *begin(output.m_val) = part_0_data;

        u8 new_high(this->ms_part());
        if (part_0_overflow)
        {
            new_high = new_high.add_overflow(1);
        }
        new_high = new_high.add_overflow(Rhs_1);

        auto new_high_data = new_high.data();
        *(begin(output.m_val) + output.m_val.size() / 2) = new_high_data;
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

        return u16(m_val).add_overflow(other->m_val);
    }

    friend constexpr bool operator==(const u16 &lhs, const u16 &rhs);

    static constexpr u16 sum_of(u8 lhs, u8 rhs)
    {
        uint8_t high_byte = 0;
        if (lhs.overflow_with(rhs))
        {
            high_byte = 1;
        }
        /* SYMMETRY PADDING */

        return u16({lhs.add_overflow(rhs).data(), high_byte});
    }

    constexpr std::array<uint8_t, 2> data_le() const
    {
        return m_val;
    }

    constexpr std::array<uint8_t, 2> data_be() const
    {
        std::array<uint8_t, 2> output = m_val;
        std::reverse(begin(output), end(output));
        return output;
    }

  private:
    // Little endian
    std::array<uint8_t, 2> m_val;

    constexpr uint8_t ls_part() const
    {
        return m_val[0];
    }

    constexpr uint8_t ms_part() const
    {
        return m_val[1];
    }
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

#else

#include "unsigned.hh"

namespace xm
{

using u16 = u<16>;

}

#endif
