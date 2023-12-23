#include "u8_tests.hh"

#include "u16.hh"

#include "u16_tests.hh"

int main()
{
    using namespace xm;

    u16(0xffff).add_overflow(0x0001);
}
