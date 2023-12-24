#pragma once

#if 0

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
        u16 L_hs_0(this->ls_le_part());
        u16 L_hs_1(this->ms_le_part());
        u16 R_hs_0(rhs.ls_le_part());
        u16 R_hs_1(rhs.ms_le_part());

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
        u16 Lhs_0(this->ls_le_part());
        u16 Lhs_1(this->ms_le_part());
        u16 Rhs_0(rhs.ls_le_part());
        u16 Rhs_1(rhs.ms_le_part());

        u16 part_0 = Lhs_0.add_overflow(Rhs_0);
        auto part_0_data = part_0.data_le();

        u32 output = 0;
        std::copy(begin(part_0_data), end(part_0_data), begin(output.m_val));

        u16 new_high(this->ms_le_part());
        if (Lhs_0.overflow_with(Rhs_0))
        {
            new_high = new_high.add_overflow(1);
        }
        new_high = new_high.add_overflow(Rhs_1);

        auto new_high_data = new_high.data_le();
        std::copy(begin(new_high_data), end(new_high_data), begin(output.m_val) + output.m_val.size() / 2);
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

        u32 output = 0;
        auto part_0_data = lhs.add_overflow(rhs).data_le();
        auto part_1_data = std::array<uint8_t, sizeof(output) / 2>();
        part_1_data[0] = high_byte;
        std::copy(begin(part_0_data), end(part_0_data), begin(output.m_val));
        std::copy(begin(part_1_data), end(part_1_data), begin(output.m_val) + output.m_val.size() / 2);
        return output;
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

    constexpr std::array<uint8_t, 2> ls_le_part() const
    {
        std::array<uint8_t, 2> output;
        std::copy(begin(m_val), begin(m_val) + 2, begin(output));
        return output;
    }

    constexpr std::array<uint8_t, 2> ms_le_part() const
    {
        std::array<uint8_t, 2> output;
        std::copy(begin(m_val) + 2, end(m_val), begin(output));
        return output;
    }
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

#else
#include "unsigned.hh"

namespace xm
{

using u32 = u<32>;

}

#endif
