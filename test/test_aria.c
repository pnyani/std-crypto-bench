/* ARIA-128: RFC 5794 Section 2, Test Vector #1. */

#include <stdint.h>
#include <stdio.h>
#include "test_utils.h"
#include "aria.h"

void run_aria_tests(void)
{
    FILE          *f = test_open_vectors("test/vectors/aria_ecb.txt");
    test_vector_t  v;
    uint8_t        out[16];

    while (test_load_vector(f, &v)) {
        aria128_ctx_t ctx;
        char          enc[32], dec[32];
        snprintf(enc, sizeof(enc), "ARIA-128 [%d] encrypt", v.count);
        snprintf(dec, sizeof(dec), "ARIA-128 [%d] decrypt", v.count);
        aria128_key_expand(v.key, &ctx);
        aria128_encrypt(v.pt, out, &ctx);
        test_assert_bytes(enc, v.ct, out, 16);
        aria128_decrypt(v.ct, out, &ctx);
        test_assert_bytes(dec, v.pt, out, 16);
    }
    fclose(f);
}
