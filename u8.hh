#pragma once

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

} // namespace xm
