#include "u32.hh"

using namespace xm;

static_assert(u32(0xffffffff).overflow_with(1));
static_assert(u32(0x0fffffff) + 1 == u32(0x10000000));
static_assert(u32(0x00ffffff) + 1 == u32(0x01000000));
static_assert(u32(0x000fffff) + 1 == u32(0x00100000));
static_assert(u32(0x0000ffff) + 1 == u32(0x00010000));
static_assert(u32(0x00000fff) + 1 == u32(0x00001000));
static_assert(u32(0x000000ff) + 1 == u32(0x00000100));
static_assert(u32(0x0000000f) + 1 == u32(0x00000010));

static_assert(u32(0xa0b0c0d0) + 0x01020304 == 0xa1b2c3d4);
