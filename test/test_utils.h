#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdint.h>
#include <stdio.h>

void test_assert_bytes(const char *name,
                       const uint8_t *expected,
                       const uint8_t *actual,
                       int len);

#define TV_MAX_KEY_BYTES 32

typedef struct {
    int     count;
    uint8_t key[TV_MAX_KEY_BYTES];
    int     key_len;
    uint8_t pt[16];
    uint8_t ct[16];
} test_vector_t;

FILE *test_open_vectors(const char *path);
int   test_load_vector(FILE *f, test_vector_t *v);

#endif /* TEST_UTILS_H */
