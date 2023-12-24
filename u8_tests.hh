#include "u8.hh"
#include <optional>

using namespace xm;

static_assert(u8(0xff) == u8(0xff));

static_assert(u8(0xff).overflow_with(1));

static_assert(u8(0xff).add_overflow(u8(0x1)) == u8(0x0));
static_assert(u8(0xff).add_overflow(u8(0x2)) == u8(0x1));
static_assert(u8(0x0f).add_overflow(u8(0x1)) == u8(0x10));
static_assert(u8(0x00).add_overflow(u8(0xaa)) == u8(0xaa));
static_assert(u8(0x00).add_overflow(u8(0xaa)) == u8(0xaa));

static_assert(u8(56) + u8(2) == u8(58));
