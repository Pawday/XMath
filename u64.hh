#pragma once

#if 0

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
        u32 Lhs_0(this->ls_le_part());
        u32 Lhs_1(this->ms_le_part());
        u32 Rhs_0(rhs.ls_le_part());
        u32 Rhs_1(rhs.ms_le_part());

        bool part_0_overflow = u32(Lhs_0).overflow_with(Rhs_0);
        bool part_1_overflow = u32(Lhs_1).overflow_with(Rhs_1);

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

        return Lhs_1.overflow_with(1);
    }

    [[nodiscard]] constexpr u64 add_overflow(u64 rhs) const
    {
        u32 Lhs_0(this->ls_le_part());
        u32 Lhs_1(this->ms_le_part());
        u32 Rhs_0(rhs.ls_le_part());
        u32 Rhs_1(rhs.ms_le_part());

        u32 part_0 = Lhs_0.add_overflow(Rhs_0);
        auto part_0_data = part_0.data_le();

        u64 output = 0;
        std::copy(begin(part_0_data), end(part_0_data), begin(output.m_val));

        u32 new_high(this->ms_le_part());
        if (Lhs_0.overflow_with(Rhs_0))
        {
            new_high = new_high.add_overflow(1);
        }
        new_high = new_high.add_overflow(Rhs_1);

        auto new_high_data = new_high.data_le();
        std::copy(begin(new_high_data), end(new_high_data), begin(output.m_val) + output.m_val.size() / 2);
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

        u64 output = 0;
        auto part_0_data = lhs.add_overflow(rhs).data_le();
        auto part_1_data = std::array<uint8_t, sizeof(output) / 2>();
        part_1_data[0] = high_byte;
        std::copy(begin(part_0_data), end(part_0_data), begin(output.m_val));
        std::copy(begin(part_1_data), end(part_1_data), begin(output.m_val) + output.m_val.size() / 2);
        return output;
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

    constexpr std::array<uint8_t, 4> ls_le_part() const
    {
        std::array<uint8_t, 4> output;
        std::copy(begin(m_val), begin(m_val) + 4, begin(output));
        return output;
    }

    constexpr std::array<uint8_t, 4> ms_le_part() const
    {
        std::array<uint8_t, 4> output;
        std::copy(begin(m_val) + 4, begin(m_val) + 8, begin(output));
        return output;
    }
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

#else

#include "unsigned.hh"

namespace xm
{

using u64 = u<64>;

}

#endif
