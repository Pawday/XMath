#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>

#include "u32.hh"
#include "xm_endian.hh"

namespace xm
{

struct u64
{
    constexpr u64(uint64_t val) : m_val(htole(val))
    {
    }

    constexpr u64(std::array<uint8_t, 8> little_endian_val) : m_val(little_endian_val)
    {
    }

    constexpr bool overflow_with(u64 rhs) const
    {
        /* SYMMETRY PADDING */

        u32 L_hs_0({m_val[0], m_val[1], m_val[2], m_val[3]});
        u32 L_hs_1({m_val[4], m_val[5], m_val[6], m_val[7]});
        u32 R_hs_0({rhs.m_val[0], rhs.m_val[1], rhs.m_val[2], rhs.m_val[3]});
        u32 R_hs_1({rhs.m_val[4], rhs.m_val[5], rhs.m_val[6], rhs.m_val[7]});
        /* SYMMETRY PADDING */

        bool part_0_overflow = u32(L_hs_0).overflow_with(R_hs_0);
        bool part_1_overflow = u32(L_hs_1).overflow_with(R_hs_1);

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

    [[nodiscard]] constexpr u64 add_overflow(u64 rhs) const
    {
        u32 L_hs_0({m_val[0], m_val[1], m_val[2], m_val[3]});
        u32 L_hs_1({m_val[4], m_val[5], m_val[6], m_val[7]});
        u32 R_hs_0({rhs.m_val[0], rhs.m_val[1], rhs.m_val[2], rhs.m_val[3]});
        u32 R_hs_1({rhs.m_val[4], rhs.m_val[5], rhs.m_val[6], rhs.m_val[7]});

        bool part_0_overflow = L_hs_0.overflow_with(R_hs_0);
        u32 part_0 = L_hs_0.add_overflow(R_hs_0);

        auto part_0_data = part_0.data_le();
        u64 output({part_0_data[0], part_0_data[1], part_0_data[2], part_0_data[3], m_val[4], m_val[5], m_val[6], m_val[7]});

        u32 new_high({m_val[4], m_val[5], m_val[6], m_val[7]});
        if (part_0_overflow)
        {
            new_high = new_high.add_overflow(1);
        }
        new_high = new_high.add_overflow(R_hs_1);

        auto new_high_data = new_high.data_le();
        output.m_val[4] = new_high_data[0];
        output.m_val[5] = new_high_data[1];
        output.m_val[6] = new_high_data[2];
        output.m_val[7] = new_high_data[3];
        return output;
    }

    constexpr std::optional<u64> operator+(const std::optional<u64> &other)
    {
        if (!other.has_value())
        {
            return std::nullopt;
        }

        if (this->overflow_with(*other))
        {
            return std::nullopt;
        }

        return u64(m_val).add_overflow(other->m_val);
    }

    friend constexpr bool operator==(const u64 &lhs, const u64 &rhs);

    static constexpr u64 sum_of(u32 lhs, u32 rhs)
    {
        uint8_t high_byte = 0;
        if (lhs.overflow_with(rhs))
        {
            high_byte = 1;
        }

        std::array<uint8_t, 4> part_0_data = lhs.add_overflow(rhs).data_le();
        return u64({part_0_data[0], part_0_data[1], part_0_data[2], part_0_data[3], high_byte, 0, 0, 0});
    }

    constexpr std::array<uint8_t, 8> data_le() const
    {
        return m_val;
    }

    constexpr std::array<uint8_t, 8> data_be() const
    {
        std::array<uint8_t, 8> output = m_val;
        std::reverse(begin(output), end(output));
        return output;
    }

  private:
    // Little endian
    std::array<uint8_t, 8> m_val;
};

constexpr bool operator==(const u64 &lhs, const u64 &rhs)
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
