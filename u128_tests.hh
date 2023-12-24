#include "u128.hh"

using namespace xm;

#define U128_LS_SET(ms) u128(ms, 0xffffffffffffffff)
#define U128_LS_ZERO(ms) u128(ms, 0x0000000000000000)

static_assert(U128_LS_SET(0xffffffffffffffff).overflow_with(1));

static_assert(u128(0x000000000000000f) + 1 == 0x0000000000000010);
static_assert(u128(0x00000000000000ff) + 1 == 0x0000000000000100);
static_assert(u128(0x0000000000000fff) + 1 == 0x0000000000001000);
static_assert(u128(0x000000000000ffff) + 1 == 0x0000000000010000);
static_assert(u128(0x00000000000fffff) + 1 == 0x0000000000100000);
static_assert(u128(0x0000000000ffffff) + 1 == 0x0000000001000000);
static_assert(u128(0x000000000fffffff) + 1 == 0x0000000010000000);
static_assert(u128(0x00000000ffffffff) + 1 == 0x0000000100000000);
static_assert(u128(0x0000000fffffffff) + 1 == 0x0000001000000000);
static_assert(u128(0x000000ffffffffff) + 1 == 0x0000010000000000);
static_assert(u128(0x00000fffffffffff) + 1 == 0x0000100000000000);
static_assert(u128(0x0000ffffffffffff) + 1 == 0x0001000000000000);
static_assert(u128(0x000fffffffffffff) + 1 == 0x0010000000000000);
static_assert(u128(0x00ffffffffffffff) + 1 == 0x0100000000000000);
static_assert(u128(0x0fffffffffffffff) + 1 == 0x1000000000000000);

static_assert(u128(0x000000000000000f) + 2  == 0x0000000000000011);
static_assert(u128(0x00000000000000ff) + 3  == 0x0000000000000102);
static_assert(u128(0x0000000000000fff) + 4  == 0x0000000000001003);
static_assert(u128(0x000000000000ffff) + 5  == 0x0000000000010004);
static_assert(u128(0x00000000000fffff) + 6  == 0x0000000000100005);
static_assert(u128(0x0000000000ffffff) + 7  == 0x0000000001000006);
static_assert(u128(0x000000000fffffff) + 8  == 0x0000000010000007);
static_assert(u128(0x00000000ffffffff) + 9  == 0x0000000100000008);
static_assert(u128(0x0000000fffffffff) + 10 == 0x0000001000000009);
static_assert(u128(0x000000ffffffffff) + 11 == 0x000001000000000a);
static_assert(u128(0x00000fffffffffff) + 12 == 0x000010000000000b);
static_assert(u128(0x0000ffffffffffff) + 13 == 0x000100000000000c);
static_assert(u128(0x000fffffffffffff) + 14 == 0x001000000000000d);
static_assert(u128(0x00ffffffffffffff) + 15 == 0x010000000000000e);
static_assert(u128(0x0fffffffffffffff) + 16 == 0x100000000000000f);

static_assert(U128_LS_SET(0x0000000000000000) + 1 == U128_LS_ZERO(0x0000000000000001));
static_assert(U128_LS_SET(0x000000000000000f) + 1 == U128_LS_ZERO(0x0000000000000010));
static_assert(U128_LS_SET(0x00000000000000ff) + 1 == U128_LS_ZERO(0x0000000000000100));
static_assert(U128_LS_SET(0x0000000000000fff) + 1 == U128_LS_ZERO(0x0000000000001000));
static_assert(U128_LS_SET(0x000000000000ffff) + 1 == U128_LS_ZERO(0x0000000000010000));
static_assert(U128_LS_SET(0x00000000000fffff) + 1 == U128_LS_ZERO(0x0000000000100000));
static_assert(U128_LS_SET(0x0000000000ffffff) + 1 == U128_LS_ZERO(0x0000000001000000));
static_assert(U128_LS_SET(0x000000000fffffff) + 1 == U128_LS_ZERO(0x0000000010000000));
static_assert(U128_LS_SET(0x00000000ffffffff) + 1 == U128_LS_ZERO(0x0000000100000000));
static_assert(U128_LS_SET(0x0000000fffffffff) + 1 == U128_LS_ZERO(0x0000001000000000));
static_assert(U128_LS_SET(0x000000ffffffffff) + 1 == U128_LS_ZERO(0x0000010000000000));
static_assert(U128_LS_SET(0x00000fffffffffff) + 1 == U128_LS_ZERO(0x0000100000000000));
static_assert(U128_LS_SET(0x0000ffffffffffff) + 1 == U128_LS_ZERO(0x0001000000000000));
static_assert(U128_LS_SET(0x000fffffffffffff) + 1 == U128_LS_ZERO(0x0010000000000000));
static_assert(U128_LS_SET(0x00ffffffffffffff) + 1 == U128_LS_ZERO(0x0100000000000000));
static_assert(U128_LS_SET(0x0fffffffffffffff) + 1 == U128_LS_ZERO(0x1000000000000000));

static_assert(
    u128(0x1020304050607080, 0x0090a0b0c0d0e0f0)
  + u128(0x0f0e0d0c0b0a0908, 0x0706050403020100) ==
    u128(0x1f2e3d4c5b6a7988, 0x0796a5b4c3d2e1f0)
);
