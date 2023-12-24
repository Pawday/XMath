#include "u16.hh"
#include <optional>

using namespace xm;



static_assert(u16(851) + u16(3) == 854);

static_assert(u16(0xfa01).add_overflow(0x0010) == u16(0xfa11));
static_assert(u16(0x0fff).add_overflow(0x0001) == u16(0x1000));
static_assert(u16(0xffff).add_overflow(0x0001) == u16(0x0000));

static_assert(u16(0xffff) + 1 == std::nullopt);
static_assert(u16(0xffff).add_overflow(1) == 0);

static_assert(u16(0x000f) + 1 == 0x0010);
static_assert(u16(0x00ff) + 1 == 0x0100);
static_assert(u16(0x0fff) + 1 == 0x1000);


static_assert(u16(0xa0b0) + 0x0102 == 0xa1b2);


