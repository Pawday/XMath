#pragma once

#if 0

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>

#include "u64.hh"

namespace xm
{

struct u128
{
    constexpr u128(u64 ms_part, u64 ls_part) : m_val(make_le(ms_part, ls_part))
    {
    }

    constexpr u128(uint64_t ls_part) : u128(0, ls_part)
    {
    }

    constexpr u128(std::array<uint8_t, 16> little_endian_val) : m_val(little_endian_val)
    {
    }

    constexpr bool overflow_with(u128 rhs) const
    {
        /* SYMMETRY PADDING */

        u64 L_hs_0(this->ls_le_part());
        u64 L_hs_1(this->ms_le_part());
        u64 R_hs_0(rhs.ls_le_part());
        u64 R_hs_1(rhs.ms_le_part());

        bool part_0_overflow = u64(L_hs_0).overflow_with(R_hs_0);
        bool part_1_overflow = u64(L_hs_1).overflow_with(R_hs_1);

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

    [[nodiscard]] constexpr u128 add_overflow(u128 rhs) const
    {
        u64 Lhs_0(this->ls_le_part());
        u64 Lhs_1(this->ms_le_part());
        u64 Rhs_0(rhs.ls_le_part());
        u64 Rhs_1(rhs.ms_le_part());

        u64 part_0 = Lhs_0.add_overflow(Rhs_0);
        auto part_0_data = part_0.data_le();

        u128 output = 0;
        std::copy(begin(part_0_data), end(part_0_data), begin(output.m_val));

        u64 new_high(this->ms_le_part());
        if (Lhs_0.overflow_with(Rhs_0))
        {
            new_high = new_high.add_overflow(1);
        }
        new_high = new_high.add_overflow(Rhs_1);

        auto new_high_data = new_high.data_le();
        std::copy(begin(new_high_data), end(new_high_data), begin(output.m_val) + output.m_val.size() / 2);
        return output;
    }

    constexpr std::optional<u128> operator+(const std::optional<u128> &other)
    {
        if (!other.has_value())
        {
            return std::nullopt;
        }

        if (this->overflow_with(*other))
        {
            return std::nullopt;
        }

        return u128(m_val).add_overflow(other->m_val);
    }

    friend constexpr bool operator==(const u128 &lhs, const u128 &rhs);

    static constexpr u128 sum_of(u64 lhs, u64 rhs)
    {
        uint8_t high_byte = 0;
        if (lhs.overflow_with(rhs))
        {
            high_byte = 1;
        }

        u128 output = 0;
        auto part_0_data = lhs.add_overflow(rhs).data_le();
        auto part_1_data = std::array<uint8_t, sizeof(output) / 2>();
        part_1_data[0] = high_byte;
        std::copy(begin(part_0_data), end(part_0_data), begin(output.m_val));
        std::copy(begin(part_1_data), end(part_1_data), begin(output.m_val) + output.m_val.size() / 2);
        return output;
    }

    constexpr std::array<uint8_t, 16> data_le() const
    {
        return m_val;
    }

    constexpr std::array<uint8_t, 16> data_be() const
    {
        std::array<uint8_t, 16> output = m_val;
        std::reverse(begin(output), end(output));
        return output;
    }

  private:
    // Little endian
    std::array<uint8_t, 16> m_val;

    static constexpr std::array<uint8_t, 16> make_le(u64 ms_part, u64 ls_part)
    {
        std::array<uint8_t, 16> output;
        std::array<uint8_t, 8> ls_data = ls_part.data_le();
        std::array<uint8_t, 8> ms_data = ms_part.data_le();
        std::copy(begin(ls_data), end(ls_data), begin(output));
        std::copy(begin(ms_data), end(ms_data), begin(output) + sizeof(u64));
        return output;
    }

    constexpr std::array<uint8_t, 8> ls_le_part() const
    {
        std::array<uint8_t, 8> output;
        std::copy(begin(m_val), begin(m_val) + 8, begin(output));
        return output;
    }

    constexpr std::array<uint8_t, 8> ms_le_part() const
    {
        std::array<uint8_t, 8> output;
        std::copy(begin(m_val) + 8, begin(m_val) + 16, begin(output));
        return output;
    }
};

constexpr bool operator==(const u128 &lhs, const u128 &rhs)
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

using u128 = u<128>;

}

#endif
