#include <stdio.h>
#include <string.h>
#include "test_utils.h"

static int g_pass = 0;
static int g_fail = 0;

void test_assert_bytes(const char *name,
                       const uint8_t *expected,
                       const uint8_t *actual,
                       int len)
{
    if (memcmp(expected, actual, (size_t)len) == 0) {
        printf("  PASS  %s\n", name);
        g_pass++;
    } else {
        int i;
        printf("  FAIL  %s\n", name);
        printf("    expected: ");
        for (i = 0; i < len; i++) printf("%02x", expected[i]);
        printf("\n    actual  : ");
        for (i = 0; i < len; i++) printf("%02x", actual[i]);
        printf("\n");
        g_fail++;
    }
}

void run_aes_tests(void);
void run_aes_variant_tests(void);
void run_aria_tests(void);
void run_seed_tests(void);
void run_des_tests(void);

int main(void)
{
    printf("=== AES ===\n");         run_aes_tests();
    printf("=== AES Variant ===\n"); run_aes_variant_tests();
    printf("=== ARIA ===\n");        run_aria_tests();
    printf("=== SEED ===\n");        run_seed_tests();
    printf("=== DES / 3DES ===\n");  run_des_tests();

    printf("\nResult: %d passed, %d failed\n", g_pass, g_fail);
    return g_fail > 0 ? 1 : 0;
}
