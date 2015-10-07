#include "ovs-atomic.h"
#include <stdio.h>

static void test_atomic_flag(void)
{
    atomic_flag flag = ATOMIC_FLAG_INIT;
    ovs_assert(atomic_flag_test_and_set(&flag) == false);
    ovs_assert(atomic_flag_test_and_set(&flag) == true);
    atomic_flag_clear(&flag);
    ovs_assert(atomic_flag_test_and_set(&flag) == false);
}

#define TEST_ATOMIC_TYPE(ATOMIC_TYPE, BASE_TYPE)        \
    {                                                   \
        ATOMIC_TYPE x = ATOMIC_VAR_INIT(1);             \
        BASE_TYPE value, orig;                          \
                                                        \
        atomic_read(&x, &value);                        \
        ovs_assert(value == 1);                         \
                                                        \
        atomic_store(&x, 2);                            \
        atomic_read(&x, &value);                        \
        ovs_assert(value == 2);                         \
                                                        \
        atomic_init(&x, 3);                             \
        atomic_read(&x, &value);                        \
        ovs_assert(value == 3);                         \
                                                        \
        atomic_add(&x, 1, &orig);                       \
        ovs_assert(orig == 3);                          \
        atomic_read(&x, &value);                        \
        ovs_assert(value == 4);                         \
                                                        \
        atomic_sub(&x, 2, &orig);                       \
        ovs_assert(orig == 4);                          \
        atomic_read(&x, &value);                        \
        ovs_assert(value == 2);                         \
                                                        \
        atomic_or(&x, 6, &orig);                        \
        ovs_assert(orig == 2);                          \
        atomic_read(&x, &value);                        \
        ovs_assert(value == 6);                         \
                                                        \
        atomic_and(&x, 10, &orig);                      \
        ovs_assert(orig == 6);                          \
        atomic_read(&x, &value);                        \
        ovs_assert(value == 2);                         \
                                                        \
        atomic_xor(&x, 10, &orig);                      \
        ovs_assert(orig == 2);                          \
        atomic_read(&x, &value);                        \
        ovs_assert(value == 8);                         \
    }

static void
test_atomic_plain(void)
{
    TEST_ATOMIC_TYPE(atomic_char, char);
    TEST_ATOMIC_TYPE(atomic_uchar, unsigned char);
    TEST_ATOMIC_TYPE(atomic_schar, signed char);
    TEST_ATOMIC_TYPE(atomic_short, short);
    TEST_ATOMIC_TYPE(atomic_ushort, unsigned short);
    TEST_ATOMIC_TYPE(atomic_int, int);
    TEST_ATOMIC_TYPE(atomic_uint, unsigned int);
    TEST_ATOMIC_TYPE(atomic_long, long int);
    TEST_ATOMIC_TYPE(atomic_ulong, unsigned long int);
    TEST_ATOMIC_TYPE(atomic_llong, long long int);
    TEST_ATOMIC_TYPE(atomic_ullong, unsigned long long int);
    TEST_ATOMIC_TYPE(atomic_size_t, size_t);
    TEST_ATOMIC_TYPE(atomic_ptrdiff_t, ptrdiff_t);
    TEST_ATOMIC_TYPE(atomic_intmax_t, intmax_t);
    TEST_ATOMIC_TYPE(atomic_uintmax_t, uintmax_t);
    TEST_ATOMIC_TYPE(atomic_intptr_t, intptr_t);
    TEST_ATOMIC_TYPE(atomic_uintptr_t, uintptr_t);
    TEST_ATOMIC_TYPE(atomic_uint8_t, uint8_t);
    TEST_ATOMIC_TYPE(atomic_int8_t, int8_t);
    TEST_ATOMIC_TYPE(atomic_uint16_t, uint16_t);
    TEST_ATOMIC_TYPE(atomic_int16_t, int16_t);
    TEST_ATOMIC_TYPE(atomic_uint32_t, uint32_t);
    TEST_ATOMIC_TYPE(atomic_int32_t, int32_t);
}

#define TEST_ATOMIC_TYPE_EXPLICIT(ATOMIC_TYPE, BASE_TYPE,               \
                                  ORDER_READ, ORDER_STORE, ORDER_RMW)   \
    {                                                                   \
        ATOMIC_TYPE x = ATOMIC_VAR_INIT(1);                             \
        BASE_TYPE value, orig;                                          \
                                                                        \
        atomic_read_explicit(&x, &value, ORDER_READ);                   \
        ovs_assert(value == 1);                                         \
                                                                        \
        atomic_store_explicit(&x, 2, ORDER_STORE);                      \
        atomic_read_explicit(&x, &value, ORDER_READ);                   \
        ovs_assert(value == 2);                                         \
                                                                        \
        atomic_init(&x, 3);                                             \
        atomic_read_explicit(&x, &value, ORDER_READ);                   \
        ovs_assert(value == 3);                                         \
                                                                        \
        atomic_add_explicit(&x, 1, &orig, ORDER_RMW);                   \
        ovs_assert(orig == 3);                                          \
        atomic_read_explicit(&x, &value, ORDER_READ);                   \
        ovs_assert(value == 4);                                         \
                                                                        \
        atomic_sub_explicit(&x, 2, &orig, ORDER_RMW);                   \
        ovs_assert(orig == 4);                                          \
        atomic_read_explicit(&x, &value, ORDER_READ);                   \
        ovs_assert(value == 2);                                         \
                                                                        \
        atomic_or_explicit(&x, 6, &orig, ORDER_RMW);                    \
        ovs_assert(orig == 2);                                          \
        atomic_read_explicit(&x, &value, ORDER_READ);                   \
        ovs_assert(value == 6);                                         \
                                                                        \
        atomic_and_explicit(&x, 10, &orig, ORDER_RMW);                  \
        ovs_assert(orig == 6);                                          \
        atomic_read_explicit(&x, &value, ORDER_READ);                   \
        ovs_assert(value == 2);                                         \
                                                                        \
        atomic_xor_explicit(&x, 10, &orig, ORDER_RMW);                  \
        ovs_assert(orig == 2);                                          \
        atomic_read_explicit(&x, &value, ORDER_READ);                   \
        ovs_assert(value == 8);                                         \
    }

#define TEST_ATOMIC_ORDER(ORDER_READ, ORDER_STORE, ORDER_RMW)           \
    {                                                                   \
        TEST_ATOMIC_TYPE_EXPLICIT(atomic_char, char,                    \
                                  ORDER_READ, ORDER_STORE, ORDER_RMW);  \
        TEST_ATOMIC_TYPE_EXPLICIT(atomic_uchar, unsigned char,          \
                                  ORDER_READ, ORDER_STORE, ORDER_RMW);  \
        TEST_ATOMIC_TYPE_EXPLICIT(atomic_schar, signed char,            \
                                  ORDER_READ, ORDER_STORE, ORDER_RMW);  \
        TEST_ATOMIC_TYPE_EXPLICIT(atomic_short, short,                  \
                                  ORDER_READ, ORDER_STORE, ORDER_RMW);  \
        TEST_ATOMIC_TYPE_EXPLICIT(atomic_ushort, unsigned short,        \
                                  ORDER_READ, ORDER_STORE, ORDER_RMW);  \
        TEST_ATOMIC_TYPE_EXPLICIT(atomic_int, int,                      \
                                  ORDER_READ, ORDER_STORE, ORDER_RMW);  \
        TEST_ATOMIC_TYPE_EXPLICIT(atomic_uint, unsigned int,            \
                                  ORDER_READ, ORDER_STORE, ORDER_RMW);  \
        TEST_ATOMIC_TYPE_EXPLICIT(atomic_long, long int,                \
                                  ORDER_READ, ORDER_STORE, ORDER_RMW);  \
        TEST_ATOMIC_TYPE_EXPLICIT(atomic_ulong, unsigned long int,      \
                                  ORDER_READ, ORDER_STORE, ORDER_RMW);  \
        TEST_ATOMIC_TYPE_EXPLICIT(atomic_llong, long long int,          \
                                  ORDER_READ, ORDER_STORE, ORDER_RMW);  \
        TEST_ATOMIC_TYPE_EXPLICIT(atomic_ullong, unsigned long long int, \
                                  ORDER_READ, ORDER_STORE, ORDER_RMW);  \
        TEST_ATOMIC_TYPE_EXPLICIT(atomic_size_t, size_t,                \
                                  ORDER_READ, ORDER_STORE, ORDER_RMW);  \
        TEST_ATOMIC_TYPE_EXPLICIT(atomic_ptrdiff_t, ptrdiff_t,          \
                                  ORDER_READ, ORDER_STORE, ORDER_RMW);  \
        TEST_ATOMIC_TYPE_EXPLICIT(atomic_intmax_t, intmax_t,            \
                                  ORDER_READ, ORDER_STORE, ORDER_RMW);  \
        TEST_ATOMIC_TYPE_EXPLICIT(atomic_uintmax_t, uintmax_t,          \
                                  ORDER_READ, ORDER_STORE, ORDER_RMW);  \
        TEST_ATOMIC_TYPE_EXPLICIT(atomic_intptr_t, intptr_t,            \
                                  ORDER_READ, ORDER_STORE, ORDER_RMW);  \
        TEST_ATOMIC_TYPE_EXPLICIT(atomic_uintptr_t, uintptr_t,          \
                                  ORDER_READ, ORDER_STORE, ORDER_RMW);  \
        TEST_ATOMIC_TYPE_EXPLICIT(atomic_uint8_t, uint8_t,              \
                                  ORDER_READ, ORDER_STORE, ORDER_RMW);  \
        TEST_ATOMIC_TYPE_EXPLICIT(atomic_int8_t, int8_t,                \
                                  ORDER_READ, ORDER_STORE, ORDER_RMW);  \
        TEST_ATOMIC_TYPE_EXPLICIT(atomic_uint16_t, uint16_t,            \
                                  ORDER_READ, ORDER_STORE, ORDER_RMW);  \
        TEST_ATOMIC_TYPE_EXPLICIT(atomic_int16_t, int16_t,              \
                                  ORDER_READ, ORDER_STORE, ORDER_RMW);  \
        TEST_ATOMIC_TYPE_EXPLICIT(atomic_uint32_t, uint32_t,            \
                                  ORDER_READ, ORDER_STORE, ORDER_RMW);  \
        TEST_ATOMIC_TYPE_EXPLICIT(atomic_int32_t, int32_t,              \
                                  ORDER_READ, ORDER_STORE, ORDER_RMW);  \
    }

static void
test_atomic_relaxed(void)
{
    TEST_ATOMIC_ORDER(memory_order_relaxed, memory_order_relaxed,
                      memory_order_relaxed);
}

static void
test_atomic_consume(void)
{
    TEST_ATOMIC_ORDER(memory_order_consume, memory_order_release,
                      memory_order_release);
}

static void
test_atomic_acquire(void)
{
    TEST_ATOMIC_ORDER(memory_order_acquire, memory_order_release,
                      memory_order_release);
}

static void
test_atomic_acq_rel(void)
{
    TEST_ATOMIC_ORDER(memory_order_acquire, memory_order_release,
                      memory_order_acq_rel);
}

static void
test_atomic_seq_cst(void)
{
    TEST_ATOMIC_ORDER(memory_order_seq_cst, memory_order_seq_cst,
                      memory_order_seq_cst);
}

int main(int argc, char **argv) {
    printf("test atomic flag\n");
    test_atomic_flag();
    test_atomic_relaxed();
    test_atomic_consume();
    test_atomic_acquire();
    test_atomic_acq_rel();
    test_atomic_seq_cst();
}
