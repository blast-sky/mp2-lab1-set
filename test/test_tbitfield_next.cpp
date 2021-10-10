#include <gtest.h>
#include "tbitfield.h"

TEST(TBitField, can_set_last_bit_1)
{
    const size_t num_el = 1;
    const size_t n = sizeof(elType) * 8 * num_el;
    TBitField field(n);
    ASSERT_EQ(field.getNumBytes(), sizeof(elType) * num_el);
    ASSERT_NO_THROW(field.setBit(n - 1));
    ASSERT_EQ(field.getBit(n - 1), true);
}

TEST(TBitField, can_set_last_bit_2)
{
    const size_t num_el = 15;
    const size_t n = sizeof(elType) * 8 * num_el;
    TBitField field(n);
    ASSERT_EQ(field.getNumBytes(), sizeof(elType) * num_el);
    ASSERT_NO_THROW(field.setBit(n - 1));
    ASSERT_EQ(field.getBit(n - 1), true);
}

TEST(TBitField, can_not_set_bit_after_last_1)
{
    const size_t num_el = 1;
    const size_t n = sizeof(elType) * num_el;
    TBitField field(n);
    ASSERT_ANY_THROW(field.setBit(n));
}

TEST(TBitField, can_not_set_bit_after_last_2)
{
    const size_t num_el = 21;
    const size_t n = sizeof(elType) * 8 * num_el;
    TBitField field(n);
    ASSERT_ANY_THROW(field.setBit(n));
}
