#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdint.h>

void test_assert_bytes(const char *name,
                       const uint8_t *expected,
                       const uint8_t *actual,
                       int len);

#endif /* TEST_UTILS_H */
