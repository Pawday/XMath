#include "u16.hh"

using namespace xm;

static_assert(u16(0xfa01).add_overflow(0x0010) == u16(0xfa11));
static_assert(u16(0x0fff).add_overflow(0x0001) == u16(0x1000));
static_assert(u16(0xffff).add_overflow(0x0001).data() == u16(0x0000));

constexpr u16 a(0xffff);
