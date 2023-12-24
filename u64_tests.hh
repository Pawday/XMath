#include "u64.hh"

using namespace xm;

static_assert(u64(0xffffffffffffffff).overflow_with(1));

static_assert(u64(0x000000000000000f) + 1 == u64(0x0000000000000010));
static_assert(u64(0x00000000000000ff) + 1 == u64(0x0000000000000100));
static_assert(u64(0x0000000000000fff) + 1 == u64(0x0000000000001000));
static_assert(u64(0x000000000000ffff) + 1 == u64(0x0000000000010000));
static_assert(u64(0x00000000000fffff) + 1 == u64(0x0000000000100000));
static_assert(u64(0x0000000000ffffff) + 1 == u64(0x0000000001000000));
static_assert(u64(0x000000000fffffff) + 1 == u64(0x0000000010000000));
static_assert(u64(0x00000000ffffffff) + 1 == u64(0x0000000100000000));
static_assert(u64(0x0000000fffffffff) + 1 == u64(0x0000001000000000));
static_assert(u64(0x000000ffffffffff) + 1 == u64(0x0000010000000000));
static_assert(u64(0x00000fffffffffff) + 1 == u64(0x0000100000000000));
static_assert(u64(0x0000ffffffffffff) + 1 == u64(0x0001000000000000));
static_assert(u64(0x000fffffffffffff) + 1 == u64(0x0010000000000000));
static_assert(u64(0x00ffffffffffffff) + 1 == u64(0x0100000000000000));
static_assert(u64(0x0fffffffffffffff) + 1 == u64(0x1000000000000000));

static_assert(u64(0xa0b0c0d0e0f00000) + 0x0102030405060708 == u64(0xa1b2c3d4e5f60708));

