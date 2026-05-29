#include <stdio.h>
#include <stdlib.h>
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

static int hex_val(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

static int parse_hex(const char *s, uint8_t *buf, int maxlen)
{
    int n = 0;
    while (s[0] && s[1] && n < maxlen) {
        int hi = hex_val(s[0]);
        int lo = hex_val(s[1]);
        if (hi < 0 || lo < 0) break;
        buf[n++] = (uint8_t)((hi << 4) | lo);
        s += 2;
    }
    return n;
}

FILE *test_open_vectors(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "Cannot open vector file: %s\n", path);
        exit(1);
    }
    return f;
}

int test_load_vector(FILE *f, test_vector_t *v)
{
    char line[128];
    int  got_key = 0, got_pt = 0, got_ct = 0;
    v->count = -1;

    while (fgets(line, sizeof(line), f)) {
        char *p = line;
        while (*p == ' ' || *p == '\t') p++;
        if (*p == '#' || *p == '\n' || *p == '\r') continue;

        if (strncmp(p, "COUNT = ", 8) == 0) {
            v->count = atoi(p + 8);
        } else if (strncmp(p, "KEY = ", 6) == 0) {
            v->key_len = parse_hex(p + 6, v->key, TV_MAX_KEY_BYTES);
            got_key = 1;
        } else if (strncmp(p, "PLAINTEXT = ", 12) == 0) {
            parse_hex(p + 12, v->pt, 16);
            got_pt = 1;
        } else if (strncmp(p, "CIPHERTEXT = ", 13) == 0) {
            parse_hex(p + 13, v->ct, 16);
            got_ct = 1;
        }

        if (got_key && got_pt && got_ct) return 1;
    }
    return 0;
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
