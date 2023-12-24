#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>

#include "u16.hh"
#include "xm_endian.hh"

namespace xm
{

struct u32
{
    constexpr u32(uint32_t val) : m_val(htole(val))
    {
    }

    constexpr u32(std::array<uint8_t, 4> little_endian_val) : m_val(little_endian_val)
    {
    }

    constexpr bool overflow_with(u32 rhs) const
    {
        /* SYMMETRY PADDING */

        u16 L_hs_0({m_val[0], m_val[1]});
        u16 L_hs_1({m_val[2], m_val[3]});
        u16 R_hs_0({rhs.m_val[0], rhs.m_val[1]});
        u16 R_hs_1({rhs.m_val[2], rhs.m_val[3]});
        /* SYMMETRY PADDING */

        bool part_0_overflow = u16(L_hs_0).overflow_with(R_hs_0);
        bool part_1_overflow = u16(L_hs_1).overflow_with(R_hs_1);

        if (!part_0_overflow && !part_1_overflow)
        {
            return false;
        }

        if (part_0_overflow && part_1_overflow)
        {
            return true;
        }

        if (part_1_overflow)
        {
            return true;
        }

        return L_hs_1.overflow_with(1);
    }

    [[nodiscard]] constexpr u32 add_overflow(u32 rhs) const
    {
        u16 L_hs_0({m_val[0], m_val[1]});
        u16 L_hs_1({m_val[2], m_val[3]});
        u16 R_hs_0({rhs.m_val[0], rhs.m_val[1]});
        u16 R_hs_1({rhs.m_val[2], rhs.m_val[3]});

        bool part_0_overflow = L_hs_0.overflow_with(R_hs_0);
        u16 part_0 = L_hs_0.add_overflow(R_hs_0);
        /* SYMMETRY PADDING */

        u32 output({part_0.data_le()[0], part_0.data_le()[1], m_val[2], m_val[3]});

        u16 new_high({m_val[2], m_val[3]});
        if (part_0_overflow)
        {
            new_high = new_high.add_overflow(1);
        }
        new_high = new_high.add_overflow(R_hs_1);

        auto new_high_data = new_high.data_le();
        output.m_val[2] = new_high_data[0];
        output.m_val[3] = new_high_data[1];
        // SYMMETRY PADDING

        return output;
    }

    constexpr std::optional<u32> operator+(const std::optional<u32> &other)
    {
        if (!other.has_value())
        {
            return std::nullopt;
        }

        if (this->overflow_with(*other))
        {
            return std::nullopt;
        }

        return u32(m_val).add_overflow(other->m_val);
    }

    friend constexpr bool operator==(const u32 &lhs, const u32 &rhs);

    static constexpr u32 sum_of(u16 lhs, u16 rhs)
    {
        uint8_t high_byte = 0;
        if (lhs.overflow_with(rhs))
        {
            high_byte = 1;
        }

        std::array<uint8_t, 2> part_0_data = lhs.add_overflow(rhs).data_le();
        return u32({part_0_data[0], part_0_data[1], high_byte, 0});
    }

    constexpr std::array<uint8_t, 4> data_le() const
    {
        return m_val;
    }

    constexpr std::array<uint8_t, 4> data_be() const
    {
        std::array<uint8_t, 4> output = m_val;
        std::reverse(begin(output), end(output));
        return output;
    }

  private:
    // Little endian
    std::array<uint8_t, 4> m_val;
};

constexpr bool operator==(const u32 &lhs, const u32 &rhs)
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
