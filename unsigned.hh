#pragma once

#include <array>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <type_traits>

#include "xm_endian.hh"

namespace xm
{

template <size_t BITS>
concept PowerOf2 = []() -> bool
{
    size_t nb_bytes = 0;
    for (size_t bit_idx = 0; bit_idx < std::numeric_limits<size_t>::digits; bit_idx++)
    {
        size_t mask = 1;
        mask <<= bit_idx;
        if ((BITS & mask) != 0)
        {
            nb_bytes++;
        }

        if (nb_bytes > 1)
        {
            return false;
        }
    }

    if (nb_bytes > 1)
    {
        return false;
    }

    return true;
}();

template <size_t BITS>
    requires PowerOf2<BITS> && (BITS >= 8)
struct u;

template <size_t BITS>
    requires PowerOf2<BITS> && (BITS >= 8)
constexpr bool operator==(const u<BITS> &lhs, const u<BITS> &rhs);

template <> struct u<8>
{
    constexpr u(uint8_t val) : m_val(val)
    {
    }

    constexpr u(std::array<uint8_t, 1> val) : m_val(val[0])
    {
    }

    constexpr bool overflow_with(u r) const
    {
        return this->overflow_with(r.m_val);
    }

    constexpr bool overflow_with(uint8_t r) const
    {
        return this->m_val > (0xff - r);
    }

    // 0xff + 0x01 = 0x00
    [[nodiscard]] constexpr u add_overflow(u increment) const
    {
        if (!this->overflow_with(increment))
        {
            return u(this->m_val + increment.m_val);
        }

        return [](u l, u r)
        {
            if ((0xff - l.m_val) > r.m_val)
            {
                return u(0xff - ((0xff - l.m_val) - r.m_val));
            }
            return u(0xff - ((0xff - r.m_val) - l.m_val));
        }(*this, increment);
    }


    constexpr std::optional<u> operator+(const std::optional<u> &other)
    {
        if (!other.has_value())
        {
            return std::nullopt;
        }
        if (this->overflow_with(*other))
        {
            return std::nullopt;
        }
        return u(this->m_val).add_overflow(other->m_val);
    }

    friend constexpr bool operator==<8>(const u &lhs, const u &rhs);

    constexpr std::array<uint8_t, 1> data_le() const
    {
        return {m_val};
    }

    constexpr uint8_t data() const
    {
        return m_val;
    }

  private:
    uint8_t m_val;
};

template <> constexpr bool operator==(const u<8> &lhs, const u<8> &rhs)
{
    return lhs.m_val == rhs.m_val;
}

template <size_t BITS>
    requires PowerOf2<BITS> && (BITS >= 8)
struct u
{
    using u_prev_t = u<(BITS / 2)>;
    static constexpr size_t NB_BYTES = BITS / 8;

    constexpr u(u_prev_t ms_part, u_prev_t ls_part) : m_val(make_le(ms_part, ls_part))
    {
    }

    constexpr u(u_prev_t ls_part) : u(0, ls_part)
    {
    }

    constexpr u(std::array<uint8_t, NB_BYTES> little_endian_val) : m_val(little_endian_val)
    {
    }

    constexpr bool overflow_with(uint8_t rhs) const
    {
        return this->overflow_with(u<BITS>(rhs));
    }

    constexpr bool overflow_with(const u &rhs) const
    {
        /* SYMMETRY PADDING */

        u_prev_t L_hs_0(this->ls_le_part());
        u_prev_t L_hs_1(this->ms_le_part());
        u_prev_t R_hs_0(rhs.ls_le_part());
        u_prev_t R_hs_1(rhs.ms_le_part());

        bool part_0_overflow = u_prev_t(L_hs_0).overflow_with(R_hs_0);
        bool part_1_overflow = u_prev_t(L_hs_1).overflow_with(R_hs_1);

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

    [[nodiscard]] constexpr u add_overflow(u rhs) const
    {
        u_prev_t Lhs_0(this->ls_le_part());
        u_prev_t Lhs_1(this->ms_le_part());
        u_prev_t Rhs_0(rhs.ls_le_part());
        u_prev_t Rhs_1(rhs.ms_le_part());

        u_prev_t part_0 = Lhs_0.add_overflow(Rhs_0);
        auto part_0_data = part_0.data_le();

        u output = 0;
        std::copy(begin(part_0_data), end(part_0_data), begin(output.m_val));

        u_prev_t new_high(this->ms_le_part());
        if (Lhs_0.overflow_with(Rhs_0))
        {
            new_high = new_high.add_overflow(1);
        }
        new_high = new_high.add_overflow(Rhs_1);

        auto new_high_data = new_high.data_le();
        std::copy(begin(new_high_data), end(new_high_data), begin(output.m_val) + output.m_val.size() / 2);
        return output;
    }


    friend constexpr bool operator==<BITS>(const u &lhs, const u &rhs);

    static constexpr u sum_of(u_prev_t lhs, u_prev_t rhs)
    {
        uint8_t high_byte = 0;
        if (lhs.overflow_with(rhs))
        {
            high_byte = 1;
        }

        u output = 0;
        auto part_0_data = lhs.add_overflow(rhs).data_le();
        auto part_1_data = std::array<uint8_t, sizeof(output) / 2>();
        part_1_data[0] = high_byte;
        std::copy(begin(part_0_data), end(part_0_data), begin(output.m_val));
        std::copy(begin(part_1_data), end(part_1_data), begin(output.m_val) + output.m_val.size() / 2);
        return output;
    }

    constexpr std::array<uint8_t, NB_BYTES> data_le() const
    {
        return m_val;
    }

    constexpr std::array<uint8_t, NB_BYTES> data_be() const
    {
        std::array<uint8_t, NB_BYTES> output = m_val;
        std::reverse(begin(output), end(output));
        return output;
    }

  private:
    // Little endian
    std::array<uint8_t, NB_BYTES> m_val;

    static constexpr std::array<uint8_t, NB_BYTES> make_le(u_prev_t ms_part, u_prev_t ls_part)
    {
        std::array<uint8_t, NB_BYTES> output;
        std::array<uint8_t, NB_BYTES / 2> ls_data = ls_part.data_le();
        std::array<uint8_t, NB_BYTES / 2> ms_data = ms_part.data_le();
        std::copy(begin(ls_data), end(ls_data), begin(output));
        std::copy(begin(ms_data), end(ms_data), begin(output) + sizeof(u_prev_t));
        return output;
    }

    constexpr std::array<uint8_t, NB_BYTES / 2> ls_le_part() const
    {
        std::array<uint8_t, NB_BYTES / 2> output;
        std::copy(begin(m_val), begin(m_val) + m_val.size() / 2, begin(output));
        return output;
    }

    constexpr std::array<uint8_t, NB_BYTES / 2> ms_le_part() const
    {
        std::array<uint8_t, NB_BYTES / 2> output;
        std::copy(begin(m_val) + m_val.size() / 2, end(m_val), begin(output));
        return output;
    }
};

template <size_t BITS>
    requires PowerOf2<BITS> && (BITS >= 8)
constexpr bool operator==(const u<BITS> &lhs, const u<BITS> &rhs)
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
